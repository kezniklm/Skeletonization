#include "SkeletonizationWorker/infrastructure/local_object_storage.hpp"

#include <expected>
#include <filesystem>

namespace worker::infrastructure
{
	std::expected<void, std::string> local_object_storage::download_to_file(const std::string& key,
	                                                                        const std::filesystem::path&
	                                                                        destination_path)
	{
		std::error_code ec;

		std::filesystem::create_directories(destination_path.parent_path(), ec);
		std::filesystem::copy_file(key, destination_path, std::filesystem::copy_options::overwrite_existing, ec);

		if (ec)
		{
			return std::unexpected(
				"Failed to copy file: " + std::string{key} + " -> " + destination_path.string() + ": " + ec.message());
		}

		return {};
	}

	std::expected<void, std::string> local_object_storage::upload_from_file(const std::filesystem::path& source_path,
	                                                                        const std::string& key,
	                                                                        const
	                                                                        application::interfaces::object_put_options&
	                                                                        /*options*/)
	{
		std::error_code ec;

		const std::filesystem::path key_path{key};

		std::filesystem::create_directories(key_path.parent_path(), ec);

		std::filesystem::copy_file(source_path, key_path, std::filesystem::copy_options::overwrite_existing, ec);

		if (ec)
		{
			return std::unexpected("Failed to copy file: " + source_path.string() + " -> " + key + ": " + ec.message());
		}

		return {};
	}

	std::expected<void, std::string> local_object_storage::remove(const std::string& key)
	{
		std::error_code ec;

		std::filesystem::remove(std::filesystem::path{key}, ec);

		return {};
	}

	bool local_object_storage::is_remote_backend() const noexcept
	{
		return false;
	}
}
