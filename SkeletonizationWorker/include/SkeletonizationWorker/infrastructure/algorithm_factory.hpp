#pragma once

#include <algorithm>
#include <memory>
#include <ranges>
#include <string>

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

#if SKELETONIZATION_WITH_GPU
#include "SkeletonizationCoreGPU/choi_lam_siu.cuh"
#include "SkeletonizationCoreGPU/guo_hall.cuh"
#include "SkeletonizationCoreGPU/han_la_rhee.cuh"
#include "SkeletonizationCoreGPU/kwon_gi_kang.cuh"
#include "SkeletonizationCoreGPU/petrosino_salvi.cuh"
#include "SkeletonizationCoreGPU/tarabek.cuh"
#include "SkeletonizationCoreGPU/zhang_suen.cuh"
#endif

namespace worker::infrastructure
{
	class algorithm_factory
	{
	public:
		explicit algorithm_factory(const skeletonizer::skeletonizer_type processor_type) : processor_type_(processor_type)
		{
		}

		std::unique_ptr<skeletonizer::skeletonizer<>> create(const std::string& algorithm_name) const
		{
			std::string normalized_name = algorithm_name;

			std::ranges::transform(normalized_name, normalized_name.begin(), [](const unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			});
			std::ranges::replace(normalized_name, ' ', '_');
			std::ranges::replace(normalized_name, '-', '_');

			if (normalized_name == "zhang_suen")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::zhang_suen
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::zhang_suen
#endif
				>();
			}

			if (normalized_name == "guo_hall")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::guo_hall
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::guo_hall
#endif
				>();
			}

			if (normalized_name == "k3m")
			{
				return create_algorithm<skeletonizer::cpu::algorithms::k3m>();
			}

			if (normalized_name == "kmm")
			{
				return create_algorithm<skeletonizer::cpu::algorithms::kmm>();
			}

			if (normalized_name == "choi_lam_siu")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::choi_lam_siu
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::choi_lam_siu
#endif
				>();
			}

			if (normalized_name == "han_la_rhee")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::han_la_rhee
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::han_la_rhee
#endif
				>();
			}

			if (normalized_name == "kwon_kang" || normalized_name == "kwon_gi_kang")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::kwon_gi_kang
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::kwon_gi_kang
#endif
				>();
			}

			if (normalized_name == "petrosino_salvi")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::petrosino_salvi
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::petrosino_salvi
#endif
				>();
			}

			if (normalized_name == "tarabek")
			{
				return create_algorithm<
					skeletonizer::cpu::algorithms::tarabek
#if SKELETONIZATION_WITH_GPU
					, skeletonizer::gpu::algorithms::tarabek
#endif
				>();
			}

			if (normalized_name == "liu_zhang")
			{
				return create_algorithm<skeletonizer::cpu::algorithms::liu_zhang>();
			}

			return nullptr;
		}

	private:
		skeletonizer::skeletonizer_type processor_type_;

		template <typename CpuAlgorithm
#if SKELETONIZATION_WITH_GPU
		          , typename GpuAlgorithm = void
#endif
		>
		std::unique_ptr<skeletonizer::skeletonizer<>> create_algorithm() const
		{
#if SKELETONIZATION_WITH_GPU
			if constexpr (!std::is_void_v<GpuAlgorithm>)
			{
				if (processor_type_ == skeletonizer::skeletonizer_type::gpu)
				{
					return std::make_unique<GpuAlgorithm>();
				}
			}
#endif
			return std::make_unique<CpuAlgorithm>();
		}
	};
}
