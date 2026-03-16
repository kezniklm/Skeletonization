/**
*
* @file redis_ssl.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares Redis TLS configuration helpers.
*
* This file defines Redis TLS context wrappers and SSL library initialization
* helpers.
*
* Main responsibilities:
* - manage Redis SSL context lifecycle
* - apply TLS context to Redis connections
* - initialize SSL library once per process
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <memory>
#include <string>

#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>

namespace worker::infrastructure::redis
{
	/**
	 * @class ssl_context
	 * @brief Wraps Redis SSL context lifecycle.
	 */
	class ssl_context
	{
	public:
		ssl_context() = default;
		/**
		 * @brief Releases owned Redis SSL context.
		 */
		~ssl_context();

		/**
		 * @brief Move-constructs SSL context wrapper.
		 * @param other Source SSL context wrapper.
		 */
		ssl_context(ssl_context&&) noexcept;
		/**
		 * @brief Move-assigns SSL context wrapper.
		 * @param other Source SSL context wrapper.
		 * @return Reference to current wrapper.
		 */
		ssl_context& operator=(ssl_context&&) noexcept;
		ssl_context(const ssl_context&) = delete;
		ssl_context& operator=(const ssl_context&) = delete;

		/**
		 * @brief Creates Redis SSL context with optional certificate paths.
		 * @param ca_cert_path CA certificate file path.
		 * @param cert_path Client certificate file path.
		 * @param key_path Client private key file path.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> initialize(
			const char* ca_cert_path = nullptr,
			const char* cert_path = nullptr,
			const char* key_path = nullptr);

		/**
		 * @brief Attaches SSL context to hiredis connection context.
		 * @param ctx Hiredis connection context.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> apply_to_context(redisContext* ctx) const;


		/**
		 * @brief Indicates whether SSL context is initialized.
		 * @return True when context pointer is initialized.
		 */
		[[nodiscard]] bool is_initialized() const noexcept;

		/**
		 * @brief Returns raw Redis SSL context pointer.
		 * @return Owned raw SSL context pointer.
		 */
		[[nodiscard]] redisSSLContext* get() const noexcept;

	private:
		/// Owned Redis SSL context pointer.
		redisSSLContext* ssl_context_ = nullptr;
	};

	/**
	 * @class ssl_library_initializer
	 * @brief Initializes shared SSL library state.
	 */
	class ssl_library_initializer
	{
	public:
		/**
		 * @brief Initializes global SSL library state once.
		 * @return Empty result on success or error message on failure.
		 */
		static std::expected<void, std::string> initialize();

		/**
		 * @brief Indicates whether SSL library has been initialized.
		 * @return True when SSL library initialization completed.
		 */
		static bool is_initialized() noexcept;

	private:
		ssl_library_initializer() = default;

		/// Indicates whether SSL library is already initialized.
		static bool initialized_;
	};
}
