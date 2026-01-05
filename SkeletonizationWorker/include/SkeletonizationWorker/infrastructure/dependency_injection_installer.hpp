#pragma once

#include <memory>
#include <stdexcept>

#include <boost/di.hpp>

#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/application/interfaces/image_service.hpp"
#include "SkeletonizationWorker/application/interfaces/jobs_queue.hpp"
#include "SkeletonizationWorker/application/interfaces/job_deserializer.hpp"
#include "SkeletonizationWorker/application/interfaces/job_status_store.hpp"
#include "SkeletonizationWorker/application/interfaces/object_storage.hpp"
#include "SkeletonizationWorker/application/interfaces/result_publisher.hpp"
#include "SkeletonizationWorker/application/interfaces/result_transport.hpp"
#include "SkeletonizationWorker/application/interfaces/skeletonization_processor.hpp"
#include "SkeletonizationWorker/infrastructure/aws_sdk_initializer.hpp"
#include "SkeletonizationWorker/infrastructure/image_service.hpp"
#include "SkeletonizationWorker/infrastructure/local_object_storage.hpp"
#include "SkeletonizationWorker/infrastructure/s3_object_storage.hpp"
#include "SkeletonizationWorker/infrastructure/rapidjson_job_deserializer.hpp"
#include "SkeletonizationWorker/infrastructure/redis.hpp"
#include "SkeletonizationWorker/infrastructure/redis_adapter.hpp"
#include "SkeletonizationWorker/infrastructure/result_publisher.hpp"
#include "SkeletonizationWorker/infrastructure/skeletonization_processor.hpp"

namespace worker::infrastructure::dependency_injection
{
	namespace di = boost::di;

	inline auto make_configuration_installer(const configuration::configuration& configuration)
	{
		using namespace configuration::dependency_injection;

		return di::make_injector(
			di::bind<configuration::configuration>.to(configuration),
			di::bind<worker_id_t>.to(worker_id_t{configuration.worker_id}),
			di::bind<jobs_queue_t>.to(jobs_queue_t{configuration.jobs_queue}),
			di::bind<processing_queue_t>.to(processing_queue_t{configuration.processing_queue}),
			di::bind<results_queue_t>.to(results_queue_t{configuration.results_queue}),
			di::bind<output_directory_t>.to(output_directory_t{configuration.output_directory}),
			di::bind<poll_timeout_t>.to(poll_timeout_t{std::chrono::seconds{configuration.poll_timeout_seconds}}),
			di::bind<cancellation_token_t>.to([]() -> cancellation_token_t&
			{
				static cancellation_token_t token{};
				return token;
			}),
			di::bind<skeletonizer::skeletonizer_type>.to(configuration.processor_type),
			di::bind<configuration::s3_configuration>.to(configuration.s3)
		);
	}

	inline auto make_redis_installer(const configuration::configuration& configuration)
	{
		auto redis_config = redis::redis_configuration::from_url(configuration.redis_url);

		if (!redis_config)
		{
			throw std::runtime_error("Failed to parse REDIS_URL: " + redis_config.error());
		}

		return di::make_injector(
			di::bind<redis::redis_configuration>.to(std::move(redis_config.value())),
			di::bind<redis::client>.in(di::singleton)
		);
	}

	inline std::shared_ptr<application::interfaces::i_object_storage> make_object_storage(
		const configuration::configuration& configuration)
	{
		if (configuration.backend == configuration::storage_backend::local)
		{
			return std::make_shared<local_object_storage>();
		}

		static aws_sdk_initializer aws_init;

		return std::make_shared<s3_object_storage>(aws_init, configuration.s3);
	}

	inline auto make_services_installer(const configuration::configuration& configuration)
	{
		return di::make_injector(
			di::bind<application::interfaces::i_job_deserializer>.to<rapidjson_job_deserializer>().in(di::singleton),
			di::bind<application::interfaces::i_image_service>.to<image_service>(),
			di::bind<application::interfaces::i_object_storage>.to(make_object_storage(configuration)),
			di::bind<application::interfaces::i_skeletonization_processor>.to<skeletonization_processor>(),
			di::bind<application::interfaces::i_result_publisher>.to<result_publisher>().in(di::singleton),
			di::bind<application::interfaces::i_jobs_queue>.to<redis_adapter>(),
			di::bind<application::interfaces::i_result_transport>.to<redis_adapter>(),
			di::bind<application::interfaces::i_job_status_store>.to<redis_adapter>()
		);
	}

	inline auto make_installer(const configuration::configuration& configuration)
	{
		return di::make_injector(
			make_configuration_installer(configuration),
			make_redis_installer(configuration),
			make_services_installer(configuration)
		);
	}
}
