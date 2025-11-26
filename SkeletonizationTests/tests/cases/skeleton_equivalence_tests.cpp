#include "gtest/gtest.h"
#include "opencv2/core.hpp"

import tests_common;

using namespace skeltest;

class per_image_per_triple : public testing::TestWithParam<param>
{
};

TEST_P(per_image_per_triple, CpuMtGpu_Identical)
{
	const auto& test_image = test_images();
	const auto& trs = triples();

	const auto [ti, ii] = GetParam();
	const auto& triple = trs.at(ti);
	const auto& img = test_image.at(ii);

	const cv::Mat reference_implementation = triple.cpu(img);

	ASSERT_TRUE(is_binary01(reference_implementation)) << "CPU output not binary {0,1}";
	EXPECT_TRUE(subset01(reference_implementation, to01(img))) << "Subset violation";

	const cv::Mat threaded_implementation = triple.mt(img);

	ASSERT_TRUE(is_binary01(threaded_implementation)) << "MT output not binary {0,1}";
	EXPECT_TRUE(mat_equal01(reference_implementation, threaded_implementation))
        << "CPU vs MT mismatch (" << triple.name << ", img " << ii << ", " << size_str(img.size()) << ")";

#if SKELETONIZATION_WITH_GPU
	if (!triple.gpu.has_value())
	{
		return;
	}

	const cv::Mat gpu_implementation = (*triple.gpu)(img);

	ASSERT_TRUE(is_binary01(gpu_implementation)) << "GPU output not binary {0,1}";
	EXPECT_TRUE(mat_equal01(reference_implementation, gpu_implementation))
                << "CPU vs GPU mismatch (" << triple.name << ", img " << ii << ", " << size_str(img.size()) << ")";

#endif
}

INSTANTIATE_TEST_SUITE_P(
	AllTriplesAllImages,
	per_image_per_triple,
	::testing::ValuesIn(all_params( static_cast<int>(triples().size()), static_cast<int>(test_images().size()) )),
	[](const ::testing::TestParamInfo<param>& info) -> std::string {
	const auto& imgs = test_images();
	const auto& trs = triples();
	const int ti = info.param.first;
	const int ii = info.param.second;
	return param_name(trs.at(ti).name, ii, static_cast<int>(imgs.at(ii).cols), static_cast<int>(imgs.at(ii).rows));
	}
);
