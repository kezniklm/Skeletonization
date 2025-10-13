#pragma once

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "concepts.hpp"
#include "name_utils.hpp"
#include "numpy_converter.hpp"
#include "registry.hpp"
#include "type_name.hpp"

namespace py = pybind11;

template <skeletonizer_subclass Skeletonizer>
py::array_t<bool> py_run_skeletonizer(const py::array_t<bool, py::array::c_style | py::array::forcecast> numpy_image)
{
	const auto source = numpy_bool_2d_to_cv8u(numpy_image);

	auto to_skeletonize = source.clone();

	// Release GIL - Global Interpreter Lock during processing
	{
		py::gil_scoped_release release;
		Skeletonizer skeletonizer;
		skeletonizer.apply(to_skeletonize);
	}


	return cv8u_to_numpy_eq(to_skeletonize, 1u);
}

template <skeletonizer_subclass T>
void register_skeletonizer(py::module_& module, const std::string_view display = {},
                           const std::string_view description = {})
{
	const auto id = type_name<T>();

	const auto py_name = pythonize_type_name(canonical_type(id));

	module.def(py_name.c_str(), &py_run_skeletonizer<T>, py::arg("binary_image"),
	           "Run the skeletonizer on a 2D boolean NumPy array and return a boolean mask.");

	skelpy::add_algo(id, py_name, display.empty() ? display : py_name, description);
}
