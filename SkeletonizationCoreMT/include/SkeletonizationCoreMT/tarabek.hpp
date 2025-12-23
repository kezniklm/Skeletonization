#pragma once

#include <array>
#include <cstdint>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::mt::algorithms
{
	using std::uint8_t;

	class tarabek final : public backend_threads, public ::skeletonizer::algorithms::tarabek
	{
	public:
		void apply(cv::Mat& binary_image) const override;

	private:
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);

		template <std::size_t N>
		static void process_template_set(
			cv::Mat& binary_image,
			const std::array<mask_bits, N>& templates_bits,
			const uint8_t background
		)
		{
			cv::Mat marker(binary_image.size(), CV_8UC1);

			bool changed;

			do
			{
				changed = false;
				marker.setTo(0);

				cv::parallel_for_(cv::Range(1, binary_image.rows - 1),
				                  [&](const cv::Range& range)
				                  {
					                  for (int row = range.start; row < range.end; ++row)
					                  {
						                  const auto* previous = binary_image.ptr<uint8_t>(row - 1);
						                  const auto* current = binary_image.ptr<uint8_t>(row);
						                  const auto* next = binary_image.ptr<uint8_t>(row + 1);
						                  auto* marker_pointer = marker.ptr<uint8_t>(row);

						                  for (int c = 1; c < binary_image.cols - 1; ++c)
						                  {
							                  if (current[c] == background)
							                  {
								                  continue;
							                  }

							                  const auto neigh_bits = compute_neigh_bits(previous, current, next, c);

							                  for (std::size_t template_bit_position = 0;
							                       template_bit_position < templates_bits.size();
							                       ++template_bit_position)
							                  {
								                  const mask_bits& mb = templates_bits[template_bit_position];

								                  if (((neigh_bits ^ mb.fixed_value) & mb.fixed_mask) != 0)
								                  {
									                  continue;
								                  }

								                  if (!aux_conditions_met_bits(mb, neigh_bits))
								                  {
									                  continue;
								                  }

								                  marker_pointer[c] = background;
								                  changed = true;
								                  break;
							                  }
						                  }
					                  }
				                  });

				binary_image &= ~marker;
			}
			while (changed);
		}

		static inline void postprocessing(cv::Mat& binary_image);
	};
}
