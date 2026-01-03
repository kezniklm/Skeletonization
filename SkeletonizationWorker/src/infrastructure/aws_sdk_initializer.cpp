#include "SkeletonizationWorker/infrastructure/aws_sdk_initializer.hpp"

namespace worker::infrastructure
{
	aws_sdk_initializer::aws_sdk_initializer()
	{
		Aws::InitAPI(options_);
	}

	aws_sdk_initializer::~aws_sdk_initializer()
	{
		Aws::ShutdownAPI(options_);
	}
}
