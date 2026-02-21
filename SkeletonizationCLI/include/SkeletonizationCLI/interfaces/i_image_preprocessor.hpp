#pragma once

#include <opencv2/core.hpp>

namespace cli::interfaces
{
	/**
	 * @brief Interface for preprocessing images.
	 *
	 * Abstracts image preprocessing operations (e.g., binarization,
	 * normalization) to enable dependency injection and testability.
	 */
	class i_image_preprocessor
	{
	public:
		virtual ~i_image_preprocessor() = default;

		/**
		 * @brief Preprocess an image.
		 * @param image Input image to preprocess.
		 * @return Preprocessed image.
		 */
		[[nodiscard]] virtual cv::Mat preprocess(const cv::Mat& image) const = 0;

	protected:
		i_image_preprocessor() = default;
		i_image_preprocessor(const i_image_preprocessor&) = default;
		i_image_preprocessor& operator=(const i_image_preprocessor&) = default;
		i_image_preprocessor(i_image_preprocessor&&) = default;
		i_image_preprocessor& operator=(i_image_preprocessor&&) = default;
	};
}
