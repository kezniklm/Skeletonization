#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

#include <drogon/drogon.h>
#include <opencv2/opencv.hpp>
#include "glog/logging.h"

#ifdef _WIN32
#include <shellapi.h>
#include <windows.h>
#endif

#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"
#include "SkeletonizationCLI/visual_inspector/image_container.hpp"

namespace visual_inspector
{
	void open_in_browser(const std::string& url)
	{
#ifdef _WIN32
		const std::string cmd = "start " + url;
#elifdef __APPLE__
		const std::string cmd = "open " + url;
#else
		const std::string cmd = "xdg-open " + url;
#endif
		const auto return_code = std::system(cmd.c_str());

		if (return_code != 0)
		{
			LOG(ERROR) << "Failed to open browser with URL: " << url << ". Return code: " << return_code;
		}
	}

	class visualizer_server final : public std::enable_shared_from_this<visualizer_server>
	{
	public:
		visualizer_server(const std::filesystem::path& web_root, const uint16_t port)
			: root_(std::filesystem::is_regular_file(web_root)
				        ? web_root.parent_path()
				        : web_root),
			  port_(port)
		{
			if (!std::filesystem::exists(root_))
			{
				throw std::runtime_error(
					"[visualizer_server] Root does not exist: " + root_.string());
			}

			auto& app = drogon::app();
			app.setDocumentRoot(root_.string());
			app.addListener("127.0.0.1", port_);

			app.setDefaultHandler(
				[p = root_](
				const drogon::HttpRequestPtr& request,
				std::function<void(const drogon::HttpResponsePtr&)>&& callback)
				{
					const auto ends_with = [](const std::string_view s,
					                          const std::string_view suffix)
					{
						return s.size() >= suffix.size() && s.ends_with(suffix);
					};

					if (request->getMethod() != drogon::Get)
					{
						callback(drogon::HttpResponse::newNotFoundResponse());
						return;
					}

					if (ends_with(request->path(), "benchmark_data.json"))
					{
						const auto response =
							drogon::HttpResponse::newFileResponse(
								(p / "benchmark_data.json").string());
						response->setStatusCode(drogon::k200OK);
						response->setContentTypeCode(drogon::CT_APPLICATION_JSON);

						callback(response);
						return;
					}

					const auto response = drogon::HttpResponse::newFileResponse(
						(p / "index.html").string());
					response->setStatusCode(drogon::k200OK);
					response->setContentTypeCode(drogon::CT_TEXT_HTML);

					callback(response);
				});
		}

		void start() const
		{
			const auto url = "http://127.0.0.1:" + std::to_string(port_) + "/";

#ifdef _WIN32
			if (ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL) <= reinterpret_cast<HINSTANCE>(32))
			{
				LOG(ERROR) << "Failed to open browser with URL: " << url;
			}
#elifdef __APPLE__
			const auto return_code = std::system(std::string("open \"" + url + "\"").c_str());

			if (return_code != 0)
			{
				LOG(ERROR) << "Failed to open browser with URL: " << url << ". Return code: " << return_code;
			}
#else
			const auto return_code = std::system(std::string("xdg-open \"" + url + "\"").c_str());

			if (return_code != 0)
			{
				LOG(ERROR) << "Failed to open browser with URL: " << url << ". Return code: " << return_code;
			}
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

	void visualiser::add_benchmark_image_container(const image_container& image_container)
	{
		benchmark_image_containers_.push_back(image_container);
		container_metrics_.emplace_back();
	}

	void visualiser::show(const std::filesystem::path& web_root_or_index,
	                      const uint16_t port)
	{
		visualizer_server server(web_root_or_index, port);

		try
		{
			LOG(INFO) << "Visualizer running at " << server.url();
			server.start();
		}
		catch (const std::exception& e)
		{
			LOG(ERROR) << "Failed to start visualizer server at: "
				<< server.url() << " (" << e.what() << ")";
			visualizer_server::quit();
		}
	}
}
