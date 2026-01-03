#pragma once

#include <boost/di.hpp>

namespace worker::domain::dependency_injection
{
	namespace di = boost::di;

	inline auto make_installer()
	{
		return di::make_injector();
	}
}
