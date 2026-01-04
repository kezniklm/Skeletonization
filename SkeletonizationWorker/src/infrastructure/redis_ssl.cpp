#include <openssl/ssl.h>

#include <glog/logging.h>

#include "SkeletonizationWorker/infrastructure/redis_ssl.hpp"

namespace worker::infrastructure::redis
{
	bool ssl_library_initializer::initialized_ = false;

	std::expected<void, std::string> ssl_library_initializer::initialize()
	{
		if (initialized_)
		{
			return {};
		}

		const auto result = redisInitOpenSSL();

		if (result != REDIS_OK)
		{
			return std::unexpected("Failed to initialize OpenSSL for hiredis");
		}

		initialized_ = true;

		LOG(INFO) << "Hiredis SSL library initialized";

		return {};
	}

	bool ssl_library_initializer::is_initialized() noexcept
	{
		return initialized_;
	}

	ssl_context::~ssl_context()
	{
		if (ssl_context_)
		{
			redisFreeSSLContext(ssl_context_);
			ssl_context_ = nullptr;
		}
	}

	ssl_context::ssl_context(ssl_context&& other) noexcept : ssl_context_(other.ssl_context_)
	{
		other.ssl_context_ = nullptr;
	}

	ssl_context& ssl_context::operator=(ssl_context&& other) noexcept
	{
		if (this != &other)
		{
			if (ssl_context_)
			{
				redisFreeSSLContext(ssl_context_);
			}

			ssl_context_ = other.ssl_context_;
			other.ssl_context_ = nullptr;
		}
		return *this;
	}

	std::expected<void, std::string> ssl_context::initialize(
		const char* ca_cert_path,
		const char* cert_path,
		const char* key_path)
	{
		if (ssl_context_)
		{
			redisFreeSSLContext(ssl_context_);
			ssl_context_ = nullptr;
		}

		const auto result = ssl_library_initializer::initialize();

		if (!result)
		{
			return std::unexpected(result.error());
		}

		redisSSLContextError ssl_error;

		redisSSLOptions ssl_options{
			.cacert_filename = ca_cert_path,
			.capath = nullptr,
			.cert_filename = cert_path,
			.private_key_filename = key_path,
			.server_name = nullptr,
			.verify_mode = SSL_VERIFY_NONE
		};

		ssl_context_ = redisCreateSSLContextWithOptions(&ssl_options, &ssl_error);

		if (!ssl_context_)
		{
			const auto error_str = redisSSLContextGetError(ssl_error);

			return std::unexpected(
				std::string("Failed to create SSL context: ") + (error_str ? error_str : "unknown error"));
		}

		LOG(INFO) << "Redis SSL context initialized";

		return {};
	}

	std::expected<void, std::string> ssl_context::apply_to_context(redisContext* ctx) const
	{
		if (!ctx)
		{
			return std::unexpected("Cannot apply SSL to null Redis context");
		}

		if (!ssl_context_)
		{
			return std::unexpected("SSL context not initialized");
		}

		const int result = redisInitiateSSLWithContext(ctx, ssl_context_);

		if (result != REDIS_OK)
		{
			std::string error = "Failed to initiate SSL handshake";
			if (ctx->err)
			{
				error += ": ";
				error += ctx->errstr[0] != '\0' ? ctx->errstr : "unknown error";
			}
			return std::unexpected(error);
		}

		return {};
	}

	bool ssl_context::is_initialized() const noexcept
	{
		return ssl_context_ != nullptr;
	}

	redisSSLContext* ssl_context::get() const noexcept
	{
		return ssl_context_;
	}
}
