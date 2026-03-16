/**
*
* @file exporter.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements benchmark export routines.
*
* This file implements JSON export of benchmark outputs and configurations.
*
* Main responsibilities:
* - write benchmark package data to JSON
* - write configuration metadata to JSON
* - create timestamped export output paths
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCLI/benchmark/exporter.hpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

#include "glog/logging.h"
#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "SkeletonizationCLI/benchmark/aggregator.hpp"
#include "SkeletonizationCLI/utils/base64_encoder.hpp"

namespace skeletonization_benchmark
{
	namespace
	{
		/**
		 * @brief Creates timestamped output directory under outputs root.
		 *
		 * @return Filesystem path to created timestamped directory.
		 */
		std::filesystem::path create_timestamped_output_directory()
		{
			const auto now = std::chrono::system_clock::now();
			const auto time = std::chrono::system_clock::to_time_t(now);
			std::tm tm{};
#ifdef _WIN32
			localtime_s(&tm, &time);
#else
			localtime_r(&time, &tm);
#endif
			std::ostringstream oss;
			oss << std::put_time(&tm, "%Y%m%d_%H%M%S");

			const auto outputs_dir = std::filesystem::path("outputs") / oss.str();
			std::filesystem::create_directories(outputs_dir);

			return outputs_dir;
		}

		using rapidjson_allocator = rapidjson::Document::AllocatorType;

		/**
		 * @brief Adds string member to RapidJSON object.
		 *
		 * @param obj Target JSON object.
		 * @param key Member key.
		 * @param s Member string value.
		 * @param alloc RapidJSON allocator.
		 */
		void set_string_member(rapidjson::Value& obj,
		                       const char* key,
		                       const std::string& s,
		                       rapidjson_allocator& alloc)
		{
			rapidjson::Value value;
			value.SetString(s.data(), static_cast<rapidjson::SizeType>(s.size()), alloc);
			obj.AddMember(rapidjson::Value(key, alloc), value, alloc);
		}

		/**
		 * @brief Adds string member only when value is non-empty.
		 *
		 * @param obj Target JSON object.
		 * @param key Member key.
		 * @param s Candidate value.
		 * @param alloc RapidJSON allocator.
		 */
		void add_if_nonempty_string(rapidjson::Value& obj,
		                            const char* key,
		                            const std::string& s,
		                            rapidjson_allocator& alloc)
		{
			if (!s.empty())
			{
				set_string_member(obj, key, s, alloc);
			}
		}

		/**
		 * @brief Adds numeric member when value is strictly positive.
		 *
		 * @tparam T Numeric value type.
		 * @param obj Target JSON object.
		 * @param key Member key.
		 * @param v Candidate value.
		 * @param alloc RapidJSON allocator.
		 */
		template <class T>
		void add_if_positive(rapidjson::Value& obj,
		                     const char* key,
		                     const T& v,
		                     rapidjson_allocator& alloc)
		{
			if constexpr (std::is_integral_v<T>)
			{
				if (v > 0)
				{
					obj.AddMember(rapidjson::StringRef(key), v, alloc);
				}
			}
			else
			{
				if (v > static_cast<T>(0))
				{
					obj.AddMember(rapidjson::StringRef(key), v, alloc);
				}
			}
		}

		/**
		 * @brief Builds local ISO-8601 timestamp without timezone suffix.
		 *
		 * @return Timestamp string in yyyy-mm-ddThh:mm:ss format.
		 */
		std::string make_timestamp_iso8601_local()
		{
			const auto now = std::chrono::system_clock::now();
			const auto t = std::chrono::system_clock::to_time_t(now);
			std::tm tm{};
#ifdef _WIN32
			localtime_s(&tm, &t);
#else
			localtime_r(&t, &tm);
#endif
			std::ostringstream oss;
			oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
			return oss.str();
		}

		/**
		 * @brief Appends timestamp member to export document root.
		 *
		 * @param document Target document.
		 * @param allocator RapidJSON allocator.
		 */
		void add_timestamp_member(rapidjson::Document& document,
		                          rapidjson_allocator& allocator)
		{
			const auto timestamp = make_timestamp_iso8601_local();

			rapidjson::Value key("timestamp", allocator);

			rapidjson::Value value;
			value.SetString(timestamp.data(),
			                static_cast<rapidjson::SizeType>(timestamp.size()),
			                allocator);

			document.AddMember(key, value, allocator);
		}

		/**
		 * @brief Encodes matrix image buffer to base64 string.
		 *
		 * @param matrix Input matrix to encode.
		 * @param opts Export options controlling codec parameters.
		 * @param out_b64 Output base64 payload.
		 * @return True when encoding succeeds.
		 */
		bool encode_mat_to_base64(const cv::Mat& matrix,
		                          const exporter_options& opts,
		                          std::string& out_b64)
		{
			std::vector<int> params;

			if (cli::utils::equals_ignore_case(opts.image_ext, ".png"))
			{
				params.push_back(cv::IMWRITE_PNG_COMPRESSION);
				params.push_back(std::clamp(opts.png_compression, 0, 9));
			}
			else if (cli::utils::equals_ignore_case(opts.image_ext, ".jpg") ||
				cli::utils::equals_ignore_case(opts.image_ext, ".jpeg"))
			{
				params.push_back(cv::IMWRITE_JPEG_QUALITY);
				params.push_back(std::clamp(opts.jpeg_quality, 1, 100));
			}

			std::vector<uchar> encoded;
			if (!cv::imencode(opts.image_ext, matrix, encoded, params))
			{
				return false;
			}

			return cli::utils::base64_encoder::encode(encoded, out_b64);
		}

		/**
		 * @brief Sanitizes identifier fragment for stable JSON image IDs.
		 *
		 * @param s Raw identifier fragment.
		 * @return Sanitized identifier with unsupported characters removed.
		 */
		std::string sanitize_id_component(std::string_view s)
		{
			std::string output;

			for (const unsigned char ch : s)
			{
				if (std::isalnum(ch) || ch == '_' || ch == '-')
				{
					output.push_back(static_cast<char>(ch));
				}
				else if (std::isspace(ch))
				{
					output.push_back('_');
				}
			}

			while (!output.empty() &&
				(output.back() == '_' || output.back() == '-'))
			{
				output.pop_back();
			}

			return output;
		}

		/**
		 * @brief Creates deterministic image identifier for export payload.
		 *
		 * @param container_name Source container name.
		 * @param label Image label.
		 * @param index Image index in container.
		 * @return Stable image identifier.
		 */
		std::string make_image_id(std::string_view container_name,
		                          std::string_view label,
		                          std::size_t index)
		{
			std::string container = sanitize_id_component(container_name);
			std::string label_name = sanitize_id_component(label);
			if (label_name.empty())
			{
				label_name = "img";
			}

			std::string id;
			id.reserve(container.size() + 1 + label_name.size() + 20);
			id.append(container).push_back('_');
			id.append(label_name).push_back('_');
			id.append(std::to_string(index));
			return id;
		}

		/**
		 * @brief Write a RapidJSON document to a file with optional atomic write.
		 * @param document The RapidJSON document to write
		 * @param out_path The output file path
		 * @param opts Export options (pretty printing, atomic write)
		 * @param error_context Context string for error messages (e.g., "output JSON", "configuration JSON")
		 * @return true if successful, false otherwise
		 */
		bool write_json_document(
			const rapidjson::Document& document,
			const std::filesystem::path& out_path,
			const exporter_options& opts,
			const std::string& error_context)
		{
			try
			{
				if (opts.atomic_write)
				{
					const auto tmp = out_path.string() + ".tmp";
					{
						std::ofstream ofstream(tmp, std::ios::binary | std::ios::trunc);

						if (!ofstream)
						{
							LOG(ERROR) << "Failed to open temporary file for writing: " << tmp;
							return false;
						}

						rapidjson::OStreamWrapper o_stream_wrapper(ofstream);

						if (opts.pretty)
						{
							rapidjson::PrettyWriter writer(o_stream_wrapper);
							writer.SetIndent(' ', 2);
							document.Accept(writer);
						}
						else
						{
							rapidjson::Writer writer(o_stream_wrapper);
							document.Accept(writer);
						}
					}

					std::error_code error_code;
					std::filesystem::rename(tmp, out_path, error_code);

					if (error_code)
					{
						LOG(ERROR) << "Failed to rename temporary file '" << tmp
							<< "' to '" << out_path.string()
							<< "': " << error_code.message();
						return false;
					}
				}
				else
				{
					std::ofstream ofstream(out_path, std::ios::binary | std::ios::trunc);

					if (!ofstream)
					{
						LOG(ERROR) << "Failed to open " << error_context << " file for writing: " << out_path.string();
						return false;
					}

					rapidjson::OStreamWrapper osw(ofstream);

					if (opts.pretty)
					{
						rapidjson::PrettyWriter writer(osw);
						writer.SetIndent(' ', 2);
						document.Accept(writer);
					}
					else
					{
						rapidjson::Writer writer(osw);
						document.Accept(writer);
					}
				}

				return true;
			}
			catch (const std::exception& e)
			{
				LOG(ERROR) << "Failed to write " << error_context << " '" << out_path.string()
					<< "': " << e.what();
				return false;
			}
			catch (...)
			{
				LOG(ERROR) << "Unknown error while writing " << error_context << ": " << out_path.string();
				return false;
			}
		}
	}

	/**
	 * @brief Writes benchmark output package JSON.
	 *
	 * @param packages Aggregated benchmark packages.
	 * @param out_json_path Destination JSON path.
	 * @param opts Export behavior options.
	 * @return True when JSON was written successfully.
	 */
	bool exporter::write_output_json(const std::vector<aggregator::package>& packages,
	                                 const std::filesystem::path& out_json_path,
	                                 const exporter_options& opts)
	{
		if (out_json_path.empty())
		{
			return false;
		}

		try
		{
			std::filesystem::create_directories(out_json_path.parent_path());

			rapidjson::Document document;
			document.SetObject();

			auto& allocator = document.GetAllocator();

			add_timestamp_member(document, allocator);

			rapidjson::Value containers(rapidjson::kArrayType);

			for (const auto& [container,
				     metrics_list,
				     description,
				     avg_time_ms,
				     min_time_ms,
				     max_time_ms,
				     total_iterations,
				     avg_iterations,
				     std_dev_ms,
				     throughput_alg_per_s,
				     note] : packages)
			{
				rapidjson::Value value(rapidjson::kObjectType);

				set_string_member(value, "name", container.name(), allocator);

				if (opts.include_algorithm_info)
				{
					rapidjson::Value info(rapidjson::kObjectType);
					add_if_nonempty_string(info, "description", description, allocator);
					add_if_positive(info, "avgTime", avg_time_ms, allocator);
					add_if_positive(info, "minTime", min_time_ms, allocator);
					add_if_positive(info, "maxTime", max_time_ms, allocator);
					add_if_positive(info, "totalIterations", total_iterations, allocator);
					add_if_positive(info, "avgIterations", avg_iterations, allocator);
					add_if_positive(info, "stdDev", std_dev_ms, allocator);
					add_if_positive(info, "throughput", throughput_alg_per_s, allocator);
					add_if_nonempty_string(info, "complexity", note, allocator);

					if (!info.ObjectEmpty())
					{
						value.AddMember("algorithmInfo", info, allocator);
					}
				}

				rapidjson::Value images(rapidjson::kArrayType);

				for (std::size_t index = 0; index < container.size(); ++index)
				{
					const auto& matrix = container.image(index);
					const auto& label = container.label(index);

					std::string base_64;
					if (!encode_mat_to_base64(matrix, opts, base_64))
					{
						continue;
					}

					rapidjson::Value image(rapidjson::kObjectType);

					const std::string id =
						make_image_id(container.name(), label, index);

					set_string_member(image, "id", id, allocator);
					set_string_member(
						image,
						"label",
						label.empty() ? "Image " + std::to_string(index) : label,
						allocator);

					image.AddMember("width", matrix.cols, allocator);
					image.AddMember("height", matrix.rows, allocator);

					set_string_member(image, "data", base_64, allocator);

					if (opts.include_metrics && index < metrics_list.size())
					{
						const auto& [iterations,
							real_time,
							cpu_time,
							time_unit,
							bytes_per_second,
							items_per_second,
							execution_time_ms,
							memory_usage_mb,
							pixel_count,
							non_zero_pixels,
							compression_ratio] = metrics_list[index];

						rapidjson::Value metrics(rapidjson::kObjectType);

						add_if_positive(metrics, "iterations", iterations, allocator);
						add_if_positive(metrics, "realTime", real_time, allocator);
						add_if_positive(metrics, "cpuTime", cpu_time, allocator);
						if (!time_unit.empty() && time_unit != "ns")
						{
							set_string_member(metrics, "timeUnit", time_unit, allocator);
						}
						add_if_positive(metrics, "bytesPerSecond", bytes_per_second, allocator);
						add_if_positive(metrics, "itemsPerSecond", items_per_second, allocator);
						add_if_positive(metrics, "executionTimeMs", execution_time_ms, allocator);
						add_if_positive(metrics, "memoryUsageMB", memory_usage_mb, allocator);
						add_if_positive(metrics, "pixelCount", pixel_count, allocator);
						add_if_positive(metrics, "nonZeroPixels", non_zero_pixels, allocator);
						add_if_positive(metrics, "compressionRatio", compression_ratio, allocator);

						if (!metrics.ObjectEmpty())
						{
							image.AddMember("metrics", metrics, allocator);
						}
					}

					images.PushBack(image, allocator);
				}

				value.AddMember("images", images, allocator);
				containers.PushBack(value, allocator);
			}

			document.AddMember("containers", containers, allocator);

			return write_json_document(document, out_json_path, opts, "output JSON");
		}
		catch (const std::exception& e)
		{
			LOG(ERROR) << "Failed to write output JSON '" << out_json_path.string()
				<< "': " << e.what();
			return false;
		}
		catch (...)
		{
			LOG(ERROR) << "Unknown error while writing output JSON: " << out_json_path.string();
			return false;
		}
	}

	/**
	 * @brief Writes configuration metadata JSON for visualizer consumption.
	 *
	 * @param configs Configuration metadata collection.
	 * @param out_json_path Destination JSON path.
	 * @param opts Export behavior options.
	 * @return True when JSON was written successfully.
	 */
	bool exporter::write_configuration_json(
		const std::vector<configuration::image_benchmark_metadata>& configs,
		const std::filesystem::path& out_json_path,
		const exporter_options& opts)
	{
		if (out_json_path.empty())
		{
			return false;
		}

		try
		{
			std::filesystem::create_directories(out_json_path.parent_path());

			rapidjson::Document document;

			document.SetObject();

			auto& allocator = document.GetAllocator();

			add_timestamp_member(document, allocator);

			rapidjson::Value configurations(rapidjson::kArrayType);

			for (const auto& config : configs)
			{
				rapidjson::Value config_obj(rapidjson::kObjectType);

				set_string_member(config_obj, "name", config.name, allocator);
				set_string_member(config_obj, "path", config.path, allocator);

				rapidjson::Value variants(rapidjson::kArrayType);

				for (const auto& [type, algorithm] : config.variants)
				{
					rapidjson::Value variant_obj(rapidjson::kObjectType);
					set_string_member(variant_obj, "type", type, allocator);
					set_string_member(variant_obj, "algorithm", algorithm, allocator);
					variants.PushBack(variant_obj, allocator);
				}

				config_obj.AddMember("skeletonizers", variants, allocator);
				configurations.PushBack(config_obj, allocator);
			}

			document.AddMember("configurations", configurations, allocator);

			return write_json_document(document, out_json_path, opts, "configuration JSON");
		}
		catch (const std::exception& e)
		{
			LOG(ERROR) << "Failed to write configuration JSON '" << out_json_path.string()
				<< "': " << e.what();
			return false;
		}
		catch (...)
		{
			LOG(ERROR) << "Unknown error while writing configuration JSON: " << out_json_path.string();
			return false;
		}
	}

	/**
	 * @brief Creates timestamped output file path inside generated output directory.
	 *
	 * @param filename Output filename.
	 * @return Full output path with timestamped directory prefix.
	 */
	std::filesystem::path exporter::create_timestamped_output_path(const std::string& filename)
	{
		const auto output_dir = create_timestamped_output_directory();

		return output_dir / filename;
	}
}
