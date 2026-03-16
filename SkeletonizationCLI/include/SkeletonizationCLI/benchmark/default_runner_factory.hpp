/**
*
* @file default_runner_factory.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares default runner factory implementation.
*
* This file defines the default runner factory used to create runner
* instances with injected CLI services.
*
* Main responsibilities:
* - create runner instances from metadata
* - inject argument, loading, and preprocessing services
* - provide default runner factory behavior
*
* @version 1.0
* @date 2026-03-16
*/

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
	 * @class default_runner_factory
	 * @brief Default implementation of i_runner_factory.
	 *
	 * Creates standard runner instances for benchmarking.
	 * Injects all required dependencies into each created runner.
	 */
	class default_runner_factory final : public cli::interfaces::i_runner_factory
	{
	public:
		/**
		 * @brief Constructs a default runner factory.
		 *
		 * @param args_provider Arguments provider service.
		 * @param image_loader Image loader service.
		 * @param image_preprocessor Image preprocessor service.
		 */
		default_runner_factory(
			std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider,
			std::shared_ptr<cli::interfaces::i_image_loader> image_loader,
			std::shared_ptr<cli::interfaces::i_image_preprocessor> image_preprocessor)
			: args_provider_(std::move(args_provider))
			  , image_loader_(std::move(image_loader))
			  , image_preprocessor_(std::move(image_preprocessor))
		{
		}

		/**
		 * @brief Creates a runner instance for image metadata.
		 *
		 * @param image_metadata Image benchmark metadata.
		 * @return Created runner instance.
		 */
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
		/// Shared arguments provider dependency.
		std::shared_ptr<cli::interfaces::i_arguments_provider> args_provider_;
		/// Shared image loader dependency.
		std::shared_ptr<cli::interfaces::i_image_loader> image_loader_;
		/// Shared image preprocessor dependency.
		std::shared_ptr<cli::interfaces::i_image_preprocessor> image_preprocessor_;
	};
}
