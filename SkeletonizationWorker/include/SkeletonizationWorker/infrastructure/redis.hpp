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
	struct reply_deleter
	{
		void operator()(redisReply* ptr) const
		{
			if (ptr)
			{
				freeReplyObject(ptr);
			}
		}
	};

	using reply = std::unique_ptr<redisReply, reply_deleter>;

	class client
	{
	public:
		explicit client(redis_configuration configuration);
		~client();

		client(client&&) noexcept = default;
		client& operator=(client&&) noexcept = default;
		client(const client&) = delete;
		client& operator=(const client&) = delete;

		std::expected<void, std::string> ensure_connected();
		std::expected<std::string, std::string> pop_safe(std::string_view list,
		                                                 std::string_view processing_key,
		                                                 std::chrono::seconds timeout);
		std::expected<void, std::string> acknowledge_job(std::string_view processing_key,
		                                                 std::string_view job_payload);
		std::expected<void, std::string> requeue(std::string_view list, std::string_view job_payload);
		std::expected<void, std::string> set(std::string_view key, std::string_view value);
		std::expected<std::string, std::string> get(std::string_view key);

		[[nodiscard]] std::string connection_info() const;

	private:
		redis_configuration configuration_;
		redisContext* redis_context_;
		std::optional<ssl_context> ssl_context_;
		std::mt19937_64 rng_;

		std::expected<void, std::string> connect();
		std::expected<void, std::string> connect_plain();
		std::expected<void, std::string> connect_ssl();
		std::expected<void, std::string> authenticate();
		std::expected<void, std::string> select_database();
		void disconnect();
		std::expected<reply, std::string> execute_command(const std::vector<std::string_view>& arguments);
		std::expected<reply, std::string> execute_with_retry(const std::vector<std::string_view>& args);
	};
}
