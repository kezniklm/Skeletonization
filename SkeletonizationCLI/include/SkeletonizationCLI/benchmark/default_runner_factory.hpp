#pragma once

#include <memory>

#include "runner.hpp"
#include "SkeletonizationCLI/interfaces/i_arguments_provider.hpp"
#include "SkeletonizationCLI/interfaces/i_image_loader.hpp"
#include "SkeletonizationCLI/interfaces/i_image_preprocessor.hpp"
#include "SkeletonizationCLI/interfaces/i_runner_factory.hpp"
#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	/**
	 * @brief Default implementation of i_runner_factory.
	 *
	 * Creates standard runner instances for benchmarking.
	 * Injects all required dependencies into each created runner.
	 */
	class default_runner_factory final : public cli::interfaces::i_runner_factory
	{
	public:
		default_runner_factory(
			std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider,
			std::shared_ptr<cli::interfaces::i_image_loader> image_loader,
			std::shared_ptr<cli::interfaces::i_image_preprocessor> image_preprocessor)
			: args_provider_(std::move(args_provider))
			  , image_loader_(std::move(image_loader))
			  , image_preprocessor_(std::move(image_preprocessor))
		{
		}

		[[nodiscard]] std::unique_ptr<runner>
		create(const configuration::image_benchmark_metadata& image_metadata) const override
		{
			return std::make_unique<runner>(
				image_metadata,
				args_provider_,
				*image_loader_,
				*image_preprocessor_);
		}

	private:
		std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider_;
		std::shared_ptr<cli::interfaces::i_image_loader> image_loader_;
		std::shared_ptr<cli::interfaces::i_image_preprocessor> image_preprocessor_;
	};
}
