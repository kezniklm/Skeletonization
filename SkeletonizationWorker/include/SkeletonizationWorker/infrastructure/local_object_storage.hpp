#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include "SkeletonizationWorker/application/interfaces/object_storage.hpp"

namespace worker::infrastructure
{
	class local_object_storage final : public application::interfaces::i_object_storage
	{
	public:
		local_object_storage() = default;

		std::expected<void, std::string> download_to_file(const std::string& key,
		                                                  const std::filesystem::path& destination_path) override;
		std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
		                                                  const std::string& key,
		                                                  const application::interfaces::object_put_options&
		                                                  options) override;
		std::expected<void, std::string> remove(const std::string& key) override;

		bool is_remote_backend() const noexcept override;
	};
}
