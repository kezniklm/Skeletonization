#pragma once

#include <chrono>
#include <expected>
#include <string>
#include <string_view>

namespace worker::infrastructure::redis
{
	struct redis_uri_components
	{
		std::string host = "localhost";
		int port = 6379;
		int database = 0;
		std::string username;
		std::string password;
		bool use_ssl = false;
	};

	class redis_uri_parser
	{
	public:
		static std::expected<redis_uri_components, std::string> parse(std::string_view uri);

	private:
		static std::expected<std::string, std::string> url_decode(std::string_view encoded);
		static std::expected<int, std::string> parse_port(std::string_view port_str);
		static std::expected<int, std::string> parse_database(std::string_view db_str);
	};

	struct redis_connection_options
	{
		int max_retries = 3;
		std::chrono::milliseconds base_backoff{200};
		double backoff_jitter = 0.2; // fraction 0..1
		std::chrono::seconds connect_timeout{5};
	};

	struct redis_configuration
	{
		redis_uri_components uri;
		redis_connection_options options;

		static std::expected<redis_configuration, std::string> from_url(
			std::string_view url, const redis_connection_options& options = {});
	};
}
