#pragma once

#include <opencv2/core.hpp>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

inline cv::Mat numpy_bool_2d_to_cv8u(const py::array_t<bool, py::array::c_style | py::array::forcecast>& a)
{
	if (a.ndim() != 2)
	{
		throw std::runtime_error("binary_image must be 2D");
	}

	return cv::Mat(static_cast<int>(a.shape(0)), static_cast<int>(a.shape(1)), CV_8UC1, const_cast<bool*>(a.data()));
}

inline py::array_t<bool> cv8u_to_numpy_eq(const cv::Mat& matrix, const uint8_t eq)
{
	if (matrix.type() != CV_8UC1)
	{
		throw std::runtime_error("expected CV_8UC1");
	}

	py::array_t<bool> output({matrix.rows, matrix.cols});

	auto* dst = output.mutable_data();

	const auto* src = matrix.ptr<uint8_t>(0);

	for (auto index = 0, n = matrix.rows * matrix.cols; index < n; ++index)
	{
		dst[index] = src[index] == eq;
	}
	return output;
}
