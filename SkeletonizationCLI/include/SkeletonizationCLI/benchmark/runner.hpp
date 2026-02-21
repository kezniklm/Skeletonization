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
	 * @brief Manages benchmark execution for a single image configuration.
	 *
	 * Coordinates image loading, preprocessing, benchmark registration,
	 * and result collection. Uses dependency injection for all external
	 * dependencies to improve testability and flexibility.
	 */
	class runner
	{
	public:
		runner(const configuration::image_benchmark_metadata& image_metadata,
		       std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider,
		       const cli::interfaces::i_image_loader& image_loader,
		       const cli::interfaces::i_image_preprocessor& image_preprocessor);

		void register_all_benchmarks();

		[[nodiscard]] visual_inspector::image_container process_benchmark_results() const;

	private:
		void register_benchmark(const std::string& name,
		                        std::unique_ptr<skeletonizer::skeletonizer<>> skeletonizer);

		std::string create_benchmark_name(const std::string& skeletonizer_name,
		                                  skeletonizer::skeletonizer_type type) const;

		static constexpr int max_algorithm_type_length = 5;
		static constexpr int max_algorithm_author_length = 20;
		static constexpr int user_algorithm_name_length = 25;
		static constexpr int max_total_algorithm_name_length =
			user_algorithm_name_length +
			max_algorithm_author_length +
			max_algorithm_type_length;

		configuration::image_benchmark_metadata image_metadata_;
		std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider_;
		cv::Mat input_image_;
		cv::Mat binary_image_;
		std::map<std::string, cv::Mat> results_;
	};
}
