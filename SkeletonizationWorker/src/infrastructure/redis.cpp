/**
*
* @file redis.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements Redis operations used by worker infrastructure.
*
* This file implements Redis client connection and command operations.
*
* Main responsibilities:
* - connect and authenticate Redis sessions
* - execute queue and key-value commands
* - apply retries and reconnect behavior
*
* @version 1.0
* @date 2026-03-16
*/

#include <algorithm>
#include <ranges>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

#include "SkeletonizationWorker/infrastructure/redis.hpp"

namespace worker::infrastructure::redis
{
	client::client(redis_configuration configuration)
		: configuration_(std::move(configuration))
		  , redis_context_(nullptr)
		  , ssl_context_(std::nullopt)
		  , rng_(std::random_device{}())
	{
		LOG(INFO) << "Redis client created for " << connection_info();
	}

	client::~client()
	{
		disconnect();

		LOG(INFO) << "Redis connection destroyed.";
	}

	std::string client::connection_info() const
	{
		std::ostringstream oss;
		oss << (configuration_.uri.use_ssl ? "rediss://" : "redis://")
			<< configuration_.uri.host << ":" << configuration_.uri.port
			<< "/" << configuration_.uri.database;
		return oss.str();
	}

	std::expected<void, std::string> client::ensure_connected()
	{
		return connect();
	}

	std::expected<std::string, std::string> client::pop_safe(const std::string_view list,
	                                                         const std::string_view processing_key,
	                                                         const std::chrono::seconds timeout)
	{
		const std::string timeout_str = std::to_string(static_cast<int>(timeout.count()));
		const std::vector<std::string_view> arguments = {
			"BRPOPLPUSH", list, processing_key, timeout_str
		};

		const auto reply_result = execute_with_retry(arguments);

		if (!reply_result)
		{
			return std::unexpected(reply_result.error());
		}

		const auto& reply = reply_result.value().get();

		if (!reply)
		{
			return std::unexpected(std::string("null reply from BRPOPLPUSH"));
		}
		if (reply->type == REDIS_REPLY_STRING)
		{
			return std::string(reply->str, reply->len);
		}

		if (reply->type == REDIS_REPLY_NIL)
		{
			return {};
		}

		if (reply->type == REDIS_REPLY_ERROR)
		{
			return std::unexpected(std::string("BRPOPLPUSH error: ") + (reply->str ? reply->str : ""));
		}

		return {};
	}

	std::expected<void, std::string> client::acknowledge_job(const std::string_view processing_key,
	                                                         const std::string_view job_payload)
	{
		const std::vector<std::string_view> arguments = {"LREM", processing_key, "1", job_payload};

		const auto reply_result = execute_with_retry(arguments);

		if (!reply_result)
		{
			return std::unexpected(reply_result.error());
		}

		const auto& reply = reply_result.value().get();

		if (!reply)
		{
			return std::unexpected(std::string("null reply from LREM"));
		}

		if (reply->type == REDIS_REPLY_INTEGER)
		{
			return {};
		}

		if (reply->type == REDIS_REPLY_ERROR)
		{
			return std::unexpected(std::string("LREM error: ") + (reply->str ? reply->str : ""));
		}

		return {};
	}

	std::expected<void, std::string> client::requeue(const std::string_view list, const std::string_view job_payload)
	{
		const std::vector<std::string_view> args = {"LPUSH", list, job_payload};

		const auto reply_result = execute_with_retry(args);

		if (!reply_result)
		{
			return std::unexpected(reply_result.error());
		}

		return {};
	}

	std::expected<void, std::string> client::set(const std::string_view key, const std::string_view value)
	{
		const std::vector<std::string_view> arguments = {"SET", key, value};

		auto reply_result = execute_with_retry(arguments);

		if (!reply_result)
		{
			return std::unexpected(reply_result.error());
		}

		const auto reply = reply_result.value().get();

		if (!reply)
		{
			return std::unexpected(std::string("null reply from SET"));
		}

		if (reply->type == REDIS_REPLY_STATUS)
		{
			return {};
		}

		if (reply->type == REDIS_REPLY_ERROR)
		{
			return std::unexpected(std::string("SET error: ") + (reply->str ? reply->str : ""));
		}

		return {};
	}

