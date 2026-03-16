/**
*
* @file image_container.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares visual inspector image container model.
*
* This file defines image container model used by visual inspection output.
*
* Main responsibilities:
* - store named image collections
* - associate labels with stored images
* - provide indexed access to container data
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

namespace visual_inspector
{
	/**
	 * @class image_container
	 * @brief Stores named image collection and labels for visualization.
	 */
	class image_container
	{
	public:
		/**
		 * @brief Constructs image container with name.
		 *
		 * @param name Container display name.
		 */
		explicit image_container(std::string name);

		/**
		 * @brief Adds image with default label.
		 *
		 * @param image Image matrix.
		 */
		void add_image(const cv::Mat& image);
		/**
		 * @brief Adds image with explicit label.
		 *
		 * @param image Image matrix.
		 * @param label Image label.
		 */
		void add_image(const cv::Mat& image, const std::string& label);

		/**
		 * @brief Returns number of stored images.
		 *
		 * @return Stored image count.
		 */
		std::size_t size() const noexcept;

		/**
		 * @brief Returns image by index.
		 *
		 * @param index Image index.
		 * @return Stored image reference.
		 */
		const cv::Mat& image(std::size_t index) const;
		/**
		 * @brief Returns label by index.
		 *
		 * @param index Label index.
		 * @return Stored label reference.
		 */
		const std::string& label(std::size_t index) const;
		/**
		 * @brief Returns container name.
		 *
		 * @return Container name.
		 */
		const std::string& name() const noexcept;

	private:
		/// Container display name.
		std::string name_;
		/// Stored images.
		std::vector<cv::Mat> images_;
		/// Stored image labels.
		std::vector<std::string> labels_;
	};
}
