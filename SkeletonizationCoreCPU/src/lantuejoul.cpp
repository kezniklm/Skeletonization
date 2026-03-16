/**
*
* @file lantuejoul.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements the CPU lantuejoul skeletonization algorithm.
*
* This file applies iterative morphological erosion and opening differences to
* accumulate a skeleton representation.
*
* Main responsibilities:
* - perform iterative erosions and openings
* - accumulate skeleton subsets across iterations
* - write the final morphological skeleton to output image
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationCoreCPU/lantuejoul.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @brief Applies lantuejoul morphological skeletonization.
	 *
	 * @param binary_image Binary image modified in place.
	 */
	void lantuejoul::apply(cv::Mat& binary_image) const
	{
		CV_Assert(binary_image.type() == CV_8UC1);

		const cv::Mat B = cv::getStructuringElement(
			cv::MORPH_ELLIPSE,
			cv::Size(3, 3)
		);

		cv::Mat skelet = cv::Mat::zeros(binary_image.size(), CV_8UC1);
		cv::Mat eroded = binary_image.clone();
		cv::Mat tmp, opened, subset;

		while (has_changed(eroded))
		{
			// opened = (X ? nB) ° B
			cv::erode(eroded, tmp, B, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, 0);
			cv::dilate(tmp, opened, B, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, 0);

			// subset = eroded \ opened  (set difference)
			subset = eroded.clone();
			subset.setTo(0, opened); // where opened==1 => remove

			// skelet = skelet ? subset
			cv::bitwise_or(skelet, subset, skelet);

			// eroded = eroded ? B
			cv::erode(eroded, eroded, B, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, 0);
		}

		skelet.copyTo(binary_image);
	}
}
