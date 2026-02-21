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
	 * @brief Registry for benchmark runners.
	 *
	 * Manages the lifecycle of benchmark runners, supporting registration,
	 * removal, and iteration. Follows Single Responsibility Principle by
	 * separating runner management from execution.
	 */
	class benchmark_registry
	{
	public:
		using runner_entry = std::pair<std::string, std::unique_ptr<runner>>;
		using runner_container = std::vector<runner_entry>;
		using const_iterator = runner_container::const_iterator;

		benchmark_registry() = default;

		/**
		 * @brief Construct with a custom runner factory for dependency injection.
		 */
		explicit benchmark_registry(std::shared_ptr<cli::interfaces::i_runner_factory> factory)
			: runner_factory_(std::move(factory))
		{
		}

		/**
		 * @brief Add a runner for the given image metadata.
		 */
		void add(const configuration::image_benchmark_metadata& image_metadata);

		/**
		 * @brief Remove a runner by name.
		 */
		void remove(const std::string& name);

		/**
		 * @brief Clear all registered runners.
		 */
		void clear();

		/**
		 * @brief Check if any runners are registered.
		 */
		[[nodiscard]] bool empty() const noexcept
		{
			return runners_.empty();
		}

		/**
		 * @brief Get the number of registered runners.
		 */
		[[nodiscard]] std::size_t size() const noexcept
		{
			return runners_.size();
		}

		/**
		 * @brief Get read-only access to runners for iteration.
		 */
		[[nodiscard]] const runner_container& runners() const noexcept
		{
			return runners_;
		}

		[[nodiscard]] const_iterator begin() const noexcept
		{
			return runners_.begin();
		}

		[[nodiscard]] const_iterator end() const noexcept
		{
			return runners_.end();
		}

	private:
		runner_container runners_;
		std::shared_ptr<cli::interfaces::i_runner_factory> runner_factory_;
	};
}
