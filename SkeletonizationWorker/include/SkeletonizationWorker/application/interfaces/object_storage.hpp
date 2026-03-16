/**
*
* @file object_storage.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the object storage contract for worker artifacts.
 *
 * This file defines object storage interface for upload/download/remove flows.
 *
 * Main responsibilities:
 * - define object download contract
 * - define object upload contract
 * - define object removal and backend capability contract
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <string>

namespace worker::application::interfaces
{
	/**
	 * @class object_put_options
	 * @brief Represents optional metadata for upload operations.
	 */
	struct object_put_options
	{
		/// Optional content type for uploaded object.
		std::optional<std::string> content_type;
	};

	/**
	 * @class i_object_storage
	 * @brief Defines object storage operations used by the worker.
	 */
	class i_object_storage
	{
	public:
		/**
		 * @brief Destroys the object storage interface.
		 */
		virtual ~i_object_storage() = default;

		/**
		 * @brief Downloads an object into a local file path.
		 *
		 * @param key Source object key.
		 * @param destination_path Destination local path.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> download_to_file(const std::string& key,
		                                                          const std::filesystem::path& destination_path) = 0;
		/**
		 * @brief Uploads a local file as an object.
		 *
		 * @param source_path Source file path.
		 * @param key Destination object key.
		 * @param options Optional upload metadata.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
		                                                          const std::string& key,
		                                                          const object_put_options& options = {}) = 0;
		/**
		 * @brief Removes an object by key.
		 *
		 * @param key Object key to remove.
		 * @return Empty result on success or error message.
		 */
		virtual std::expected<void, std::string> remove(const std::string& key) = 0;

		/**
		 * @brief Indicates whether the storage backend is remote.
		 *
		 * @return True for remote backends, false for local backends.
		 */
		virtual bool is_remote_backend() const noexcept = 0;
	};
}
