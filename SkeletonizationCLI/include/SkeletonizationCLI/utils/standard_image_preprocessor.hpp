#pragma once

#include "SkeletonizationCLI/interfaces/i_image_preprocessor.hpp"

namespace cli::utils
{
	/**
	 * @brief Default implementation of i_image_preprocessor.
	 *
	 * Uses standard image preprocessing operations including
	 * binarization and normalization.
	 */
	class standard_image_preprocessor final : public interfaces::i_image_preprocessor
	{
	public:
		[[nodiscard]] cv::Mat preprocess(const cv::Mat& image) const override;
	};
}
