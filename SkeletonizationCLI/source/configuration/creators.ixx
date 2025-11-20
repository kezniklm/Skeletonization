module;

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

export module configuration:creators;

import :types;
import :creators_common;

import skeletonizer;

import skeletonizer_cpu;

#if SKELETONIZATION_WITH_GPU
import skeletonizer_gpu;
#endif

namespace configuration
{
	inline std::vector<skeletonizer_creator> make_algorithm_creators(
		skeletonizer::skeletonizer_type skeletonizer_type,
		const std::string& algorithm)
	{
		constexpr auto entries = std::tuple{
			make_entry<
				skeletonizer::cpu::algorithms::zhang_suen_cpu,
				skeletonizer::cpu::algorithms::zhang_suen_cpu_threads
#if SKELETONIZATION_WITH_GPU
				,skeletonizer::gpu::algorithms::zhang_suen_gpu
#endif
			>("zhang_suen"),

			make_entry<
				skeletonizer::cpu::algorithms::guo_hall_cpu,
				skeletonizer::cpu::algorithms::guo_hall_cpu_threads
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::guo_hall_gpu
#endif
			>("guo_hall"),

			make_entry<
				skeletonizer::cpu::algorithms::kwon_gi_kang_cpu,
				skeletonizer::cpu::algorithms::kwon_gi_kang_cpu_threads
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::kwon_gi_kang_gpu
#endif
			>("kwon_gi_kang"),

			make_entry<
				skeletonizer::cpu::algorithms::petrosino_salvi_cpu,
				skeletonizer::cpu::algorithms::petrosino_salvi_thread
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::petrosino_salvi_gpu
#endif
			>("petrosino_salvi"),

			make_entry<
				skeletonizer::cpu::algorithms::han_la_rhee_cpu,
				skeletonizer::cpu::algorithms::han_la_rhee_cpu_threads
#if SKELETONIZATION_WITH_GPU
				, skeletonizer::gpu::algorithms::han_la_rhee_gpu
#endif
			>("han_la_rhee"),

			make_entry<
				skeletonizer::cpu::algorithms::choi_lam_siu_cpu,
				skeletonizer::cpu::algorithms::choi_lam_siu_threads
#if SKELETONIZATION_WITH_GPU
				skeletonizer::gpu::algorithms::choi_lam_siu_gpu
#endif
			>("choi_lam_siu"),

			make_entry<
				skeletonizer::cpu::algorithms::kmm_cpu
			>("kmm"),

			make_entry<
				skeletonizer::cpu::algorithms::k3m_cpu
			>("k3m"),

			make_entry<
				skeletonizer::cpu::algorithms::tarabek_cpu,
				skeletonizer::cpu::algorithms::tarabek_threads
#if SKELETONIZATION_WITH_GPU
				,skeletonizer::gpu::algorithms::tarabek_gpu
#endif
			>("tarabek"),

			make_entry<
				skeletonizer::cpu::algorithms::liu_zhang_cpu,
				skeletonizer::cpu::algorithms::liu_zhang_threads
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
							algorithm_entry.push_creators_for_type(skeletonizer_type, creators);
							found = true;
						}
					}()
				), ...);
			},
			entries);

		if (creators.empty())
		{
			throw std::runtime_error(
				"No valid creator for type=" + to_string(skeletonizer_type) + ", algorithm=" + algorithm);
		}

		return creators;
	}
}
