module;

#include <expected>
#include <random>
#include <hiredis/hiredis.h>
#include <glog/logging.h>

export module redis_client;

namespace redis
{
	export struct config
	{
		std::string host = "127.0.0.1";
		int port = 6379;
		int db = 0;
		std::string password;

		int max_retries = 3;
		std::chrono::milliseconds base_backoff{200}; // ms
		double backoff_jitter = 0.2; // fraction 0..1
	};

	struct reply
	{
		redisReply* redis_reply;

		explicit reply(redisReply* redis_reply = nullptr) : redis_reply(redis_reply)
		{
		}

		~reply()
		{
			if (redis_reply)
			{
				freeReplyObject(redis_reply);
			}
		}

		redisReply* get() const noexcept
		{
			return redis_reply;
		}

		explicit operator bool() const noexcept
		{
			return redis_reply != nullptr;
		}
	};

	export class client
	{
	public:
		explicit client(config cfg) : configuration_(std::move(cfg)), redis_context_(nullptr),
		                              rng_(std::random_device{}())
		{
			LOG(INFO) << "Redis connection created";
		}

		~client()
		{
			disconnect();

			LOG(INFO) << "Redis connection destroyed.";
		}

		client(client&&) noexcept = default;

		client& operator=(client&&) noexcept = default;

		client(const client&) = delete;

		client& operator=(const client&) = delete;

		std::expected<void, std::string> ensure_connected()
		{
			return connect();
		}

