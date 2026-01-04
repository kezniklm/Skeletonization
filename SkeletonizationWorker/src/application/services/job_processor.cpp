#include <chrono>
#include <expected>
#include <filesystem>

#include <cctype>

#include <glog/logging.h>

#include "SkeletonizationWorker/application/services/job_processor.hpp"
#include "SkeletonizationWorker/domain/output_format.hpp"

namespace worker::application::services
{
	namespace
	{
		std::string sanitize_filename(std::string value)
		{
			for (auto& ch : value)
			{
				const auto uc = static_cast<unsigned char>(ch);
				if (std::isalnum(uc) == 0 && ch != '.' && ch != '-' && ch != '_')
				{
					ch = '_';
				}
			}
			if (value.empty())
			{
				value = "file";
			}
			return value;
		}

		bool file_exists(const std::string& path)
		{
			std::error_code ec;
			return std::filesystem::exists(std::filesystem::path{path}, ec);
		}
	}

	job_processor::job_processor(interfaces::i_image_service& image_service,
	                             interfaces::i_object_storage& object_storage,
	                             interfaces::i_skeletonization_processor& processor,
	                             interfaces::i_result_publisher& publisher,
	                             configuration::dependency_injection::output_directory_t output_directory)
		: image_service_(image_service),
		  object_storage_(object_storage),
		  skeletonization_processor_(processor),
		  result_publisher_(publisher),
		  output_directory_(std::move(output_directory.value))
	{
		if (!std::filesystem::exists(output_directory_))
		{
			std::filesystem::create_directories(output_directory_);
			LOG(INFO) << "Created output directory: " << output_directory_;
		}
	}

	std::expected<void, std::string> job_processor::process_job(const job::job& job_data)
	{
		LOG(INFO) << "Processing job: " << job_data.id << " with " << job_data.tasks.size() << " tasks";

		size_t successful_tasks = 0;
		size_t failed_tasks = 0;

		for (const auto& task : job_data.tasks)
		{
			const auto result = process_task(job_data.id, task);

			if (result)
			{
				++successful_tasks;
			}
			else
			{
				++failed_tasks;
				LOG(ERROR) << "Task failed for " << task.path << ": " << result.error();
			}
		}

		LOG(INFO) << "Job " << job_data.id << " completed: "
			<< successful_tasks << " successful, "
			<< failed_tasks << " failed";

		if (failed_tasks > 0 && successful_tasks == 0)
		{
			return std::unexpected("All tasks failed for job " + job_data.id);
		}

		return {};
	}

	std::expected<void, std::string> job_processor::process_task(const std::string& job_id,
	                                                             const job::image_task& task) const
	{
		LOG(INFO) << "Processing task: " << task.path << " with algorithm " << task.algorithm;

		const bool is_remote = object_storage_.is_remote_backend();
		const bool can_use_local_input = file_exists(task.path);

		LOG(INFO) << "Storage backend: " << (is_remote ? "remote (S3)" : "local")
			<< ", local file exists: " << (can_use_local_input ? "yes" : "no");

		const std::filesystem::path inputs_dir = std::filesystem::path(output_directory_) / "inputs";
		const std::filesystem::path outputs_dir = std::filesystem::path(output_directory_) / "outputs";
		std::error_code ec;
		std::filesystem::create_directories(inputs_dir, ec);
		std::filesystem::create_directories(outputs_dir, ec);

		std::filesystem::path input_path_local;
		if (is_remote && !can_use_local_input)
		{
			const std::filesystem::path key_path(task.path);
			const std::string input_filename = sanitize_filename(job_id + "_" + key_path.filename().string());
			input_path_local = inputs_dir / input_filename;

			LOG(INFO) << "Downloading from S3: " << task.path << " -> " << input_path_local.string();

			const auto dl = object_storage_.download_to_file(task.path, input_path_local);
			if (!dl)
			{
				const auto error = "Failed to download image object: " + dl.error();
				LOG(ERROR) << error;
				const auto publish_result = result_publisher_.publish_task_result(
					job_id, task.path, task.algorithm, "", false, 0.0, error);
				if (!publish_result)
				{
					LOG(WARNING) << "Failed to publish failure result: " << publish_result.error();
				}
				return std::unexpected(error);
			}
			LOG(INFO) << "Downloaded successfully to: " << input_path_local.string();
		}
		else
		{
			input_path_local = std::filesystem::path(task.path);
			LOG(INFO) << "Using local path: " << input_path_local.string();
		}

		const auto image_result = image_service_.load_image(input_path_local.string());

		if (!image_result)
		{
			const auto error = "Failed to load image: " + image_result.error();

			const auto publish_result = result_publisher_.publish_task_result(
				job_id, task.path, task.algorithm, "", false, 0.0, error);

			if (!publish_result)
			{
				LOG(WARNING) << "Failed to publish failure result: " << publish_result.error();
			}

			return std::unexpected(error);
		}

		const auto start_time = std::chrono::high_resolution_clock::now();

		const auto processed_result = skeletonization_processor_.process(
			image_result.value(), task.algorithm, task.should_run_preprocessing);

		const auto end_time = std::chrono::high_resolution_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
		const double processing_time_ms = duration.count() / 1000.0;

		LOG(INFO) << "Algorithm " << task.algorithm << " took " << processing_time_ms << "ms";

		if (!processed_result)
		{
			const auto error = "Failed to process image: " + processed_result.error();

			const auto publish_result = result_publisher_.publish_task_result(
				job_id, task.path, task.algorithm, "", false, processing_time_ms, error);

			if (!publish_result)
			{
				LOG(WARNING) << "Failed to publish failure result: " << publish_result.error();
			}

			return std::unexpected(error);
		}

		const std::filesystem::path input_name_for_output = input_path_local;

		const std::string output_extension{job::to_extension(task.format)};
		const std::string output_mime_type{job::to_mime_type(task.format)};

		const std::string output_filename = sanitize_filename(
			job_id + "_" + input_name_for_output.stem().string() + "_" + task.algorithm + "." + output_extension);

		const std::filesystem::path output_path_local = outputs_dir / output_filename;

		const auto save_result = image_service_.save_image(processed_result.value(), output_path_local.string());

		if (!save_result)
		{
			const auto error = "Failed to save image: " + save_result.error();

			const auto publish_result = result_publisher_.publish_task_result(
				job_id, task.path, task.algorithm, "", false, processing_time_ms, error);
			if (!publish_result)
			{
				LOG(WARNING) << "Failed to publish failure result: " << publish_result.error();
			}

			return std::unexpected(error);
		}

		std::string published_output_ref = output_path_local.string();
		if (object_storage_.is_remote_backend())
		{
			const std::string output_key = std::string{"outputs/"} + output_filename;
			interfaces::object_put_options put_options{};
			put_options.content_type = output_mime_type;
			const auto ul = object_storage_.upload_from_file(output_path_local, output_key, put_options);
			if (!ul)
			{
				const auto error = "Failed to upload output image: " + ul.error();
				const auto publish_failure = result_publisher_.publish_task_result(
					job_id, task.path, task.algorithm, "", false, processing_time_ms, error);
				if (!publish_failure)
				{
					LOG(WARNING) << "Failed to publish failure result: " << publish_failure.error();
				}
				return std::unexpected(error);
			}
			published_output_ref = output_key;
		}

		const auto publish_result = result_publisher_.publish_task_result(
			job_id, task.path, task.algorithm, published_output_ref, true, processing_time_ms);

		if (!publish_result)
		{
			LOG(WARNING) << "Failed to publish result, but task completed: " << publish_result.error();
		}

		return {};
	}
}
