/**
*
* @file backend.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines backend marker types for skeletonizer implementations.
*
* This header provides lightweight backend tags used to classify algorithm
* implementations by execution model. It also defines a CUDA helper operator
* used by GPU backends when GPU support is enabled.
*
* Main responsibilities:
* - define backend category marker types
* - provide compile-time backend inheritance targets
* - expose GPU matrix inversion helper for CUDA flows
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#if SKELETONIZATION_WITH_GPU
#include <opencv2/cudaarithm.hpp>
#endif

namespace skeletonizer
{
	/**
	 * @class backend
	 * @brief Represents a base marker type for all execution backends.
	 *
	 * This class provides a common inheritance root used for backend
	 * classification in concepts and algorithm registration logic.
	 */
	class backend
	{
	};

	/**
	 * @class backend_cpu
	 * @brief Represents a CPU execution backend marker.
	 *
	 * This marker is used to classify skeletonizer implementations that execute
	 * on a single CPU context.
	 */
	class backend_cpu : public backend
	{
	};

	/**
	 * @class backend_threads
	 * @brief Represents a multithreaded CPU execution backend marker.
	 *
	 * This marker identifies implementations designed for threaded execution.
	 */
	class backend_threads : public backend
	{
	};

	/**
	 * @class backend_gpu
	 * @brief Represents a GPU execution backend marker.
	 *
	 * This marker carries compile-time block and halo parameters used by GPU
	 * algorithm implementations.
	 */
	template <int BlockDimensionX = 16, int BlockDimensionY = 16, int Halo = 1>
	class backend_gpu : public backend
	{
	protected:
		/// Block dimension in X direction.
		static constexpr int block_dimension_x = BlockDimensionX;
		/// Block dimension in Y direction.
		static constexpr int block_dimension_y = BlockDimensionY;
		/// Halo size around each processed tile.
		static constexpr int halo = Halo;
	};

#if SKELETONIZATION_WITH_GPU
	/**
	 * @brief Computes bitwise inversion of a GPU matrix.
	 *
	 * @param mat Input GPU matrix.
	 * @return Inverted GPU matrix.
	 */
	inline cv::cuda::GpuMat operator~(const cv::cuda::GpuMat& mat)
	{
		cv::cuda::GpuMat result;
		cv::cuda::bitwise_not(mat, result);
		return result;
	}
#endif
}
