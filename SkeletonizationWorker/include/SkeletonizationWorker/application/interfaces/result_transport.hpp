#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	class i_result_transport
	{
	public:
		virtual ~i_result_transport() = default;
		virtual std::expected<void, std::string> publish_result(const std::string& result_payload) = 0;
	};
}
