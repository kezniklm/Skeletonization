#include "bind_helpers.hpp"

import skeletonizer_cpu;

using namespace skeletonizer::cpu::algorithms;

#if SKELETONIZATION_WITH_GPU
import skeletonizer_gpu;

using namespace skeletonizer::gpu::algorithms;
#endif

PYBIND11_MODULE(skeletonization, m)
{
	m.doc() = "Pybind11 wrapper for SkeletonizationCore";

	register_skeletonizer<zhang_suen_cpu>(m, "Zhang–Suen");
	register_skeletonizer<zhang_suen_cpu_threads>(m, "Multithreaded Zhang–Suen");

	register_skeletonizer<guo_hall_cpu>(m, "Guo–Hall");
	register_skeletonizer<guo_hall_cpu_threads>(m, "Multithreaded Guo–Hall");

	register_skeletonizer<kwon_gi_kang_cpu>(m, "Kwon–Gi–Kang");
	register_skeletonizer<kwon_gi_kang_cpu_threads>(m, "Multithreaded Kwon–Gi–Kang");

	register_skeletonizer<han_la_rhee_cpu>(m, "Han-La-Rhee");
	register_skeletonizer<han_la_rhee_cpu_threads>(m, "Multithreaded Han-La-Rhee");

	register_skeletonizer<petrosino_salvi_cpu>(m, "Petrosino-Salvi");
	register_skeletonizer<petrosino_salvi_thread>(m, "Multithreaded Petrosino-Salvi");

#if SKELETONIZATION_WITH_GPU
	register_skeletonizer<zhang_suen_gpu>(m, "Parallelized Zhang–Suen");

	register_skeletonizer<guo_hall_gpu>(m, "Parallelized Guo–Hall");

	register_skeletonizer<kwon_gi_kang_gpu>(m, "Parallelized Kwon–Gi–Kang");

	register_skeletonizer<han_la_rhee_gpu>(m, "Parallelized Han-La-Rhee");

	register_skeletonizer<petrosino_salvi_gpu>(m, "Parallelized Petrosino-Salvi");
#endif
}
