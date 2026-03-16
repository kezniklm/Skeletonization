/**
*
* @file runner.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Provides typed algorithm runner wrapper for tests.
*
* This header defines a generic callable wrapper that normalizes input and
* output around skeletonizer execution.
*
* Main responsibilities:
* - wrap algorithm execution in a uniform callable
* - normalize input matrices before execution
* - normalize output matrices after execution
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <opencv2/core.hpp>

#include "normalize.hpp"

namespace skeltest
{
	/**
	 * @class runner
	 * @brief Executes one skeletonizer type in tests.
	 *
	 * This template runs algorithm T and normalizes matrices to binary form.
	 */
	template <class T>
	struct runner
	{
		/**
		 * @brief Runs algorithm on normalized input image.
		 *
		 * @param in Input image.
		 * @return Normalized skeleton output image.
		 */
		cv::Mat operator()(const cv::Mat& in) const
		{
			cv::Mat work = to01(in);

			T algorithm;
			algorithm.apply(work);

			return to01(work);
		}
	};
}
