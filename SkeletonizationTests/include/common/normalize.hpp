/**
*
* @file normalize.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Provides normalization helpers for binary test matrices.
*
* This header converts matrices to single-channel binary {0,1} form for
* robust test comparisons.
*
* Main responsibilities:
* - convert matrices to CV_8U format
* - normalize values to binary range {0,1}
* - provide reusable preprocessing for tests
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace skeltest
{
	/**
	 * @brief Converts matrix to binary {0,1} single-channel representation.
	 *
	 * @param in Input matrix.
	 * @return Normalized binary matrix.
	 */
	inline cv::Mat to01(const cv::Mat& in)
	{
		CV_Assert(in.channels() == 1);

		cv::Mat u8;

		if (in.type() == CV_8U)
		{
			u8 = in;
		}
		else
		{
			in.convertTo(u8, CV_8U);
		}

		cv::Mat out;
		cv::threshold(u8, out, 0, 1, cv::THRESH_BINARY);

		return out;
	}
}
