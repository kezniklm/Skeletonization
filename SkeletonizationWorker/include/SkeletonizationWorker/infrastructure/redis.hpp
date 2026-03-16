/**
*
* @file redis.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares Redis infrastructure adapter abstractions.
*
* This file defines low-level Redis client abstractions and reply wrappers.
*
* Main responsibilities:
* - manage Redis reply lifetime wrappers
* - define resilient Redis client operations
* - provide queue and key-value Redis helpers
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <chrono>
#include <expected>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include <glog/logging.h>
#include <hiredis/hiredis.h>

#include "SkeletonizationWorker/infrastructure/redis_uri.hpp"
#include "SkeletonizationWorker/infrastructure/redis_ssl.hpp"

namespace worker::infrastructure::redis
{
	/**
	 * @class reply_deleter
	 * @brief Releases hiredis reply objects.
	 */
	struct reply_deleter
	{
		/**
		 * @brief Deletes hiredis reply object when present.
		 *
		 * @param ptr Reply pointer.
		 */
		void operator()(redisReply* ptr) const
		{
			if (ptr)
			{
				freeReplyObject(ptr);
			}
		}
	};

	/// Owning Redis reply wrapper.
	using reply = std::unique_ptr<redisReply, reply_deleter>;

	/**
	 * @class client
	 * @brief Implements Redis connectivity and command operations.
	 */
	class client
	{
	public:
		/**
		 * @brief Constructs Redis client from configuration.
		 * @param configuration Parsed Redis connection configuration.
		 */
		explicit client(redis_configuration configuration);
		/**
		 * @brief Destroys Redis client and releases connection.
		 */
		~client();

		client(client&&) noexcept = default;
		client& operator=(client&&) noexcept = default;
		client(const client&) = delete;
		client& operator=(const client&) = delete;

		/**
		 * @brief Ensures that Redis connection is established.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> ensure_connected();
		/**
		 * @brief Pops one item from queue with processing transfer semantics.
		 * @param list Source queue key.
		 * @param processing_key Processing queue key.
		 * @param timeout Pop timeout duration.
		 * @return Job payload on success or error message on failure.
		 */
		std::expected<std::string, std::string> pop_safe(std::string_view list,
		                                                 std::string_view processing_key,
		                                                 std::chrono::seconds timeout);
		/**
		 * @brief Acknowledges processed job payload.
		 * @param processing_key Processing queue key.
		 * @param job_payload Original job payload.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> acknowledge_job(std::string_view processing_key,
		                                                 std::string_view job_payload);
		/**
		 * @brief Requeues a job payload back to queue.
		 * @param list Target queue key.
		 * @param job_payload Job payload to requeue.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> requeue(std::string_view list, std::string_view job_payload);
		/**
		 * @brief Sets string value for key.
		 * @param key Redis key.
		 * @param value Redis value.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> set(std::string_view key, std::string_view value);
		/**
		 * @brief Gets string value for key.
		 * @param key Redis key.
		 * @return Key value on success or error message on failure.
		 */
		std::expected<std::string, std::string> get(std::string_view key);

		/**
		 * @brief Returns human-readable connection info.
		 *
		 * @return Connection info string.
		 */
		[[nodiscard]] std::string connection_info() const;

	private:
		/// Redis client configuration.
		redis_configuration configuration_;
		/// Active hiredis context.
		redisContext* redis_context_;
		/// Optional TLS context.
		std::optional<ssl_context> ssl_context_;
		/// Random generator for retry jitter.
		std::mt19937_64 rng_;

		/**
		 * @brief Establishes Redis connection and performs startup handshake.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> connect();
		/**
		 * @brief Establishes plain (non-TLS) Redis connection.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> connect_plain();
		/**
		 * @brief Establishes TLS Redis connection.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> connect_ssl();
		/**
		 * @brief Authenticates current Redis connection when credentials are configured.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> authenticate();
		/**
		 * @brief Selects configured Redis database index.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> select_database();
		/**
		 * @brief Closes active Redis connection context.
		 */
		void disconnect();
		/**
		 * @brief Executes one Redis command without retries.
		 * @param arguments Command arguments.
		 * @return Redis reply on success or error message on failure.
		 */
		std::expected<reply, std::string> execute_command(const std::vector<std::string_view>& arguments);
		/**
		 * @brief Executes one Redis command with retry strategy.
		 * @param args Command arguments.
		 * @return Redis reply on success or error message on failure.
		 */
		std::expected<reply, std::string> execute_with_retry(const std::vector<std::string_view>& args);
	};
}
