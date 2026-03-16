/**
*
* @file i_runner_factory.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares runner factory interface.
*
* This file defines the contract for creating benchmark runner instances
* from image benchmark metadata.
*
* Main responsibilities:
* - create configured benchmark runner instances
* - abstract runner construction from orchestrators
* - support testable runner factory substitutions
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <memory>
#include <string>

#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class runner
	 * @brief Forward declaration of benchmark runner type.
	 */
	class runner;
}

namespace cli::interfaces
{
	/**
	 * @class i_runner_factory
	 * @brief Interface for creating benchmark runner instances.
	 *
	 * Abstracts runner creation for improved testability and
	 * flexibility in runner configuration.
	 */
	class i_runner_factory
	{
	public:
		/**
		 * @brief Destroys the runner factory interface.
		 */
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
