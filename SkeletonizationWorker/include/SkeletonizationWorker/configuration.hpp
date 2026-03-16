/**
*
* @file configuration.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares worker runtime configuration structures.
*
* This file defines worker runtime configuration models, environment
* parsing helpers, and dependency injection wrapper types.
*
* Main responsibilities:
* - define worker runtime configuration model
* - parse environment values into typed settings
* - define dependency injection wrapper types
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <atomic>
#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "libenvpp/env.hpp"
#include "libenvpp/detail/errors.hpp"

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace worker::configuration::backend
{
	/**
	 * @brief Enumerates supported storage backends.
	 */
	enum class storage_backend
	{
		local,
		s3
	};

	/**
	 * @brief Parses storage backend token.
	 *
	 * @param value Storage backend token.
	 * @return Parsed backend value when token is valid.
	 */
	[[nodiscard]] constexpr std::optional<storage_backend> from_string(const std::string_view value) noexcept
	{
		if (equals_ascii(value, "local"))
		{
			return storage_backend::local;
		}

		if (equals_ascii(value, "s3"))
		{
			return storage_backend::s3;
		}

		return std::nullopt;
	}
}

template <>
/**
 * @class env::default_parser<skeletonizer::skeletonizer_type>
 * @brief Parses skeletonizer backend mode from environment strings.
 */
struct env::default_parser<skeletonizer::skeletonizer_type>
{
	/**
	 * @brief Parses skeletonizer backend mode value.
	 * @param value Environment value token.
	 * @return Parsed skeletonizer backend type.
	 */
	skeletonizer::skeletonizer_type operator()(const std::string_view value) const
	{
		const auto skeletonizer_type = skeletonizer::from_string(value);

		if (!skeletonizer_type)
		{
			throw parser_error{"Invalid MODE value: " + std::string{value}};
		}

		return skeletonizer_type.value();
	}
};

template <>
/**
 * @class env::default_parser<worker::configuration::backend::storage_backend>
 * @brief Parses worker storage backend from environment strings.
 */
struct env::default_parser<worker::configuration::backend::storage_backend>
{
	/**
	 * @brief Parses storage backend value.
	 * @param value Environment value token.
	 * @return Parsed storage backend value.
	 */
	worker::configuration::backend::storage_backend operator()(const std::string_view value) const
	{
		const auto storage_backend = worker::configuration::backend::from_string(value);

		if (!storage_backend)
		{
			throw parser_error{"Invalid storage backend value: " + std::string{value}};
		}

		return storage_backend.value();
	}
};

namespace worker::configuration
{
	/**
	 * @brief Enumerates worker storage backend settings.
	 */
	enum class storage_backend
	{
		local,
		s3
	};

	/**
	 * @class s3_configuration
	 * @brief Stores S3 connection settings.
	 */
	struct s3_configuration
	{
		std::string endpoint;
		std::string region = "auto";
		std::string bucket;
		std::string access_key_id;
		std::string secret_access_key;
		bool force_path_style = true;
	};

	/**
	 * @class env_loader
	 * @brief Loads environment variables from dotenv file.
	 */
	class env_loader
	{
	public:
		explicit env_loader(const std::string_view path = ".env") : path_(path)
		{
		}

		/**
		 * @brief Loads environment file values.
		 */
		void load() const;

	private:
		/// Environment file path.
		std::string path_;
	};

	/**
	 * @brief Returns optional environment value.
	 * @tparam T Expected environment value type.
	 * @param key Environment variable name.
	 * @return Optional parsed value when key exists and parsing succeeds.
	 */
	template <typename T>
	std::optional<T> get_env_optional(const std::string_view key)
	{
		const auto result = env::get<T>(key);

		return result.has_value() ? result.value() : std::nullopt;
	}

	/**
	 * @brief Returns required environment value or throws.
	 * @tparam T Expected environment value type.
	 * @param key Environment variable name.
	 * @return Parsed environment value.
	 */
	template <typename T>
	T get_env(const std::string_view key)
	{
		const auto result = env::get<T>(key);

		if (!result)
		{
			throw std::runtime_error{"Environment variable '" + std::string{key} + "' error: " + result.error().what()};
		}

		return result.value();
	}

	/**
	 * @brief Returns environment value or provided default.
	 * @tparam T Expected environment value type.
	 * @param key Environment variable name.
	 * @param default_value Fallback value returned when key is missing.
	 * @return Parsed environment value or provided default.
	 */
	template <typename T>
	T get_env_or(const std::string_view key, const T default_value)
	{
		const auto result = env::get<T>(key);

		return result.has_value() ? result.value() : default_value;
	}

	/**
	 * @class configuration
	 * @brief Stores worker runtime configuration values.
	 */
	struct configuration
	{
		std::string worker_id;

		std::string redis_url;
		std::string jobs_queue;
		std::string processing_queue;
		std::string results_queue;

		std::string output_directory = "output";

		storage_backend backend = storage_backend::local;
		s3_configuration s3{};

		skeletonizer::skeletonizer_type processor_type;

		int poll_timeout_seconds = 5;

		static configuration from_environment(const env_loader& loader = env_loader{});
	};

	namespace dependency_injection
	{
		/** @class jobs_queue_t @brief Wraps jobs queue name for DI. */
		struct jobs_queue_t
		{
			std::string value;
		};

		/** @class processing_queue_t @brief Wraps processing queue name for DI. */
		struct processing_queue_t
		{
			std::string value;
		};

		/** @class results_queue_t @brief Wraps results queue name for DI. */
		struct results_queue_t
		{
			std::string value;
		};

		/** @class output_directory_t @brief Wraps output directory path for DI. */
		struct output_directory_t
		{
			std::string value;
		};

		/** @class worker_id_t @brief Wraps worker identifier for DI. */
		struct worker_id_t
		{
			std::string value;
		};

		/** @class poll_timeout_t @brief Wraps queue polling timeout for DI. */
		struct poll_timeout_t
		{
			std::chrono::seconds value;
		};

		/** @class cancellation_token_t @brief Shared cancellation state for loop control. */
		struct cancellation_token_t
		{
			std::atomic<bool> cancelled{false};

			/**
			 * @brief Requests cancellation.
			 */
			void request_cancel() noexcept
			{
				cancelled.store(true, std::memory_order_relaxed);
			}

			/**
			 * @brief Checks whether cancellation is requested.
			 *
			 * @return True when cancellation is requested.
			 */
			bool is_cancellation_requested() const noexcept
			{
				return cancelled.load(std::memory_order_relaxed);
			}
		};
	}
}
