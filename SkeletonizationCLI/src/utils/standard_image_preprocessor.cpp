#include "SkeletonizationCLI/utils/standard_image_preprocessor.hpp"

#include "SkeletonizationCore/extensions/image_processing.hpp"

namespace cli::utils
{
	cv::Mat standard_image_preprocessor::preprocess(const cv::Mat& image) const
	{
		return preprocess_image(image);
	}
}
