#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <drogon/drogon.h>
#include <opencv2/opencv.hpp>
#include "glog/logging.h"

#include "SkeletonizationCLI/exceptions/visualization_exception.hpp"
#include "SkeletonizationCLI/interfaces/i_browser_launcher.hpp"
#include "SkeletonizationCLI/utils/system_browser_launcher.hpp"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"
#include "SkeletonizationCLI/visual_inspector/image_container.hpp"

namespace visual_inspector
{
	class visualizer_server final : public std::enable_shared_from_this<visualizer_server>
	{
	public:
		visualizer_server(const std::filesystem::path& web_root,
		                  const uint16_t port,
		                  std::unique_ptr<cli::interfaces::i_browser_launcher> browser_launcher)
			: root_(std::filesystem::is_regular_file(web_root)
				        ? web_root.parent_path()
				        : web_root)
			  , port_(port)
			  , browser_launcher_(std::move(browser_launcher))
		{
			if (!std::filesystem::exists(root_))
			{
				throw cli::exceptions::web_root_not_found_exception(root_);
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

			// Use the injected browser launcher or create a default one
			if (browser_launcher_)
			{
				const bool opened = browser_launcher_->open(url);
				if (!opened)
				{
					LOG(WARNING) << "Failed to open browser automatically. Please navigate to " << url;
				}
			}
			else
			{
				const bool opened = cli::utils::system_browser_launcher{}.open(url);
				if (!opened)
				{
					LOG(WARNING) << "Failed to open browser automatically. Please navigate to " << url;
				}
			}

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
		std::unique_ptr<cli::interfaces::i_browser_launcher> browser_launcher_;
	};

	void visualiser::add_benchmark_image_container(const image_container& image_container)
	{
		benchmark_image_containers_.push_back(image_container);
		container_metrics_.emplace_back();
	}

	void visualiser::show(const std::filesystem::path& web_root_or_index,
	                      const std::uint16_t port)
	{
		// Use default browser launcher
		auto browser_launcher = std::make_unique<cli::utils::system_browser_launcher>();
		visualizer_server server(web_root_or_index, port, std::move(browser_launcher));

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
