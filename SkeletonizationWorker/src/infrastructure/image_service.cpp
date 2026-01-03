#include <expected>
#include <glog/logging.h>

#include "opencv2/core/mat.hpp"

#include "SkeletonizationWorker/infrastructure/image_service.hpp"

namespace worker::infrastructure
{
	std::expected<cv::Mat, std::string> image_service::load_image(const std::string& path)
	{
		try
		{
			auto image = cv::imread(path);

			if (image.empty())
			{
				return std::unexpected("Failed to load image: " + path);
			}

			LOG(INFO) << "Loaded image: " << path << " (" << image.cols << "x" << image.rows << ")";

			return image;
		}
		catch (const std::exception& e)
		{
			return std::unexpected(std::string("Exception loading image: ") + e.what());
		}
	}

	std::expected<void, std::string> image_service::save_image(const cv::Mat& image, const std::string& path)
	{
		try
		{
			if (image.empty())
			{
				return std::unexpected("Cannot save empty image");
			}

			if (!cv::imwrite(path, image))
			{
				return std::unexpected("Failed to write image: " + path);
			}

			LOG(INFO) << "Saved image: " << path << " (" << image.cols << "x" << image.rows << ")";

			return {};
		}
		catch (const std::exception& e)
		{
			return std::unexpected(std::string("Exception saving image: ") + e.what());
		}
	}
}
