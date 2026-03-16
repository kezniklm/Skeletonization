/**
*
* @file aws_sdk_initializer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares AWS SDK lifecycle initialization helpers.
*
* This file defines RAII helper for AWS SDK startup and shutdown.
*
* Main responsibilities:
* - initialize AWS SDK options on construction
* - shutdown AWS SDK on destruction
* - provide non-copyable lifecycle guard
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <aws/core/Aws.h>

namespace worker::infrastructure
{
	/**
	 * @class aws_sdk_initializer
	 * @brief Manages AWS SDK lifecycle through RAII.
	 */
	class aws_sdk_initializer
	{
	public:
		/**
		 * @brief Constructs AWS SDK initializer.
		 */
		aws_sdk_initializer();
		/**
		 * @brief Destroys AWS SDK initializer.
		 */
		~aws_sdk_initializer();

		aws_sdk_initializer(const aws_sdk_initializer&) = delete;
		aws_sdk_initializer& operator=(const aws_sdk_initializer&) = delete;
		aws_sdk_initializer(aws_sdk_initializer&&) = delete;
		aws_sdk_initializer& operator=(aws_sdk_initializer&&) = delete;

	private:
		/// AWS SDK option storage.
		Aws::SDKOptions options_{};
	};
}
