#include "SkeletonizationCLI/utils/opencv_image_loader.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"
#include "SkeletonizationCLI/exceptions/export_exception.hpp"

namespace cli::utils
{
	cv::Mat opencv_image_loader::load(const std::filesystem::path& path) const
	{
		const auto result = read_image(path.string());
		
		if (!result)
		{
			throw exceptions::image_loading_exception(path, result.error());
		}
		
		return result.value();
	}
}
