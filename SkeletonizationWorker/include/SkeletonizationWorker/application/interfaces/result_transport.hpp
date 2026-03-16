/**
*
* @file result_transport.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the result transport contract.
 *
 * This file defines transport interface for serialized result payloads.
 *
 * Main responsibilities:
 * - define serialized result transport contract
 * - abstract transport backend implementation
 * - surface transport errors through expected values
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <string>

namespace worker::application::interfaces
{
	/**
	 * @class i_result_transport
	 * @brief Defines transport operations for serialized result payloads.
	 */
	class i_result_transport
	{
	public:
		/**
		 * @brief Destroys the result transport interface.
		 */
		virtual ~i_result_transport() = default;
		/**
		 * @brief Publishes a serialized result payload.
		 *
		 * @param result_payload Serialized result payload.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> publish_result(const std::string& result_payload) = 0;
	};
}
