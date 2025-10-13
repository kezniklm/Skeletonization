module;

#include "choi_lam_siu.cuh"

#include "opencv2/core.hpp"
#include "opencv2/cudaarithm.hpp"

export module skeletonizer_gpu:choi_lam_siu;

import skeletonizer;
import :core;

export namespace skeletonizer::gpu::algorithms
{
	class choi_lam_siu_gpu final : public ::skeletonizer::algorithms::choi_lam_siu, public skeletonizer_gpu<32>
	{
	public:
		void apply(cv::Mat& binary_image) const override
		{
			cv::cuda::GpuMat gpu_binary_image(binary_image);

			constexpr dim3 block(block_dimension_x, block_dimension_y);
			const dim3 grid((binary_image.cols + block.x - 1) / block.x,
			                (binary_image.rows + block.y - 1) / block.y);

			const auto label_matrix = compute_nearest_background_labels(binary_image);

			const cv::cuda::GpuMat label_matrix_gpu(label_matrix);

			const auto max_label = get_max_array_value(label_matrix);

			const auto lut_size = max_label + 1;

			const auto lut = build_label_to_background_point_lut(gpu_binary_image, label_matrix_gpu, block, grid,
			                                                     lut_size);

			skeletonize(gpu_binary_image, label_matrix_gpu, lut, block, grid, halo);

			gpu_binary_image.download(binary_image);

			clear_border(binary_image);
		}
	};
}
