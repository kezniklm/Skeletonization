module;

#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <opencv2/core.hpp>

export module tests_common:triples;

import :runner;

import skeletonizer_cpu;

#if defined(SKELETONIZATION_WITH_GPU)
import skeletonizer_gpu;
#endif

export namespace skeltest
{
	struct triple
	{
		std::string name;
		std::function<cv::Mat(const cv::Mat&)> cpu;
		std::function<cv::Mat(const cv::Mat&)> mt;
#if defined(SKELETONIZATION_ENABLE_GPU) || defined(SKELETONIZATION_WITH_GPU)
		std::optional<std::function<cv::Mat(const cv::Mat&)>> gpu;
#endif
	};

	template <class CpuT, class MtT
#if defined(SKELETONIZATION_ENABLE_GPU) || defined(SKELETONIZATION_WITH_GPU)
	          , class GpuT
#endif
	>
	inline triple make_triple(std::string name)
	{
		runner<CpuT> rcpu;
		runner<MtT> rmt;
#if defined(SKELETONIZATION_ENABLE_GPU) || defined(SKELETONIZATION_WITH_GPU)
		runner<GpuT> rgpu;
		return triple{
			std::move(name),
			[rcpu](const cv::Mat& m) { return rcpu(m); },
			[rmt](const cv::Mat& m) { return rmt(m); },
			std::optional{std::function<cv::Mat(const cv::Mat&)>{[rgpu](const cv::Mat& m) { return rgpu(m); }}}
		};
#else
		return Triple{
			std::move(name),
			[rcpu](const cv::Mat& m) { return rcpu(m); },
			[rmt](const cv::Mat& m) { return rmt(m); }
		};
#endif
	}

	export inline const std::vector<triple>& triples()
	{
		using namespace skeletonizer::cpu::algorithms;

		using CLS_CPU = choi_lam_siu_cpu;
		using CLS_MT = choi_lam_siu_threads;
		using GH_CPU = guo_hall_cpu;
		using GH_MT = guo_hall_cpu_threads;
		using HLR_CPU = han_la_rhee_cpu;
		using HLR_MT = han_la_rhee_cpu_threads;
		using KGK_CPU = kwon_gi_kang_cpu;
		using KGK_MT = kwon_gi_kang_cpu_threads;
		using PS_CPU = petrosino_salvi_cpu;
		using PS_MT = petrosino_salvi_thread;
		using TA_CPU = tarabek_cpu;
		using TA_MT = tarabek_threads;
		using ZS_CPU = zhang_suen_cpu;
		using ZS_MT = zhang_suen_cpu_threads;

#if defined(SKELETONIZATION_ENABLE_GPU) || defined(SKELETONIZATION_WITH_GPU)
		using namespace skeletonizer::gpu::algorithms;

		using CLS_GPU = choi_lam_siu_gpu;
		using GH_GPU = guo_hall_gpu;
		using HLR_GPU = han_la_rhee_gpu;
		using KGK_GPU = kwon_gi_kang_gpu;
		using PS_GPU = petrosino_salvi_gpu;
		using TA_GPU = tarabek_gpu;
		using ZS_GPU = zhang_suen_gpu;

#endif

		static const std::vector<triple> v = []
		{
			std::vector<triple> triples;

#if defined(SKELETONIZATION_ENABLE_GPU) || defined(SKELETONIZATION_WITH_GPU)
			triples.emplace_back(make_triple<CLS_CPU, CLS_MT, CLS_GPU>("ChoiLamSiu"));
			triples.emplace_back(make_triple<GH_CPU, GH_MT, GH_GPU>("GuoHall"));
			triples.emplace_back(make_triple<HLR_CPU, HLR_MT, HLR_GPU>("HanLaRhee"));
			triples.emplace_back(make_triple<KGK_CPU, KGK_MT, KGK_GPU>("KwonGiKang"));
			triples.emplace_back(make_triple<PS_CPU, PS_MT, PS_GPU>("PetrosinoSalvi"));
			triples.emplace_back(make_triple<TA_CPU, TA_MT, TA_GPU>("Tarabek"));
			triples.emplace_back(make_triple<ZS_CPU, ZS_MT, ZS_GPU>("ZhangSuen"));
#else
			triples.emplace_back(make_triple<CLS_CPU, CLS_MT>("ChoiLamSiu"));
			triples.emplace_back(make_triple<GH_CPU, GH_MT>("GuoHall"));
			triples.emplace_back(make_triple<HLR_CPU, HLR_MT>("HanLaRhee"));
			triples.emplace_back(make_triple<KGK_CPU, KGK_MT>("KwonGiKang"));
			triples.emplace_back(make_triple<PS_CPU, PS_MT>("PetrosinoSalvi"));
			triples.emplace_back(make_triple<TA_CPU, TA_MT>("Tarabek"));
			triples.emplace_back(make_triple<ZS_CPU, ZS_MT>("ZhangSuen"));
#endif
			return triples;
		}();
		return v;
	}
}
