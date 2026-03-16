/**
*
* @file aws_sdk_initializer.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements AWS SDK initialization and shutdown routines.
*
* This file implements AWS SDK RAII lifecycle setup.
*
* Main responsibilities:
* - initialize AWS SDK on startup
* - release AWS SDK resources on shutdown
* - encapsulate SDK option configuration
*
* @version 1.0
* @date 2026-03-16
*/

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
