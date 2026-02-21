#include "SkeletonizationCLI/utils/opencv_image_loader.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"

namespace cli::utils
{
	cv::Mat opencv_image_loader::load(const std::filesystem::path& path) const
	{
		return read_image(path.string());
	}
}
