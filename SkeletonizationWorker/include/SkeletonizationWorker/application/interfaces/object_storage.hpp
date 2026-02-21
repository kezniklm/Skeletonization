#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <string>

namespace worker::application::interfaces
{
	struct object_put_options
	{
		std::optional<std::string> content_type;
	};

	class i_object_storage
	{
	public:
		virtual ~i_object_storage() = default;

		virtual std::expected<void, std::string> download_to_file(const std::string& key,
		                                                          const std::filesystem::path& destination_path) = 0;
		virtual std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
		                                                          const std::string& key,
		                                                          const object_put_options& options = {}) = 0;
		virtual std::expected<void, std::string> remove(const std::string& key) = 0;

		virtual bool is_remote_backend() const noexcept = 0;
	};
}
