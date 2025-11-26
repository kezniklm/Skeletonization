module;

#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <opencv2/core.hpp>

export module tests_common:triples;

import :runner;

import skeletonizer_cpu;
import skeletonizer_mt;

#if SKELETONIZATION_WITH_GPU
import skeletonizer_gpu;
#endif

export namespace skeltest
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
			std::optional{std::function<cv::Mat(const cv::Mat&)>{[rgpu](const cv::Mat& m) { return rgpu(m); }}}
		};
#else
		return triple{
			std::move(name),
			[rcpu](const cv::Mat& m) { return rcpu(m); },
			[rmt](const cv::Mat& m) { return rmt(m); }
		};
#endif
	}

	export inline const std::vector<triple>& triples()
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
			std::vector<triple> triples;

#if SKELETONIZATION_WITH_GPU
			triples.emplace_back(make_triple<CLS_CPU, CLS_MT, CLS_GPU>("ChoiLamSiu"));
			triples.emplace_back(make_triple<GH_CPU, GH_MT, GH_GPU>("GuoHall"));
			triples.emplace_back(make_triple<HLR_CPU, HLR_MT, HLR_GPU>("HanLaRhee"));
			triples.emplace_back(make_triple<KGK_CPU, KGK_MT, KGK_GPU>("KwonGiKang"));
			triples.emplace_back(make_triple<LZ_CPU, LZ_MT, LZ_GPU>("LiuZhang"));
			triples.emplace_back(make_triple<PS_CPU, PS_MT, PS_GPU>("PetrosinoSalvi"));
			triples.emplace_back(make_triple<TA_CPU, TA_MT, TA_GPU>("Tarabek"));
			triples.emplace_back(make_triple<ZS_CPU, ZS_MT, ZS_GPU>("ZhangSuen"));
#else
			triples.emplace_back(make_triple<CLS_CPU, CLS_MT>("ChoiLamSiu"));
			triples.emplace_back(make_triple<GH_CPU, GH_MT>("GuoHall"));
			triples.emplace_back(make_triple<HLR_CPU, HLR_MT>("HanLaRhee"));
			triples.emplace_back(make_triple<KGK_CPU, KGK_MT>("KwonGiKang"));
			triples.emplace_back(make_triple<LZ_CPU, LZ_MT>("LiuZhang"));
			triples.emplace_back(make_triple<PS_CPU, PS_MT>("PetrosinoSalvi"));
			triples.emplace_back(make_triple<TA_CPU, TA_MT>("Tarabek"));
			triples.emplace_back(make_triple<ZS_CPU, ZS_MT>("ZhangSuen"));
#endif
			return triples;
		}();
		return v;
	}
}
