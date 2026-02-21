#include "binders.hpp"

PYBIND11_MODULE(skeletonization, m)
{
	m.doc() = "Pybind11 wrapper for SkeletonizationCore";

	register_algorithms(m);
}
