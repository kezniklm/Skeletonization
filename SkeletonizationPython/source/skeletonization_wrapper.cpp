#include "bind_helpers.hpp"

import skeletonizer_cpu;

import skeletonizer_mt;

#if SKELETONIZATION_WITH_GPU
import skeletonizer_gpu;
#endif

PYBIND11_MODULE(skeletonization, m)
{
	m.doc() = "Pybind11 wrapper for SkeletonizationCore";

	register_skeletonizer<skeletonizer::cpu::algorithms::zhang_suen>(m, "Zhang–Suen");
	register_skeletonizer<skeletonizer::mt::algorithms::zhang_suen>(m, "Multithreaded Zhang–Suen");

	register_skeletonizer<skeletonizer::cpu::algorithms::guo_hall>(m, "Guo–Hall");
	register_skeletonizer<skeletonizer::mt::algorithms::guo_hall>(m, "Multithreaded Guo–Hall");

	register_skeletonizer<skeletonizer::cpu::algorithms::kwon_gi_kang>(m, "Kwon–Gi–Kang");
	register_skeletonizer<skeletonizer::mt::algorithms::kwon_gi_kang>(m, "Multithreaded Kwon–Gi–Kang");

	register_skeletonizer<skeletonizer::cpu::algorithms::han_la_rhee>(m, "Han-La-Rhee");
	register_skeletonizer<skeletonizer::mt::algorithms::han_la_rhee>(m, "Multithreaded Han-La-Rhee");

	register_skeletonizer<skeletonizer::cpu::algorithms::petrosino_salvi>(m, "Petrosino-Salvi");
	register_skeletonizer<skeletonizer::mt::algorithms::petrosino_salvi>(m, "Multithreaded Petrosino-Salvi");

#if SKELETONIZATION_WITH_GPU
	register_skeletonizer<skeletonizer::gpu::algorithms::zhang_suen>(m, "Parallelized Zhang–Suen");

	register_skeletonizer<skeletonizer::gpu::algorithms::guo_hall>(m, "Parallelized Guo–Hall");

	register_skeletonizer<skeletonizer::gpu::algorithms::kwon_gi_kang>(m, "Parallelized Kwon–Gi–Kang");

	register_skeletonizer<skeletonizer::gpu::algorithms::han_la_rhee>(m, "Parallelized Han-La-Rhee");

	register_skeletonizer<skeletonizer::gpu::algorithms::petrosino_salvi>(m, "Parallelized Petrosino-Salvi");
#endif
}
