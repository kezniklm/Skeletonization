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
	class s3_object_storage final : public application::interfaces::i_object_storage
	{
	public:
		s3_object_storage(const aws_sdk_initializer& aws,
		                  const configuration::s3_configuration& configuration);

		std::expected<void, std::string> download_to_file(const std::string& key,
		                                                  const std::filesystem::path& destination_path) override;
		std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
		                                                  const std::string& key,
		                                                  const application::interfaces::object_put_options&
		                                                  options) override;
		std::expected<void, std::string> remove(const std::string& key) override;

		bool is_remote_backend() const noexcept override;

	private:
		configuration::s3_configuration configuration_{};
		std::shared_ptr<Aws::S3::S3Client> client_;
	};
}
