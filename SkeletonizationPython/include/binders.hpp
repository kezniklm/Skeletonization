#pragma once

#include <string>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "numpy_converter.hpp"

#include "SkeletonizationCore/skeletonizer/algorithm_registry.hpp"
#include "SkeletonizationCore/configuration/concepts.hpp"

namespace py = pybind11;

template <typename Entry, typename Fn>
void for_each_variant(const Entry& entry, Fn&& fn)
{
	const std::string base(entry.name);

	if constexpr (Entry::has_cpu)
	{
		fn(std::type_identity<typename Entry::cpu_type>{}, base);
	}
	if constexpr (Entry::has_thread)
	{
		fn(std::type_identity<typename Entry::thread_type>{}, base + "_mt");
	}
#if SKELETONIZATION_WITH_GPU
	if constexpr (Entry::has_gpu)
	{
		fn(std::type_identity<typename Entry::gpu_type>{}, base + "_gpu");
	}
#endif
}

template <configuration::skeletonizer_implementation Skeletonizer>
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

template <configuration::skeletonizer_implementation T>
void register_skeletonizer(py::module_& module, const std::string& py_name)
{
	module.def(py_name.c_str(), &py_run_skeletonizer<T>, py::arg("binary_image"),
	           "Run the skeletonizer on a 2D boolean NumPy array and return a boolean mask.");
}

template <typename Entry>
void register_all_from_entry(py::module_& module, const Entry& entry)
{
	for_each_variant(entry, [&]<typename T>(std::type_identity<T>, const std::string& name)
	{
		register_skeletonizer<T>(module, name);
	});
}

inline void bind_algorithms_discovery(py::module_& module)
{
	module.def("algorithms", []
	{
		py::list out;

		configuration::for_each_algorithm([&](const auto& entry)
		{
			for_each_variant(entry, [&]<typename T>(std::type_identity<T>, const std::string& name)
			{
				py::dict dict;

				dict["name"] = name;
				dict["foreground"] = T::foreground;
				dict["skeleton"] = T::skeleton;
				dict["background"] = T::background;

				out.append(std::move(dict));
			});
		});

		return out;
	}, "Return metadata for all registered algorithms.");
}

inline void register_algorithms(py::module_& module)
{
	bind_algorithms_discovery(module);

	configuration::for_each_algorithm([&](const auto& entry)
	{
		register_all_from_entry(module, entry);
	});
}
