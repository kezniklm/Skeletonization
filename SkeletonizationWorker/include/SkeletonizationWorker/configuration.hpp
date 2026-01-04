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
	enum class storage_backend
	{
		local,
		s3
	};

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
struct env::default_parser<skeletonizer::skeletonizer_type>
{
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
struct env::default_parser<worker::configuration::backend::storage_backend>
{
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
	enum class storage_backend
	{
		local,
		s3
	};

	struct s3_configuration
	{
		std::string endpoint;
		std::string region = "auto";
		std::string bucket;
		std::string access_key_id;
		std::string secret_access_key;
		bool force_path_style = true;
	};

	class env_loader
	{
	public:
		explicit env_loader(const std::string_view path = ".env") : path_(path)
		{
		}

		void load() const;

	private:
		std::string path_;
	};

	template <typename T>
	std::optional<T> get_env_optional(const std::string_view key)
	{
		const auto result = env::get<T>(key);

		return result.has_value() ? result.value() : std::nullopt;
	}

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

	template <typename T>
	T get_env_or(const std::string_view key, const T default_value)
	{
		const auto result = env::get<T>(key);

		return result.has_value() ? result.value() : default_value;
	}

	struct configuration
	{
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
		struct jobs_queue_t
		{
			std::string value;
		};

		struct processing_queue_t
		{
			std::string value;
		};

		struct results_queue_t
		{
			std::string value;
		};

		struct output_directory_t
		{
			std::string value;
		};

		struct poll_timeout_t
		{
			std::chrono::seconds value;
		};

		struct cancellation_token_t
		{
			std::atomic<bool> cancelled{false};

			void request_cancel() noexcept
			{
				cancelled.store(true, std::memory_order_relaxed);
			}

			bool is_cancellation_requested() const noexcept
			{
				return cancelled.load(std::memory_order_relaxed);
			}
		};
	}
}
