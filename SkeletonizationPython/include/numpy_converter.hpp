/**
*
* @file numpy_converter.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines converters between NumPy arrays and OpenCV matrices.
*
* This header provides conversion helpers used by Python bindings to move
* binary image data between pybind11 and C++ APIs.
*
* Main responsibilities:
* - convert numpy boolean arrays to CV_8UC1 matrices
* - convert CV_8UC1 matrices to numpy boolean arrays
* - validate expected matrix and array constraints
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <opencv2/core.hpp>
#include <pybind11/numpy.h>

namespace py = pybind11;

/**
 * @brief Converts a 2D numpy boolean array to CV_8UC1 matrix view.
 *
 * @param a Input numpy array.
 * @return OpenCV matrix referencing numpy data.
 */
inline cv::Mat numpy_bool_2d_to_cv8u(const py::array_t<bool, py::array::c_style | py::array::forcecast>& a)
{
	CV_Assert(a.ndim() == 2);

	return cv::Mat(static_cast<int>(a.shape(0)), static_cast<int>(a.shape(1)), CV_8UC1, const_cast<bool*>(a.data()));
}

/**
 * @brief Converts CV_8UC1 matrix to numpy boolean array by equality test.
 *
 * @param matrix Input matrix.
 * @param eq Value compared for true outputs.
 * @return Numpy boolean array.
 */
inline py::array_t<bool> cv8u_to_numpy_eq(const cv::Mat& matrix, const uint8_t eq)
{
	CV_Assert(matrix.type() == CV_8UC1);

	py::array_t<bool> output({matrix.rows, matrix.cols});

	const auto dst = output.mutable_data();

	const auto src = matrix.ptr<uint8_t>(0);

	for (auto index = 0, n = matrix.rows * matrix.cols; index < n; ++index)
	{
		dst[index] = src[index] == eq;
	}
	return output;
}
