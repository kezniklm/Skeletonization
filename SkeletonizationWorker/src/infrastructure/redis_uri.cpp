#include <charconv>
#include <cctype>

#include "SkeletonizationWorker/infrastructure/redis_uri.hpp"

namespace worker::infrastructure::redis
{
	std::expected<std::string, std::string> redis_uri_parser::url_decode(const std::string_view encoded)
	{
		std::string result;
		result.reserve(encoded.size());

		for (std::size_t i = 0; i < encoded.size(); ++i)
		{
			if (encoded[i] != '%')
			{
				if (encoded[i] == '+')
				{
					result += ' ';

					continue;
				}

				result += encoded[i];

				continue;
			}

			if (i + 2 >= encoded.size())
			{
				return std::unexpected("Invalid percent encoding in URI");
			}

			const char high = encoded[i + 1];
			const char low = encoded[i + 2];

			auto hex_value = [](const char c) -> int
			{
				if (c >= '0' && c <= '9')
				{
					return c - '0';
				}
				if (c >= 'a' && c <= 'f')
				{
					return c - 'a' + 10;
				}
				if (c >= 'A' && c <= 'F')
				{
					return c - 'A' + 10;
				}
				return -1;
			};

			const int h = hex_value(high);
			const int l = hex_value(low);

			if (h < 0 || l < 0)
			{
				return std::unexpected("Invalid hex digits in percent encoding");
			}

			result += static_cast<char>((h << 4) | l);

			i += 2;
		}

		return result;
	}

	std::expected<int, std::string> redis_uri_parser::parse_port(const std::string_view port_str)
	{
		int port = 0;
		const auto [ptr, ec] = std::from_chars(port_str.data(), port_str.data() + port_str.size(), port);

		if (ec != std::errc{} || ptr != port_str.data() + port_str.size())
		{
			return std::unexpected("Invalid port number: " + std::string(port_str));
		}

		if (port < 1 || port > 65535)
		{
			return std::unexpected("Port number out of range (1-65535): " + std::to_string(port));
		}

		return port;
	}

	std::expected<int, std::string> redis_uri_parser::parse_database(const std::string_view db_str)
	{
		int db = 0;
		const auto [ptr, ec] = std::from_chars(db_str.data(), db_str.data() + db_str.size(), db);

		if (ec != std::errc{} || ptr != db_str.data() + db_str.size())
		{
			return std::unexpected("Invalid database number: " + std::string(db_str));
		}

		if (db < 0 || db > 15)
		{
			return std::unexpected("Database number out of range (0-15): " + std::to_string(db));
		}

		return db;
	}

	std::expected<redis_uri_components, std::string> redis_uri_parser::parse(const std::string_view uri)
	{
		redis_uri_components components;

		std::string_view remaining = uri;

		constexpr std::string_view redis_scheme = "redis://";
		constexpr std::string_view rediss_scheme = "rediss://";

		if (remaining.starts_with(rediss_scheme))
		{
			components.use_ssl = true;
			remaining.remove_prefix(rediss_scheme.size());
		}
		else if (remaining.starts_with(redis_scheme))
		{
			components.use_ssl = false;
			remaining.remove_prefix(redis_scheme.size());
		}
		else
		{
			return std::unexpected("Invalid Redis URI scheme. Expected 'redis://' or 'rediss://'");
		}

		const auto at_pos = remaining.find('@');

		if (at_pos != std::string_view::npos)
		{
			const std::string_view credentials = remaining.substr(0, at_pos);

			const auto colon_pos = credentials.find(':');

			if (colon_pos == std::string_view::npos)
			{
				if (auto decoded = url_decode(credentials))
				{
					components.password = std::move(decoded.value());
				}
				else
				{
					return std::unexpected(decoded.error());
				}
			}
			else
			{
				const std::string_view username_encoded = credentials.substr(0, colon_pos);
				const std::string_view password_encoded = credentials.substr(colon_pos + 1);

				if (auto decoded = url_decode(username_encoded))
				{
					components.username = std::move(decoded.value());
				}
				else
				{
					return std::unexpected(decoded.error());
				}

				if (auto decoded = url_decode(password_encoded))
				{
					components.password = std::move(decoded.value());
				}
				else
				{
					return std::unexpected(decoded.error());
				}
			}

			remaining.remove_prefix(at_pos + 1);
		}

		std::string_view host_port;
		const auto slash_pos = remaining.find('/');

		if (slash_pos != std::string_view::npos)
		{
			host_port = remaining.substr(0, slash_pos);
			remaining.remove_prefix(slash_pos + 1);
		}
		else
		{
			host_port = remaining;
			remaining = {};
		}

		if (host_port.starts_with('['))
		{
			const auto bracket_end = host_port.find(']');
			if (bracket_end == std::string_view::npos)
			{
				return std::unexpected("Invalid IPv6 address: missing closing bracket");
			}

			components.host = std::string(host_port.substr(1, bracket_end - 1));

			if (bracket_end + 1 < host_port.size())
			{
				if (host_port[bracket_end + 1] != ':')
				{
					return std::unexpected("Invalid character after IPv6 address");
				}

				const std::string_view port_str = host_port.substr(bracket_end + 2);
				if (auto port = parse_port(port_str))
				{
					components.port = port.value();
				}
				else
				{
					return std::unexpected(port.error());
				}
			}
		}
		else
		{
			const auto colon_pos = host_port.rfind(':');
			if (colon_pos != std::string_view::npos)
			{
				components.host = std::string(host_port.substr(0, colon_pos));

				const std::string_view port_str = host_port.substr(colon_pos + 1);
				if (auto port = parse_port(port_str))
				{
					components.port = port.value();
				}
				else
				{
					return std::unexpected(port.error());
				}
			}
			else
			{
				components.host = std::string(host_port);
			}
		}

		if (components.host.empty())
		{
			return std::unexpected("Empty host in Redis URI");
		}

		if (!remaining.empty())
		{
			const auto query_pos = remaining.find('?');
			const std::string_view db_str = (query_pos != std::string_view::npos)
				                                ? remaining.substr(0, query_pos)
				                                : remaining;

			if (!db_str.empty())
			{
				if (auto db = parse_database(db_str))
				{
					components.database = db.value();
				}
				else
				{
					return std::unexpected(db.error());
				}
			}
		}

		return components;
	}

	std::expected<redis_configuration, std::string> redis_configuration::from_url(
		const std::string_view url,
		const redis_connection_options& options)
	{
		auto components = redis_uri_parser::parse(url);

		if (!components)
		{
			return std::unexpected(components.error());
		}

		return redis_configuration{
			.uri = std::move(components.value()),
			.options = options
		};
	}
}
