#include "SkeletonizationCore/configuration/creators.hpp"

#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "SkeletonizationCore/configuration/types.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

#include "SkeletonizationCoreCPU/choi_lam_siu.hpp"
#include "SkeletonizationCoreCPU/guo_hall.hpp"
#include "SkeletonizationCoreCPU/han_la_rhee.hpp"
#include "SkeletonizationCoreCPU/k3m.hpp"
#include "SkeletonizationCoreCPU/kmm.hpp"
#include "SkeletonizationCoreCPU/kwon_gi_kang.hpp"
#include "SkeletonizationCoreCPU/liu_zhang.hpp"
#include "SkeletonizationCoreCPU/petrosino_salvi.hpp"
#include "SkeletonizationCoreCPU/tarabek.hpp"
#include "SkeletonizationCoreCPU/zhang_suen.hpp"

#include "SkeletonizationCoreMT/choi_lam_siu.hpp"
#include "SkeletonizationCoreMT/guo_hal.hpp"
#include "SkeletonizationCoreMT/han_la_rhee.hpp"
#include "SkeletonizationCoreMT/kwon_gi_kang.hpp"
#include <SkeletonizationCoreMT/liu_zhang.hpp>
#include "SkeletonizationCoreMT/petrosino_salvi.hpp"
#include "SkeletonizationCoreMT/tarabek.hpp"
#include "SkeletonizationCoreMT/zhang_suen.hpp"

#if SKELETONIZATION_WITH_GPU
#include "SkeletonizationCoreGPU/choi_lam_siu.cuh"
#include "SkeletonizationCoreGPU/guo_hall.cuh"
#include "SkeletonizationCoreGPU/han_la_rhee.cuh"
#include "SkeletonizationCoreGPU/kwon_gi_kang.cuh"
#include "SkeletonizationCoreGPU/petrosino_salvi.cuh"
#include "SkeletonizationCoreGPU/tarabek.cuh"
#include "SkeletonizationCoreGPU/zhang_suen.cuh"
#endif

namespace configuration
{
	std::vector<skeletonizer_creator> make_algorithm_creators(
		skeletonizer::skeletonizer_type skeletonizer_type,
		const std::string& algorithm)
	{
		constexpr auto entries = std::tuple{
			make_entry<
				skeletonizer::cpu::algorithms::zhang_suen,
				skeletonizer::mt::algorithms::zhang_suen
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::zhang_suen
#endif
			>("zhang_suen"),

			make_entry<
				skeletonizer::cpu::algorithms::guo_hall,
				skeletonizer::mt::algorithms::guo_hall
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::guo_hall
#endif
			>("guo_hall"),

			make_entry<
				skeletonizer::cpu::algorithms::kwon_gi_kang,
				skeletonizer::mt::algorithms::kwon_gi_kang
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::kwon_gi_kang
#endif
			>("kwon_gi_kang"),

			make_entry<
				skeletonizer::cpu::algorithms::petrosino_salvi,
				skeletonizer::mt::algorithms::petrosino_salvi
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::petrosino_salvi
#endif
			>("petrosino_salvi"),

			make_entry<
				skeletonizer::cpu::algorithms::han_la_rhee,
				skeletonizer::mt::algorithms::han_la_rhee
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::han_la_rhee
#endif
			>("han_la_rhee"),

			make_entry<
				skeletonizer::cpu::algorithms::choi_lam_siu,
				skeletonizer::mt::algorithms::choi_lam_siu
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::choi_lam_siu
#endif
			>("choi_lam_siu"),

			make_entry<
				skeletonizer::cpu::algorithms::kmm
			>("kmm"),

			make_entry<
				skeletonizer::cpu::algorithms::k3m
			>("k3m"),

			make_entry<
				skeletonizer::cpu::algorithms::tarabek,
				skeletonizer::mt::algorithms::tarabek
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::tarabek
#endif
			>("tarabek"),

			make_entry<
				skeletonizer::cpu::algorithms::liu_zhang,
				skeletonizer::mt::algorithms::liu_zhang
			>("liu_zhang"),
		};

		std::vector<skeletonizer_creator> creators;
		bool found = false;

		std::apply(
			[&](const auto&... algorithm_entry)
			{
				((
					[&]
					{
						if (!found && algorithm == algorithm_entry.name)
						{
							algorithm_entry.push_creators_for_type(
								skeletonizer_type, creators);
							found = true;
						}
					}()
				), ...);
			},
			entries);

		if (creators.empty())
		{
			throw std::runtime_error(
				"No valid creator for type=" + to_string(skeletonizer_type) +
				", algorithm=" + algorithm);
		}

		return creators;
	}
}
