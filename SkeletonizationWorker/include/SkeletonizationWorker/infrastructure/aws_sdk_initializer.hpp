#pragma once

#include <aws/core/Aws.h>

namespace worker::infrastructure
{
	class aws_sdk_initializer
	{
	public:
		aws_sdk_initializer();
		~aws_sdk_initializer();

		aws_sdk_initializer(const aws_sdk_initializer&) = delete;
		aws_sdk_initializer& operator=(const aws_sdk_initializer&) = delete;
		aws_sdk_initializer(aws_sdk_initializer&&) = delete;
		aws_sdk_initializer& operator=(aws_sdk_initializer&&) = delete;

	private:
		Aws::SDKOptions options_{};
	};
}
