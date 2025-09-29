module;

#include "opencv2/cudaarithm.hpp"

export module skeletonizer_gpu:core;

import skeletonizer;

namespace skeletonizer::gpu
{
	export template <int BlockDimensionX = 16, int BlockDimensionY = 16, int Halo = 1>
	class skeletonizer_gpu : virtual public skeletonizer<>
	{
	protected:
		static constexpr int block_dimension_x = BlockDimensionX;
		static constexpr int block_dimension_y = BlockDimensionY;
		static constexpr int halo = Halo;
	};

	cv::cuda::GpuMat operator~(const cv::cuda::GpuMat& mat)
	{
		cv::cuda::GpuMat result;

		cv::cuda::bitwise_not(mat, result);

		return result;
	}
}
