/**
*
* @file redis_uri.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares Redis URI parsing helpers.
*
* This file defines Redis URI parser and configuration models used by
* worker Redis infrastructure.
*
* Main responsibilities:
* - parse Redis URI strings into typed components
* - define reconnect and timeout option models
* - build Redis configuration from URI and options
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <chrono>
#include <expected>
#include <string>
#include <string_view>

namespace worker::infrastructure::redis
{
	/**
	 * @class redis_uri_components
	 * @brief Stores parsed Redis URI fields.
	 */
	struct redis_uri_components
	{
		/// Redis hostname.
		std::string host = "localhost";
		/// Redis TCP port.
		int port = 6379;
		/// Redis logical database index.
		int database = 0;
		/// Redis username when ACL auth is used.
		std::string username;
		/// Redis password value.
		std::string password;
		/// Indicates whether SSL should be used.
		bool use_ssl = false;
	};

	/**
	 * @class redis_uri_parser
	 * @brief Parses Redis URIs into component structures.
	 */
	class redis_uri_parser
	{
	public:
		/**
		 * @brief Parses Redis URI string into typed components.
		 * @param uri Redis URI value.
		 * @return Parsed URI components or validation error message.
		 */
		static std::expected<redis_uri_components, std::string> parse(std::string_view uri);

	private:
		/**
		 * @brief URL-decodes one URI segment.
		 * @param encoded Encoded URI segment.
		 * @return Decoded string or error message.
		 */
		static std::expected<std::string, std::string> url_decode(std::string_view encoded);
		/**
		 * @brief Parses TCP port value from URI.
		 * @param port_str Port token.
		 * @return Parsed port number or error message.
		 */
		static std::expected<int, std::string> parse_port(std::string_view port_str);
		/**
		 * @brief Parses database index from URI path segment.
		 * @param db_str Database token.
		 * @return Parsed database index or error message.
		 */
		static std::expected<int, std::string> parse_database(std::string_view db_str);
	};

	/**
	 * @class redis_connection_options
	 * @brief Stores Redis reconnection and timeout options.
	 */
	struct redis_connection_options
	{
		/// Maximum command retry attempts.
		int max_retries = 3;
		/// Base reconnect backoff.
		std::chrono::milliseconds base_backoff{200};
		/// Randomized backoff jitter ratio.
		double backoff_jitter = 0.2; // fraction 0..1
		/// Connect timeout threshold.
		std::chrono::seconds connect_timeout{5};
	};

	/**
	 * @class redis_configuration
	 * @brief Stores Redis URI and connection option configuration.
	 */
	struct redis_configuration
	{
		/// Parsed URI components.
		redis_uri_components uri;
		/// Runtime reconnect options.
		redis_connection_options options;

		/**
		 * @brief Builds Redis configuration from URL and connection options.
		 * @param url Redis URL value.
		 * @param options Connection options.
		 * @return Redis configuration or parse error message.
		 */
		static std::expected<redis_configuration, std::string> from_url(
			std::string_view url, const redis_connection_options& options = {});
	};
}
