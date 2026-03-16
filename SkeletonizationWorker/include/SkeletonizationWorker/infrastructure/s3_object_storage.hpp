/**
*
* @file s3_object_storage.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares S3-backed object storage implementation.
*
* This file defines S3 object storage backend implementation for worker
* artifacts.
*
* Main responsibilities:
* - download objects from S3 to local files
* - upload local files to S3 keys
* - remove S3 objects by key
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <expected>
#include <filesystem>
#include <memory>
#include <string>

#include <aws/s3/S3Client.h>

#include "SkeletonizationWorker/application/interfaces/object_storage.hpp"
#include "SkeletonizationWorker/configuration.hpp"
#include "SkeletonizationWorker/infrastructure/aws_sdk_initializer.hpp"

namespace worker::infrastructure
{
	/**
	 * @class s3_object_storage
	 * @brief Implements S3 object storage backend operations.
	 */
	class s3_object_storage final : public application::interfaces::i_object_storage
	{
	public:
		/**
		 * @brief Constructs S3 object storage backend.
		 * @param aws Initialized AWS SDK lifetime guard.
		 * @param configuration S3 backend configuration values.
		 */
		s3_object_storage(const aws_sdk_initializer& aws,
		                  const configuration::s3_configuration& configuration);

		/**
		 * @brief Downloads object key to local file path.
		 * @param key Object key to download.
		 * @param destination_path Local destination file path.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> download_to_file(const std::string& key,
		                                                  const std::filesystem::path& destination_path) override;
		/**
		 * @brief Uploads local file to object key.
		 * @param source_path Local source file path.
		 * @param key Destination object key.
		 * @param options Object upload options.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
		                                                  const std::string& key,
		                                                  const application::interfaces::object_put_options&
		                                                  options) override;
		/**
		 * @brief Removes object by key from backend.
		 * @param key Object key to remove.
		 * @return Empty result on success or error message on failure.
		 */
		std::expected<void, std::string> remove(const std::string& key) override;

		/**
		 * @brief Indicates whether backend is remote.
		 * @return Always true for S3 backend.
		 */
		bool is_remote_backend() const noexcept override;

	private:
		/// S3 configuration values.
		configuration::s3_configuration configuration_{};
		/// Shared S3 client instance.
		std::shared_ptr<Aws::S3::S3Client> client_;
	};
}
