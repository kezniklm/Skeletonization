/**
*
* @file tarabek.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the CPU tarabek thinning algorithm.
*
* This header defines the CPU implementation interface for tarabek thinning,
* including template-set processing and postprocessing helpers.
*
* Main responsibilities:
* - declare the CPU tarabek algorithm class
* - expose two-pass thinning helper declarations
* - define template-set and postprocessing helper declarations
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <array>
#include <cstdint>

#include <opencv2/core.hpp>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/algorithms.hpp"

namespace skeletonizer::cpu::algorithms
{
	/**
	 * @class tarabek
	 * @brief Implements tarabek thinning on CPU.
	 *
	 * This class evaluates neighborhood templates in alternating iterations and
	 * performs postprocessing to finalize the skeleton.
	 */
	class tarabek final : public backend_cpu, public ::skeletonizer::algorithms::tarabek
	{
	public:
		/**
		 * @brief Applies tarabek thinning to a binary image.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		void apply(cv::Mat& binary_image) const override;

	private:
		/**
		 * @brief Executes the first tarabek iteration.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void first_iteration(cv::Mat& binary_image, cv::Mat& marker);
		/**
		 * @brief Executes the second tarabek iteration.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param marker Temporary marker matrix.
		 */
		static void second_iteration(cv::Mat& binary_image, cv::Mat& marker);

		/**
		 * @brief Processes a template set against image neighborhoods.
		 *
		 * @param binary_image Binary image modified in place.
		 * @param templates_bits Template bit definitions.
		 * @param background_value Background pixel value.
		 */
		template <std::size_t N>
		static void process_template_set(
			cv::Mat& binary_image,
			const std::array<mask_bits, N>& templates_bits,
			std::uint8_t background_value);

		/**
		 * @brief Runs tarabek postprocessing refinements.
		 *
		 * @param binary_image Binary image modified in place.
		 */
		static void postprocessing(cv::Mat& binary_image);
	};
}
