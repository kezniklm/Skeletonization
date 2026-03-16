/**
*
* @file benchmark_registry.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark registration utilities.
*
* This file defines the benchmark registry that stores and manages runner
* instances created from image metadata.
*
* Main responsibilities:
* - register benchmark runners from metadata
* - remove and clear registered runners
* - expose iterable runner collection for execution
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "runner.hpp"
#include "SkeletonizationCLI/interfaces/i_runner_factory.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @class benchmark_registry
	 * @brief Registry for benchmark runners.
	 *
	 * Manages the lifecycle of benchmark runners, supporting registration,
	 * removal, and iteration. Follows Single Responsibility Principle by
	 * separating runner management from execution.
	 */
	class benchmark_registry
	{
	public:
		/// Stored runner entry type.
		using runner_entry = std::pair<std::string, std::unique_ptr<runner>>;
		/// Container type for registered runners.
		using runner_container = std::vector<runner_entry>;
		/// Const iterator type for runner container.
		using const_iterator = runner_container::const_iterator;

		/**
		 * @brief Constructs an empty benchmark registry.
		 */
		benchmark_registry() = default;

		/**
		 * @brief Construct with a custom runner factory for dependency injection.
		 * @param factory Runner factory instance used to create runners.
		 */
		explicit benchmark_registry(std::shared_ptr<cli::interfaces::i_runner_factory> factory)
			: runner_factory_(std::move(factory))
		{
		}

		/**
		 * @brief Add a runner for the given image metadata.
		 * @param image_metadata Image benchmark metadata used to create runner.
		 */
		void add(const configuration::image_benchmark_metadata& image_metadata);

		/**
		 * @brief Remove a runner by name.
		 * @param name Benchmark name to remove.
		 */
		void remove(const std::string& name);

		/**
		 * @brief Clear all registered runners.
		 */
		void clear();

		/**
		 * @brief Check if any runners are registered.
		 * @return True when no runners are registered; otherwise false.
		 */
		[[nodiscard]] bool empty() const noexcept
		{
			return runners_.empty();
		}

		/**
		 * @brief Get the number of registered runners.
		 * @return Number of registered runners.
		 */
		[[nodiscard]] std::size_t size() const noexcept
		{
			return runners_.size();
		}

		/**
		 * @brief Get read-only access to runners for iteration.
		 * @return Const reference to internal runner container.
		 */
		[[nodiscard]] const runner_container& runners() const noexcept
		{
			return runners_;
		}

		/**
		 * @brief Returns iterator to beginning of runner collection.
		 * @return Const iterator to first runner entry.
		 */
		[[nodiscard]] const_iterator begin() const noexcept
		{
			return runners_.begin();
		}

		/**
		 * @brief Returns iterator to end of runner collection.
		 * @return Const iterator past last runner entry.
		 */
		[[nodiscard]] const_iterator end() const noexcept
		{
			return runners_.end();
		}

	private:
		/// Registered runner collection.
		runner_container runners_;
		/// Runner factory used to create new entries.
		std::shared_ptr<cli::interfaces::i_runner_factory> runner_factory_;
	};
}