		std::expected<std::string, std::string> pop_safe(const std::string_view list,
		                                                 const std::string_view processing_key,
		                                                 const std::chrono::seconds timeout)
		{
			const std::vector<std::string_view> arguments = {
				"BRPOPLPUSH", list, processing_key, std::to_string(static_cast<int>(timeout.count()))
			};

			const auto reply_result = execute_with_retry(arguments);

			if (!reply_result)
			{
				return std::unexpected(reply_result.error());
			}

			const auto reply = reply_result.value().get();

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

		std::expected<void, std::string> acknowledge_job(const std::string_view processing_key,
		                                                 const std::string_view job_payload)
		{
			const std::vector<std::string_view> arguments = {"LREM", processing_key, "1", job_payload};

			const auto reply_result = execute_with_retry(arguments);

			if (!reply_result)
			{
				return std::unexpected(reply_result.error());
			}

			const auto reply = reply_result.value().get();

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

		std::expected<void, std::string> requeue(const std::string_view list, const std::string_view job_payload)
		{
			const std::vector<std::string_view> args = {"LPUSH", list, job_payload};

			const auto reply_result = execute_with_retry(args);

			if (!reply_result)
			{
				return std::unexpected(reply_result.error());
			}

			return {};
		}

		std::expected<void, std::string> set(const std::string_view key, const std::string_view value)
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

		std::expected<std::string, std::string> get(const std::string_view key)
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

	private:
		config configuration_;
		redisContext* redis_context_;
		std::mt19937_64 rng_;

		std::expected<void, std::string> connect()
		{
			if (redis_context_ && redis_context_->err == 0)
			{
				return {};
			}

			disconnect();

			redis_context_ = redisConnect(configuration_.host.c_str(), configuration_.port);


			if (!redis_context_)
			{
				return std::unexpected(std::string("redisConnect returned null context"));
			}

			if (redis_context_->err)
			{
				const std::string error = redis_context_->errstr ? redis_context_->errstr : "unknown";

				redisFree(redis_context_);

				redis_context_ = nullptr;

				return std::unexpected(std::string("redisConnect failed: ") + error);
			}

			if (!configuration_.password.empty())
			{
				const auto command_result = execute_command({
					"AUTH", configuration_.password
				});

				if (!command_result)
				{
					return std::unexpected(std::string("AUTH failed: ") + command_result.error());
				}

				const auto& reply = command_result.value();

				if (reply.get()->type == REDIS_REPLY_ERROR)
				{
					const std::string error = reply.get()->str ? reply.get()->str : "AUTH error";

					return std::unexpected(std::string("AUTH error: ") + error);
				}
			}

			if (configuration_.db != 0)
			{
				const auto command_result = execute_command({"SELECT", std::to_string(configuration_.db)});

				if (!command_result)
				{
					return std::unexpected(std::string("SELECT failed: ") + command_result.error());
				}

				const auto& reply = command_result.value();

				if (reply.get()->type == REDIS_REPLY_ERROR)
				{
					const std::string error = reply.get()->str ? reply.get()->str : "SELECT error";

					return std::unexpected(std::string("SELECT error: ") + error);
				}
			}

			LOG(INFO) << "Connected to Redis at " + configuration_.host + ":" + std::to_string(configuration_.port);

			return {};
		}

		void disconnect()
		{
			if (redis_context_)
			{
				redisFree(redis_context_);
				redis_context_ = nullptr;
			}
		}

		std::expected<reply, std::string> execute_command(const std::vector<std::string_view>& arguments)
		{
			if (!redis_context_)
			{
				const auto connection_result = connect();

				if (!connection_result)
				{
					return std::unexpected(connection_result.error());
				}
			}

			auto build_argv = [&arguments]
			{
				std::vector<std::string> owned_strings;
				owned_strings.reserve(arguments.size());

				std::vector<const char*> argv;
				argv.reserve(arguments.size());

				std::vector<size_t> argv_len;
				argv_len.reserve(arguments.size());

				for (std::string_view sv : arguments)
				{
					owned_strings.emplace_back(sv);
					const auto& s = owned_strings.back();
					argv.push_back(s.data());
					argv_len.push_back(s.size());
				}

				return std::make_tuple(std::move(owned_strings),
				                       std::move(argv),
				                       std::move(argv_len));
			};

			auto [owned_strings, argv, argv_len] = build_argv();

			const auto redis_reply = static_cast<redisReply*>(redisCommandArgv(
					redis_context_, static_cast<int>(argv.size()), argv.data(), argv_len.data())
			);

			if (!redis_reply)
			{
				if (redis_context_ && redis_context_->err)
				{
					std::ostringstream oss;

					oss << "redisCommandArgv null reply; ctx->err=" << redis_context_->err
						<< " (" << (redis_context_->errstr ? redis_context_->errstr : "") << ")";

					return std::unexpected(oss.str());
				}

				return std::unexpected(std::string("redisCommandArgv returned null (unknown)"));
			}
			return reply(redis_reply);
		}

		std::expected<reply, std::string> execute_with_retry(const std::vector<std::string_view>& args)
		{
			const auto is_retriable_error = [](const std::string_view error)
			{
				if (error.empty())
				{
					return true;
				}

				static constexpr std::string_view connection_signals[] =
				{
					"connection", "connect", "timeout", "refused", "reset", "closed", "broken pipe", "null reply"
				};

				for (const auto connection_signal : connection_signals)
				{
					if (error.find(connection_signal) != std::string_view::npos)
					{
						return true;
					}
				}
				return false;
			};

			std::string last_error;

			const int max_attempts = configuration_.max_retries + 1;

			for (int attempt = 1; attempt <= max_attempts; ++attempt)
			{
				const auto result = execute_command(args);

				if (result)
				{
					return result;
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

				LOG(WARNING) << "Attempt " << attempt << " failed: " << last_error << " — attempting reconnect.";

				disconnect();

				const auto connect_result = connect();

				if (connect_result)
				{
					LOG(INFO) << "Reconnect succeeded; retrying immediately.";

					continue;
				}

				LOG(WARNING) << "Reconnect failed: " << connect_result.error() << " — backing off.";

				std::uniform_real_distribution jitter(1.0 - configuration_.backoff_jitter,
				                                      1.0 + configuration_.backoff_jitter);

				const double factor = jitter(rng_);

				const auto base_ms = configuration_.base_backoff.count() * (1ULL << (attempt - 1));

				const auto sleep_ms = std::chrono::milliseconds(static_cast<long long>(base_ms * factor));

				LOG(WARNING) << "Sleeping " << sleep_ms.count() << " ms before retry (attempt " << (attempt + 1) <<
					" of " << max_attempts << ")";

				std::this_thread::sleep_for(sleep_ms);
			}

			return std::unexpected(last_error.empty()
				                       ? "execute_with_retry: exhausted"
				                       : last_error);
		}
	};
}