	std::expected<std::string, std::string> client::get(const std::string_view key)
	{
		const std::vector<std::string_view> arguments = {"GET", key};

		const auto reply_result = execute_with_retry(arguments);

		if (!reply_result)
		{
			return std::unexpected(reply_result.error());
		}

		const auto reply = reply_result.value().get();

		if (!reply)
		{
			return std::unexpected(std::string("null reply from GET"));
		}

		if (reply->type == REDIS_REPLY_STRING)
		{
			return std::string(reply->str, reply->len);
		}

		if (reply->type == REDIS_REPLY_NIL)
		{
			return std::string{};
		}

		if (reply->type == REDIS_REPLY_ERROR)
		{
			return std::unexpected(std::string("GET error: ") + (reply->str ? reply->str : ""));
		}

		return std::string{};
	}

	std::expected<void, std::string> client::connect()
	{
		if (redis_context_ && redis_context_->err == 0)
		{
			return {};
		}

		disconnect();

		if (configuration_.uri.use_ssl)
		{
			return connect_ssl();
		}

		return connect_plain();
	}

	std::expected<void, std::string> client::connect_plain()
	{
		timeval timeout{};
		timeout.tv_sec = static_cast<long>(configuration_.options.connect_timeout.count());
		timeout.tv_usec = 0;

		redis_context_ = redisConnectWithTimeout(
			configuration_.uri.host.c_str(),
			configuration_.uri.port,
			timeout);

		if (!redis_context_)
		{
			return std::unexpected(std::string("redisConnect returned null context"));
		}

		if (redis_context_->err)
		{
			const std::string error = redis_context_->errstr[0] != '\0' ? redis_context_->errstr : "unknown";

			redisFree(redis_context_);

			redis_context_ = nullptr;

			return std::unexpected(std::string("redisConnect failed: ") + error);
		}

		if (auto result = authenticate(); !result)
		{
			return result;
		}

		if (auto result = select_database(); !result)
		{
			return result;
		}

		LOG(INFO) << "Connected to Redis at " << connection_info();

		return {};
	}

	std::expected<void, std::string> client::connect_ssl()
	{
		if (!ssl_context_.has_value())
		{
			ssl_context_.emplace();

			if (auto result = ssl_context_->initialize(); !result)
			{
				ssl_context_.reset();
				return std::unexpected("Failed to initialize SSL: " + result.error());
			}
		}

		const timeval timeout{
			.tv_sec = static_cast<long>(configuration_.options.connect_timeout.count()),
			.tv_usec = 0
		};

		redis_context_ = redisConnectWithTimeout(
			configuration_.uri.host.c_str(),
			configuration_.uri.port,
			timeout);

		if (!redis_context_)
		{
			return std::unexpected(std::string("redisConnect returned null context"));
		}

		if (redis_context_->err)
		{
			const std::string error = redis_context_->errstr[0] != '\0' ? redis_context_->errstr : "unknown";

			redisFree(redis_context_);

			redis_context_ = nullptr;

			return std::unexpected(std::string("redisConnect failed: ") + error);
		}

		if (auto result = ssl_context_->apply_to_context(redis_context_); !result)
		{
			redisFree(redis_context_);
			redis_context_ = nullptr;
			return std::unexpected("SSL handshake failed: " + result.error());
		}

		if (auto result = authenticate(); !result)
		{
			return result;
		}

		if (auto result = select_database(); !result)
		{
			return result;
		}

		LOG(INFO) << "Connected to Redis (SSL) at " << connection_info();

		return {};
	}

	std::expected<void, std::string> client::authenticate()
	{
		if (configuration_.uri.password.empty())
		{
			return {};
		}

		const std::vector<std::string_view> auth_args = configuration_.uri.username.empty()
			                                                ? std::vector<std::string_view>{
				                                                "AUTH", configuration_.uri.password
			                                                }
			                                                : std::vector<std::string_view>{
				                                                "AUTH", configuration_.uri.username,
				                                                configuration_.uri.password
			                                                };

		const auto command_result = execute_command(auth_args);

		if (!command_result)
		{
			return std::unexpected(std::string("AUTH failed: ") + command_result.error());
		}

		const auto& reply = command_result.value();

		if (reply->type == REDIS_REPLY_ERROR)
		{
			const std::string error = reply->str ? reply->str : "AUTH error";
			return std::unexpected(std::string("AUTH error: ") + error);
		}

		return {};
	}

