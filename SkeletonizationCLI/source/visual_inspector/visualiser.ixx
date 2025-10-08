module;

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

#include "glog/logging.h"
#include "opencv2/opencv.hpp"
#include <drogon/drogon.h>

export module visual_inspector:visualiser;

import commandline;
import :image_container;

namespace visual_inspector
{
	static void open_in_browser(const std::string& url)
	{
#if defined(_WIN32)
		std::string cmd = "start " + url;
#elif defined(__APPLE__)
		std::string cmd = "open " + url;
#else
		std::string cmd = "xdg-open " + url;
#endif
		std::system(cmd.c_str());
	}

	class visualizer_server final : public std::enable_shared_from_this<visualizer_server>
	{
	public:
		visualizer_server(const std::filesystem::path& web_root, const uint16_t port)
			: root_(std::filesystem::is_regular_file(web_root) ? web_root.parent_path() : web_root),
			  port_(port)
		{
			if (!std::filesystem::exists(root_))
			{
				throw std::runtime_error("[visualizer_server] Root does not exist: " + root_.string());
			}

			auto& app = drogon::app();
			app.setDocumentRoot(root_.string());
			app.addListener("127.0.0.1", port_);


			app.setDefaultHandler([p = root_](const drogon::HttpRequestPtr& request,
			                                  std::function<void(const drogon::HttpResponsePtr&)>&& callback)
			{
				const auto ends_with = [](const std::string_view s, const std::string_view suffix)
				{
					return s.size() >= suffix.size() && 0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix);
				};

				if (request->getMethod() != drogon::Get)
				{
					callback(drogon::HttpResponse::newNotFoundResponse());
					return;
				}

				if (ends_with(request->path(), "benchmark_data.json"))
				{
					const auto response = drogon::HttpResponse::newFileResponse((p / "benchmark_data.json").string());
					response->setStatusCode(drogon::k200OK);
					response->setContentTypeCode(drogon::CT_APPLICATION_JSON);

					callback(response);
					return;
				}

				const auto response = drogon::HttpResponse::newFileResponse((p / "index.html").string());
				response->setStatusCode(drogon::k200OK);
				response->setContentTypeCode(drogon::CT_TEXT_HTML);

				callback(response);
			});
		}

		void start() const
		{
			const auto url = "http://127.0.0.1:" + std::to_string(port_) + "/";
#if defined(_WIN32)
			ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(__APPLE__)
			std::system(std::string("open \"" + url + "\"").c_str());
#else
			std::system(std::string("xdg-open \"" + url + "\"").c_str());
#endif
			drogon::app().run();
		}

		static void quit()
		{
			drogon::app().quit();
		}

		[[nodiscard]] std::string url() const
		{
			return "http://127.0.0.1:" + std::to_string(port_) + "/";
		}

	private:
		std::filesystem::path root_;
		uint16_t port_;
	};

	export struct image_metrics
	{
		// Google Benchmark specific fields
		int64_t iterations = 0;
		double real_time = 0.0; // nanoseconds
		double cpu_time = 0.0; // nanoseconds
		std::string time_unit = "ns"; // "ns", "us", "ms", "s"
		double bytes_per_second = 0.0;
		double items_per_second = 0.0;

		double execution_time_ms = 0.0;
		double memory_usage_mb = 0.0;
		int pixel_count = 0;
		int non_zero_pixels = 0;
		double compression_ratio = 0.0;
	};

	export class visualiser
	{
	public:
		void add_benchmark_image_container(const image_container& image_container)
		{
			benchmark_image_containers_.push_back(image_container);
			container_metrics_.push_back({});
		}

		static void show(const std::filesystem::path& web_root_or_index, const uint16_t port = 8787)
		{
			const visualizer_server server(web_root_or_index, port);

			try
			{
				LOG(INFO) << "Visualizer running at " << server.url();

				server.start();
			}
			catch (const std::exception& e)
			{
				LOG(ERROR) << "Failed to start visualizer server at: " << server.url();

				server.quit();
			}
		}

	private:
		std::vector<image_container> benchmark_image_containers_;
		std::vector<std::vector<image_metrics>> container_metrics_;
	};
}
