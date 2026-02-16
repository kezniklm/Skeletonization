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

#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "SkeletonizationCLI/benchmark/aggregator.hpp"

namespace skeletonization_benchmark
{
	namespace
	{
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

		void set_string_member(rapidjson::Value& obj,
		                       const char* key,
		                       const std::string& s,
		                       rapidjson_allocator& alloc)
		{
			rapidjson::Value value;
			value.SetString(s.data(), static_cast<rapidjson::SizeType>(s.size()), alloc);
			obj.AddMember(rapidjson::Value(key, alloc), value, alloc);
		}

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

		bool base64_encode(const std::vector<uchar>& in, std::string& out)
		{
			static constexpr char tbl[] =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

			const std::size_t n = in.size();

			if (n == 0)
			{
				out.clear();
				return true;
			}

			const std::size_t out_len = 4 * ((n + 2) / 3);

			out.clear();
			out.reserve(out_len);

			std::size_t i = 0;

			while (i + 3 <= n)
			{
				const uint32_t b0 = in[i++];
				const uint32_t b1 = in[i++];
				const uint32_t b2 = in[i++];

				out.push_back(tbl[(b0 & 0xfc) >> 2]);
				out.push_back(tbl[((b0 & 0x03) << 4) | ((b1 & 0xf0) >> 4)]);
				out.push_back(tbl[((b1 & 0x0f) << 2) | ((b2 & 0xc0) >> 6)]);
				out.push_back(tbl[b2 & 0x3f]);
			}

			const std::size_t rem = n - i;
			if (rem == 1)
			{
				const uint32_t b0 = in[i];
				out.push_back(tbl[(b0 & 0xfc) >> 2]);
				out.push_back(tbl[(b0 & 0x03) << 4]);
				out.push_back('=');
				out.push_back('=');
			}
			else if (rem == 2)
			{
				const uint32_t b0 = in[i];
				const uint32_t b1 = in[i + 1];
				out.push_back(tbl[(b0 & 0xfc) >> 2]);
				out.push_back(tbl[((b0 & 0x03) << 4) | ((b1 & 0xf0) >> 4)]);
				out.push_back(tbl[(b1 & 0x0f) << 2]);
				out.push_back('=');
			}

			return true;
		}

		bool equal_icase(std::string_view a, std::string_view b)
		{
			if (a.size() != b.size())
			{
				return false;
			}

			for (std::size_t index = 0; index < a.size(); ++index)
			{
				if (std::tolower(static_cast<unsigned char>(a[index])) !=
					std::tolower(static_cast<unsigned char>(b[index])))
				{
					return false;
				}
			}

			return true;
		}

		bool encode_mat_to_base64(const cv::Mat& matrix,
		                          const exporter_options& opts,
		                          std::string& out_b64)
		{
			std::vector<int> params;

			if (equal_icase(opts.image_ext, ".png"))
			{
				params.push_back(cv::IMWRITE_PNG_COMPRESSION);
				params.push_back(std::clamp(opts.png_compression, 0, 9));
			}
			else if (equal_icase(opts.image_ext, ".jpg") ||
				equal_icase(opts.image_ext, ".jpeg"))
			{
				params.push_back(cv::IMWRITE_JPEG_QUALITY);
				params.push_back(std::clamp(opts.jpeg_quality, 1, 100));
			}

			std::vector<uchar> encoded;
			if (!cv::imencode(opts.image_ext, matrix, encoded, params))
			{
				return false;
			}

			return base64_encode(encoded, out_b64);
		}

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
	}

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

			if (opts.atomic_write)
			{
				const auto tmp = out_json_path.string() + ".tmp";
				{
					std::ofstream ofstream(tmp, std::ios::binary | std::ios::trunc);

					if (!ofstream)
					{
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
				std::filesystem::rename(tmp, out_json_path, error_code);

				if (error_code)
				{
					return false;
				}
			}
			else
			{
				std::ofstream ofstream(out_json_path,
				                       std::ios::binary | std::ios::trunc);

				if (!ofstream)
				{
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
		catch (...)
		{
			return false;
		}
	}

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

			if (opts.atomic_write)
			{
				const auto tmp = out_json_path.string() + ".tmp";
				{
					std::ofstream ofstream(tmp, std::ios::binary | std::ios::trunc);

					if (!ofstream)
					{
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
				std::filesystem::rename(tmp, out_json_path, error_code);

				if (error_code)
				{
					return false;
				}
			}
			else
			{
				std::ofstream ofstream(out_json_path,
									   std::ios::binary | std::ios::trunc);

				if (!ofstream)
				{
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
		catch (...)
		{
			return false;
		}
	}

	std::filesystem::path exporter::create_timestamped_output_path(const std::string& filename)
	{
		const auto output_dir = create_timestamped_output_directory();

		return output_dir / filename;
	}
}
