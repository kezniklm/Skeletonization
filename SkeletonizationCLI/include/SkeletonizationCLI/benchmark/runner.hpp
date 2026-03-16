/**
*
* @file runner.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark runner abstractions.
*
* This file defines benchmark runner orchestration for one image metadata
* configuration.
*
* Main responsibilities:
* - load and preprocess benchmark input images
* - register benchmark cases for selected algorithms
* - collect benchmark output artifacts and metrics
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <map>
#include <memory>
#include <string>

#include <opencv2/core.hpp>

#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"
#include "SkeletonizationCLI/interfaces/i_image_loader.hpp"
#include "SkeletonizationCLI/interfaces/i_image_preprocessor.hpp"
#include "SkeletonizationCLI/visual_inspector/image_container.hpp"
#include "SkeletonizationCore/configuration/types.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class runner
	 * @brief Manages benchmark execution for a single image configuration.
	 *
	 * Coordinates image loading, preprocessing, benchmark registration,
	 * and result collection. Uses dependency injection for all external
	 * dependencies to improve testability and flexibility.
	 */
	class runner
	{
	public:
		/**
		 * @brief Constructs a runner for image benchmark metadata.
		 *
		 * @param image_metadata Image benchmark metadata.
		 * @param args_provider Arguments provider dependency.
		 * @param image_loader Image loader dependency.
		 * @param image_preprocessor Image preprocessor dependency.
		 */
		runner(const configuration::image_benchmark_metadata& image_metadata,
		       std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider,
		       const cli::interfaces::i_image_loader& image_loader,
		       const cli::interfaces::i_image_preprocessor& image_preprocessor);

		/**
		 * @brief Registers all benchmark cases for this runner.
		 */
		void register_all_benchmarks();

		/**
		 * @brief Processes benchmark run output into image container result.
		 *
		 * @return Processed image container.
		 */
		[[nodiscard]] visual_inspector::image_container process_benchmark_results() const;

	private:
		/**
		 * @brief Registers one benchmark case.
		 *
		 * @param name Benchmark name.
		 * @param skeletonizer Skeletonizer implementation instance.
		 */
		void register_benchmark(const std::string& name,
		                        std::unique_ptr<skeletonizer::skeletonizer<>> skeletonizer);

		/**
		 * @brief Creates canonical benchmark name for algorithm and backend.
		 *
		 * @param skeletonizer_name Algorithm display name.
		 * @param type Backend type.
		 * @return Composed benchmark name.
		 */
		std::string create_benchmark_name(const std::string& skeletonizer_name,
		                                  skeletonizer::skeletonizer_type type) const;

		/// Maximum backend type string length.
		static constexpr int max_algorithm_type_length = 5;
		/// Maximum author string length.
		static constexpr int max_algorithm_author_length = 20;
		/// Maximum user algorithm name length.
		static constexpr int user_algorithm_name_length = 25;
		/// Maximum total benchmark algorithm name length.
		static constexpr int max_total_algorithm_name_length =
			user_algorithm_name_length +
			max_algorithm_author_length +
			max_algorithm_type_length;

		/// Current image benchmark metadata.
		configuration::image_benchmark_metadata image_metadata_;
		/// Arguments provider dependency.
		std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider_;
		/// Loaded original input image.
		cv::Mat input_image_;
		/// Preprocessed binary image.
		cv::Mat binary_image_;
		/// Per-benchmark output image map.
		std::map<std::string, cv::Mat> results_;
	};
}
