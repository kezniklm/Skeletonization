#pragma once

#include <memory>
#include <string>

#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	class runner;
}

namespace cli::interfaces
{
	/**
	 * @brief Interface for creating benchmark runner instances.
	 *
	 * Abstracts runner creation for improved testability and
	 * flexibility in runner configuration.
	 */
	class i_runner_factory
	{
	public:
		virtual ~i_runner_factory() = default;

		/**
		 * @brief Create a new benchmark runner for the given image metadata.
		 * @param image_metadata Configuration for the benchmark run.
		 * @return Unique pointer to the created runner.
		 */
		[[nodiscard]] virtual std::unique_ptr<skeletonization_benchmark::runner>
		create(const configuration::image_benchmark_metadata& image_metadata) const = 0;

	protected:
		i_runner_factory() = default;
		i_runner_factory(const i_runner_factory&) = default;
		i_runner_factory& operator=(const i_runner_factory&) = default;
		i_runner_factory(i_runner_factory&&) = default;
		i_runner_factory& operator=(i_runner_factory&&) = default;
	};
}
