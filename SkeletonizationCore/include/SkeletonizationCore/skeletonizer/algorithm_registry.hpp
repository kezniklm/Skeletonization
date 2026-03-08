#pragma once

#include <tuple>

#include "SkeletonizationCore/configuration/creators.hpp"

#include "SkeletonizationCoreCPU/choi_lam_siu.hpp"
#include "SkeletonizationCoreCPU/guo_hall.hpp"
#include "SkeletonizationCoreCPU/han_la_rhee.hpp"
#include "SkeletonizationCoreCPU/k3m.hpp"
#include "SkeletonizationCoreCPU/kmm.hpp"
#include "SkeletonizationCoreCPU/kwon_kang.hpp"
#include "SkeletonizationCoreCPU/lantuejoul.hpp"
#include "SkeletonizationCoreCPU/liu_zhang.hpp"
#include "SkeletonizationCoreCPU/petrosino_salvi.hpp"
#include "SkeletonizationCoreCPU/tarabek.hpp"
#include "SkeletonizationCoreCPU/zhang_suen.hpp"

#include "SkeletonizationCoreMT/choi_lam_siu.hpp"
#include "SkeletonizationCoreMT/guo_hal.hpp"
#include "SkeletonizationCoreMT/han_la_rhee.hpp"
#include "SkeletonizationCoreMT/kwon_kang.hpp"
#include "SkeletonizationCoreMT/liu_zhang.hpp"
#include "SkeletonizationCoreMT/petrosino_salvi.hpp"
#include "SkeletonizationCoreMT/tarabek.hpp"
#include "SkeletonizationCoreMT/zhang_suen.hpp"

#if SKELETONIZATION_WITH_GPU
#include "SkeletonizationCoreGPU/choi_lam_siu.cuh"
#include "SkeletonizationCoreGPU/guo_hall.cuh"
#include "SkeletonizationCoreGPU/han_la_rhee.cuh"
#include "SkeletonizationCoreGPU/kwon_kang.cuh"
#include "SkeletonizationCoreGPU/liu_zhang.cuh"
#include "SkeletonizationCoreGPU/petrosino_salvi.cuh"
#include "SkeletonizationCoreGPU/tarabek.cuh"
#include "SkeletonizationCoreGPU/zhang_suen.cuh"
#endif

namespace configuration
{
	inline constexpr auto all_algorithms = std::tuple{
		make_entry<
			skeletonizer::cpu::algorithms::zhang_suen,
			skeletonizer::mt::algorithms::zhang_suen
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::zhang_suen
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::guo_hall,
			skeletonizer::mt::algorithms::guo_hall
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::guo_hall
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::kwon_kang,
			skeletonizer::mt::algorithms::kwon_kang
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::kwon_gi_kang
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::petrosino_salvi,
			skeletonizer::mt::algorithms::petrosino_salvi
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::petrosino_salvi
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::han_la_rhee,
			skeletonizer::mt::algorithms::han_la_rhee
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::han_la_rhee
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::choi_lam_siu,
			skeletonizer::mt::algorithms::choi_lam_siu
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::choi_lam_siu
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::tarabek,
			skeletonizer::mt::algorithms::tarabek
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::tarabek
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::liu_zhang,
			skeletonizer::mt::algorithms::liu_zhang
#if SKELETONIZATION_WITH_GPU
			, skeletonizer::gpu::algorithms::liu_zhang
#endif
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::kmm
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::k3m
		>(),

		make_entry<
			skeletonizer::cpu::algorithms::lantuejoul
		>(),
	};

	template <typename Callable>
	constexpr void for_each_algorithm(Callable&& fn)
	{
		std::apply(
			[&](const auto&... entry)
			{
				(fn(entry), ...);
			},
			all_algorithms);
	}
}
