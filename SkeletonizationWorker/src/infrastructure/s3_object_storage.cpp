/**
*
* @file s3_object_storage.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements S3-backed object storage operations.
*
* This file implements S3 object download, upload, and removal behavior.
*
* Main responsibilities:
* - download objects from S3 to local files
* - upload local files to S3 object keys
* - delete S3 objects when requested
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationWorker/infrastructure/s3_object_storage.hpp"

#include <fstream>

#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/client/ClientConfiguration.h>
#include <string>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>

namespace worker::infrastructure
{
	namespace
	{
		/**
		 * @class parsed_endpoint
		 * @brief Parsed endpoint host and scheme used for S3 client setup.
		 */
		struct parsed_endpoint
		{
			/// Endpoint hostname without URL scheme.
			Aws::String host;
			/// Endpoint HTTP/HTTPS scheme.
			Aws::Http::Scheme scheme = Aws::Http::Scheme::HTTPS;
		};

		parsed_endpoint parse_endpoint(const std::string& endpoint)
		{
			parsed_endpoint result{};
			std::string value = endpoint;
			if (value.starts_with("http://"))
			{
				result.scheme = Aws::Http::Scheme::HTTP;
				value.erase(0, std::string{"http://"}.size());
			}
			else if (value.starts_with("https://"))
			{
				result.scheme = Aws::Http::Scheme::HTTPS;
				value.erase(0, std::string{"https://"}.size());
			}

			while (!value.empty() && value.back() == '/')
			{
				value.pop_back();
			}

			result.host = Aws::String{value.c_str()};
			return result;
		}
	}

	s3_object_storage::s3_object_storage(const aws_sdk_initializer& /*aws*/,
	                                     const configuration::s3_configuration& configuration)
		: configuration_(configuration)
	{
		Aws::Client::ClientConfiguration client_configuration;
		client_configuration.region = Aws::String{configuration_.region.c_str()};

		const auto endpoint = parse_endpoint(configuration_.endpoint);
		client_configuration.endpointOverride = endpoint.host;
		client_configuration.scheme = endpoint.scheme;

		Aws::Auth::AWSCredentials credentials(Aws::String{configuration_.access_key_id},
		                                      Aws::String{configuration_.secret_access_key});

		const bool use_virtual_addressing = !configuration_.force_path_style;

		client_ = Aws::MakeShared<Aws::S3::S3Client>(
			"s3_object_storage",
			credentials,
			client_configuration,
			Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
			use_virtual_addressing);
	}

	std::expected<void, std::string> s3_object_storage::download_to_file(const std::string& key,
	                                                                     const std::filesystem::path& destination_path)
	{
		Aws::S3::Model::GetObjectRequest request;
		request.SetBucket(Aws::String{configuration_.bucket.c_str()});
		request.SetKey(Aws::String{key.c_str()});

		auto outcome = client_->GetObject(request);

		if (!outcome.IsSuccess())
		{
			return std::unexpected(outcome.GetError().GetMessage().c_str());
		}

		std::error_code ec;
		std::filesystem::create_directories(destination_path.parent_path(), ec);

		std::ofstream output(destination_path, std::ios::binary | std::ios::trunc);
		if (!output.is_open())
		{
			return std::unexpected("Failed to open file for writing: " + destination_path.string());
		}

		output << outcome.GetResult().GetBody().rdbuf();

		if (!output.good())
		{
			return std::unexpected("Failed to write downloaded object to: " + destination_path.string());
		}

		return {};
	}

	std::expected<void, std::string> s3_object_storage::upload_from_file(
		const std::filesystem::path& source_path,
		const std::string& key,
		const application::interfaces::object_put_options& options)
	{
		Aws::S3::Model::PutObjectRequest request;
		request.SetBucket(Aws::String{configuration_.bucket.c_str()});
		request.SetKey(Aws::String{key.c_str()});
		if (options.content_type.has_value())
		{
			request.SetContentType(Aws::String{options.content_type.value().c_str()});
		}

		const auto input_data = Aws::MakeShared<Aws::FStream>(
			"put_object_body",
			source_path.string().c_str(),
			std::ios_base::in | std::ios_base::binary);

		if (!input_data->good())
		{
			return std::unexpected("Failed to open file for upload: " + source_path.string());
		}

		request.SetBody(input_data);

		const auto outcome = client_->PutObject(request);

		if (!outcome.IsSuccess())
		{
			return std::unexpected(outcome.GetError().GetMessage().c_str());
		}

		return {};
	}

	std::expected<void, std::string> s3_object_storage::remove(const std::string& key)
	{
		Aws::S3::Model::DeleteObjectRequest request;
		request.SetBucket(Aws::String{configuration_.bucket.c_str()});
		request.SetKey(Aws::String{key.c_str()});

		const auto outcome = client_->DeleteObject(request);

		if (!outcome.IsSuccess())
		{
			return std::unexpected(outcome.GetError().GetMessage().c_str());
		}

		return {};
	}

	bool s3_object_storage::is_remote_backend() const noexcept
	{
		return true;
	}
}
