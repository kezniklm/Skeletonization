#pragma once

#include <expected>
#include <memory>
#include <string>

#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>

namespace worker::infrastructure::redis
{
	class ssl_context
	{
	public:
		ssl_context() = default;
		~ssl_context();

		ssl_context(ssl_context&&) noexcept;
		ssl_context& operator=(ssl_context&&) noexcept;
		ssl_context(const ssl_context&) = delete;
		ssl_context& operator=(const ssl_context&) = delete;

		std::expected<void, std::string> initialize(
			const char* ca_cert_path = nullptr,
			const char* cert_path = nullptr,
			const char* key_path = nullptr);

		std::expected<void, std::string> apply_to_context(redisContext* ctx) const;


		[[nodiscard]] bool is_initialized() const noexcept;

		[[nodiscard]] redisSSLContext* get() const noexcept;

	private:
		redisSSLContext* ssl_context_ = nullptr;
	};

	class ssl_library_initializer
	{
	public:
		static std::expected<void, std::string> initialize();

		static bool is_initialized() noexcept;

	private:
		ssl_library_initializer() = default;

		static bool initialized_;
	};
}
