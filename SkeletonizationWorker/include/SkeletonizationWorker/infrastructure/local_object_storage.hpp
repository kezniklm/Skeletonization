/**
*
* @file local_object_storage.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares local filesystem object storage implementation.
*
* This file defines local filesystem object storage implementation used
* by worker when remote storage is disabled.
*
* Main responsibilities:
* - download objects to local files
* - upload local files to object keys
* - remove local object files by key
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include "SkeletonizationWorker/application/interfaces/object_storage.hpp"

namespace worker::infrastructure
{
	/**
	 * @class local_object_storage
	 * @brief Implements local filesystem object storage backend.
	 */
	class local_object_storage final : public application::interfaces::i_object_storage
	{
	public:
		/**
		 * @brief Constructs local object storage backend.
		 */
		local_object_storage() = default;

		/**
		 * @brief Downloads object key into destination file.
		 * @param key Object key or relative path identifier.
		 * @param destination_path Local destination file path.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> download_to_file(const std::string& key,
		                                                  const std::filesystem::path& destination_path) override;
		/**
		 * @brief Uploads local source file under object key.
		 * @param source_path Local source file path.
		 * @param key Object key or relative path identifier.
		 * @param options Object write options.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
		                                                  const std::string& key,
		                                                  const application::interfaces::object_put_options&
		                                                  options) override;
		/**
		 * @brief Removes object by key.
		 * @param key Object key or relative path identifier.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> remove(const std::string& key) override;

		/**
		 * @brief Indicates local backend type.
		 *
		 * @return Always false for remote backend check.
		 */
		bool is_remote_backend() const noexcept override;
	};
}
