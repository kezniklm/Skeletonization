/**
*
* @file skeletonization_wrapper.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines Python module entry point for skeletonization bindings.
*
* This file initializes the pybind11 module and registers algorithm bindings.
*
* Main responsibilities:
* - define pybind11 module entry point
* - set module-level documentation string
* - register all algorithm bindings
*
* @version 1.0
* @date 2026-03-16
*/

#include "binders.hpp"

PYBIND11_MODULE(skeletonization, m)
{
	m.doc() = "Pybind11 wrapper for SkeletonizationCore";

	register_algorithms(m);
}