	std::expected<void, std::string> client::select_database()
	{
		if (configuration_.uri.database == 0)
		{
			return {};
		}

		const std::string db_str = std::to_string(configuration_.uri.database);
		const auto command_result = execute_command({"SELECT", db_str});

		if (!command_result)
		{
			return std::unexpected(std::string("SELECT failed: ") + command_result.error());
		}

		const auto& reply = command_result.value();

		if (reply->type == REDIS_REPLY_ERROR)
		{
			const std::string error = reply->str ? reply->str : "SELECT error";
			return std::unexpected(std::string("SELECT error: ") + error);
		}

		return {};
	}

	void client::disconnect()
	{
		if (redis_context_)
		{
			redisFree(redis_context_);
			redis_context_ = nullptr;
		}
	}

	/**
	 * @class redis_argv
	 * @brief Holds argv pointers and lengths for hiredis command API.
	 */
	struct redis_argv
	{
		/// Command argument pointers.
		std::vector<const char*> argv;
		/// Command argument byte lengths.
		std::vector<size_t> argv_len;
	};

	static redis_argv make_redis_argv(const std::vector<std::string_view>& arguments)
	{
		redis_argv out;

		out.argv.reserve(arguments.size());
		out.argv_len.reserve(arguments.size());

		for (const auto& s : arguments)
		{
			out.argv.emplace_back(s.data());
			out.argv_len.push_back(s.size());
		}

		return out;
	}

	std::expected<reply, std::string> client::execute_command(const std::vector<std::string_view>& arguments)
	{
		if (!redis_context_)
		{
			const auto connection_result = connect();

			if (!connection_result)
			{
				return std::unexpected(connection_result.error());
			}
		}

		auto [argv, argv_len] = make_redis_argv(arguments);

		const auto redis_reply = static_cast<redisReply*>(redisCommandArgv(
			redis_context_, static_cast<int>(argv.size()), argv.data(), argv_len.data()));

		if (!redis_reply)
		{
			if (redis_context_ && redis_context_->err)
			{
				std::ostringstream oss;

				oss << "redisCommandArgv null reply; ctx->err=" << redis_context_->err
					<< " (" << (redis_context_->errstr[0] != '\0' ? redis_context_->errstr : "") << ")";

				return std::unexpected(oss.str());
			}

			return std::unexpected(std::string("redisCommandArgv returned null (unknown)"));
		}
		return reply(redis_reply);
	}

	std::expected<reply, std::string> client::execute_with_retry(const std::vector<std::string_view>& args)
	{
		const auto is_retriable_error = [](const std::string_view error)
		{
			if (error.empty())
			{
				return true;
			}

			static constexpr std::string_view connection_signals[] =
			{
				"connection", "connect", "timeout", "refused", "reset", "closed", "broken pipe", "null reply", "ssl",
				"handshake"
			};

			return std::ranges::any_of(connection_signals, [error](const std::string_view signal)
			{
				return error.find(signal) != std::string_view::npos;
			});
		};

		std::string last_error;

		const int max_attempts = configuration_.options.max_retries + 1;

		for (int attempt = 1; attempt <= max_attempts; ++attempt)
		{
			auto result = execute_command(args);

			if (result)
			{
				return std::move(result);
			}

			last_error = result.error();

			if (!is_retriable_error(last_error))
			{
				LOG(ERROR) << "Non-retriable Redis error: " << last_error;

				return std::unexpected(last_error);
			}

			if (attempt == max_attempts)
			{
				LOG(ERROR) << "Redis command failed (final): " << last_error;

				return std::unexpected(last_error);
			}

			LOG(WARNING) << "Attempt " << attempt << " failed: " << last_error << " - attempting reconnect.";

			disconnect();

			const auto connect_result = connect();

			if (connect_result)
			{
				LOG(INFO) << "Reconnect succeeded; retrying immediately.";

				continue;
			}

			LOG(WARNING) << "Reconnect failed: " << connect_result.error() << " - backing off.";

			std::uniform_real_distribution jitter(1.0 - configuration_.options.backoff_jitter,
			                                      1.0 + configuration_.options.backoff_jitter);

			const double factor = jitter(rng_);

			const auto base_ms = configuration_.options.base_backoff.count() * (1ULL << (attempt - 1));

			const auto sleep_ms = std::chrono::milliseconds(static_cast<long long>(base_ms * factor));

			LOG(WARNING) << "Sleeping " << sleep_ms.count() << " ms before retry (attempt " << (attempt + 1) <<
				" of " << max_attempts << ")";

			std::this_thread::sleep_for(sleep_ms);
		}

		return std::unexpected(last_error.empty() ? "execute_with_retry: exhausted" : last_error);
	}
}
