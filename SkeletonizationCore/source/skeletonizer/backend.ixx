module;

#if SKELETONIZATION_WITH_GPU

#include "opencv2/cudaarithm.hpp"

#endif

export module skeletonizer:backend;

namespace skeletonizer
{
	class backend
	{
	};

	export class backend_cpu : public backend
	{
	};

	export class backend_threads : public backend
	{
	};

	export template <int BlockDimensionX = 16, int BlockDimensionY = 16, int Halo = 1>
	class backend_gpu : public backend
	{
	protected:
		static constexpr int block_dimension_x = BlockDimensionX;
		static constexpr int block_dimension_y = BlockDimensionY;
		static constexpr int halo = Halo;
	};

#if SKELETONIZATION_WITH_GPU
	cv::cuda::GpuMat operator~(const cv::cuda::GpuMat& mat)
	{
		cv::cuda::GpuMat result;

		cv::cuda::bitwise_not(mat, result);

		return result;
	}
#endif
}
