#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "runner.hpp"

#include "SkeletonizationCoreCPU/choi_lam_siu.hpp"
#include "SkeletonizationCoreCPU/guo_hall.hpp"
#include "SkeletonizationCoreCPU/han_la_rhee.hpp"
#include "SkeletonizationCoreCPU/kwon_gi_kang.hpp"
#include "SkeletonizationCoreCPU/liu_zhang.hpp"
#include "SkeletonizationCoreCPU/petrosino_salvi.hpp"
#include "SkeletonizationCoreCPU/tarabek.hpp"
#include "SkeletonizationCoreCPU/zhang_suen.hpp"

#include "SkeletonizationCoreMT/choi_lam_siu.hpp"
#include "SkeletonizationCoreMT/guo_hal.hpp"
#include "SkeletonizationCoreMT/han_la_rhee.hpp"
#include "SkeletonizationCoreMT/kwon_gi_kang.hpp"
#include "SkeletonizationCoreMT/liu_zhang.hpp"
#include "SkeletonizationCoreMT/petrosino_salvi.hpp"
#include "SkeletonizationCoreMT/tarabek.hpp"
#include "SkeletonizationCoreMT/zhang_suen.hpp"

#if SKELETONIZATION_WITH_GPU
#include "SkeletonizationCoreGPU/choi_lam_siu.cuh"
#include "SkeletonizationCoreGPU/guo_hall.cuh"
#include "SkeletonizationCoreGPU/han_la_rhee.cuh"
#include "SkeletonizationCoreGPU/kwon_gi_kang.cuh"
#include "SkeletonizationCoreGPU/liu_zhang.cuh"
#include "SkeletonizationCoreGPU/petrosino_salvi.cuh"
#include "SkeletonizationCoreGPU/tarabek.cuh"
#include "SkeletonizationCoreGPU/zhang_suen.cuh"
#endif

namespace skeltest
{
	struct triple
	{
		std::string name;
		std::function<cv::Mat(const cv::Mat&)> cpu;
		std::function<cv::Mat(const cv::Mat&)> mt;
#if SKELETONIZATION_WITH_GPU
		std::optional<std::function<cv::Mat(const cv::Mat&)>> gpu;
#endif
	};

	template <class CpuT, class MtT
#if SKELETONIZATION_WITH_GPU
	          , class GpuT
#endif
	>
	triple make_triple(std::string name)
	{
		runner<CpuT> rcpu;
		runner<MtT> rmt;
#if SKELETONIZATION_WITH_GPU
		runner<GpuT> rgpu;

		return triple{
			std::move(name),
			[rcpu](const cv::Mat& m) { return rcpu(m); },
			[rmt](const cv::Mat& m) { return rmt(m); },
			std::optional{
				std::function<cv::Mat(const cv::Mat&)>{[rgpu](const cv::Mat& m) { return rgpu(m); }}
			}
		};
#else
		return triple{
			std::move(name),
			[rcpu](const cv::Mat& m) { return rcpu(m); },
			[rmt](const cv::Mat& m) { return rmt(m); }
		};
#endif
	}

	inline const std::vector<triple>& triples()
	{
		using CLS_CPU = skeletonizer::cpu::algorithms::choi_lam_siu;
		using GH_CPU = skeletonizer::cpu::algorithms::guo_hall;
		using HLR_CPU = skeletonizer::cpu::algorithms::han_la_rhee;
		using KGK_CPU = skeletonizer::cpu::algorithms::kwon_gi_kang;
		using LZ_CPU = skeletonizer::cpu::algorithms::liu_zhang;
		using PS_CPU = skeletonizer::cpu::algorithms::petrosino_salvi;
		using TA_CPU = skeletonizer::cpu::algorithms::tarabek;
		using ZS_CPU = skeletonizer::cpu::algorithms::zhang_suen;

		using CLS_MT = skeletonizer::mt::algorithms::choi_lam_siu;
		using GH_MT = skeletonizer::mt::algorithms::guo_hall;
		using HLR_MT = skeletonizer::mt::algorithms::han_la_rhee;
		using KGK_MT = skeletonizer::mt::algorithms::kwon_gi_kang;
		using LZ_MT = skeletonizer::mt::algorithms::liu_zhang;
		using PS_MT = skeletonizer::mt::algorithms::petrosino_salvi;
		using TA_MT = skeletonizer::mt::algorithms::tarabek;
		using ZS_MT = skeletonizer::mt::algorithms::zhang_suen;

#if SKELETONIZATION_WITH_GPU
		using CLS_GPU = skeletonizer::gpu::algorithms::choi_lam_siu;
		using GH_GPU = skeletonizer::gpu::algorithms::guo_hall;
		using HLR_GPU = skeletonizer::gpu::algorithms::han_la_rhee;
		using KGK_GPU = skeletonizer::gpu::algorithms::kwon_gi_kang;
		using LZ_GPU = skeletonizer::gpu::algorithms::liu_zhang;
		using PS_GPU = skeletonizer::gpu::algorithms::petrosino_salvi;
		using TA_GPU = skeletonizer::gpu::algorithms::tarabek;
		using ZS_GPU = skeletonizer::gpu::algorithms::zhang_suen;
#endif

		static const std::vector<triple> v = []
		{
			std::vector<triple> triples_vec;

#if SKELETONIZATION_WITH_GPU
			triples_vec.emplace_back(make_triple<CLS_CPU, CLS_MT, CLS_GPU>("ChoiLamSiu"));
			triples_vec.emplace_back(make_triple<GH_CPU, GH_MT, GH_GPU>("GuoHall"));
			triples_vec.emplace_back(make_triple<HLR_CPU, HLR_MT, HLR_GPU>("HanLaRhee"));
			triples_vec.emplace_back(make_triple<KGK_CPU, KGK_MT, KGK_GPU>("KwonGiKang"));
			triples_vec.emplace_back(make_triple<LZ_CPU, LZ_MT, LZ_GPU>("LiuZhang"));
			triples_vec.emplace_back(make_triple<PS_CPU, PS_MT, PS_GPU>("PetrosinoSalvi"));
			triples_vec.emplace_back(make_triple<TA_CPU, TA_MT, TA_GPU>("Tarabek"));
			triples_vec.emplace_back(make_triple<ZS_CPU, ZS_MT, ZS_GPU>("ZhangSuen"));
#else
			triples_vec.emplace_back(make_triple<CLS_CPU, CLS_MT>("ChoiLamSiu"));
			triples_vec.emplace_back(make_triple<GH_CPU, GH_MT>("GuoHall"));
			triples_vec.emplace_back(make_triple<HLR_CPU, HLR_MT>("HanLaRhee"));
			triples_vec.emplace_back(make_triple<KGK_CPU, KGK_MT>("KwonGiKang"));
			triples_vec.emplace_back(make_triple<LZ_CPU, LZ_MT>("LiuZhang"));
			triples_vec.emplace_back(make_triple<PS_CPU, PS_MT>("PetrosinoSalvi"));
			triples_vec.emplace_back(make_triple<TA_CPU, TA_MT>("Tarabek"));
			triples_vec.emplace_back(make_triple<ZS_CPU, ZS_MT>("ZhangSuen"));
#endif

			return triples_vec;
		}();

		return v;
	}
}
