/**
*
* @file configuration.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements worker configuration parsing and normalization.
*
* This file implements loading and normalization of worker configuration.
*
* Main responsibilities:
* - load environment values into configuration model
* - normalize and validate runtime settings
* - construct typed dependency injection values
*
* @version 1.0
* @date 2026-03-16
*/

#include <algorithm>
#include <fstream>
#include <ranges>
#include <iostream>
#include <filesystem>

#include "SkeletonizationWorker/configuration.hpp"

namespace worker::configuration
{
	void env_loader::load() const
	{
		std::ifstream file(std::string{path_});

		if (!file.is_open())
		{
			return;
		}

		auto trim = [](std::string& s)
		{
			auto is_space = [](const unsigned char c) { return std::isspace(c) != 0; };

			const auto it_begin = std::ranges::find_if_not(s, is_space);
			if (it_begin == s.end())
			{
				s.clear();
				return;
			}

			const auto it_end = std::ranges::find_if_not(std::ranges::reverse_view(s), is_space).base();
			s.assign(it_begin, it_end);
		};

		std::string line;

		while (std::getline(file, line))
		{
			trim(line);
			if (line.empty() || line[0] == '#' || line[0] == ';')
			{
				continue;
			}

			constexpr std::string_view export_prefix = "export ";

			if (line.starts_with(export_prefix))
			{
				line.erase(0, export_prefix.size());
				trim(line);
			}

			const auto pos = line.find('=');
			if (pos == std::string::npos)
			{
				continue;
			}

			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);

			trim(key);
			trim(value);

			if (key.empty())
			{
				continue;
			}

			if (value.size() >= 2)
			{
				const char first = value.front();
				const char last = value.back();

				if ((first == '"' && last == '"') || (first == '\'' && last == '\''))
				{
					value = value.substr(1, value.size() - 2);
				}
			}

#ifdef _WIN32
			_putenv_s(key.c_str(), value.c_str());
#else
			constexpr int shouldOverwrite = 1;
			::setenv(key.c_str(), value.c_str(), shouldOverwrite);
#endif
		}
	}

	inline void get_storage_backend(configuration& config)
	{
		const auto backend = get_env<backend::storage_backend>("STORAGE_BACKEND");

		if (backend != backend::storage_backend::s3)
		{
			config.backend = storage_backend::local;

			return;
		}

		config.backend = storage_backend::s3;
		config.s3.endpoint = get_env<std::string>("S3_ENDPOINT");
		config.s3.region = get_env_or<std::string>("S3_REGION", "auto");
		config.s3.bucket = get_env<std::string>("S3_BUCKET");
		config.s3.access_key_id = get_env<std::string>("S3_ACCESS_KEY_ID");
		config.s3.secret_access_key = get_env<std::string>("S3_SECRET_ACCESS_KEY");
		config.s3.force_path_style = get_env_or<int>("S3_FORCE_PATH_STYLE", 1) != 0;
	}

	configuration configuration::from_environment(const env_loader& loader)
	{
		loader.load();

		configuration configuration{
			.worker_id = get_env<std::string>("WORKER_ID"),
			.redis_url = get_env<std::string>("REDIS_URL"),
			.jobs_queue = get_env<std::string>("JOBS_QUEUE"),
			.processing_queue = get_env<std::string>("PROCESSING_QUEUE"),
			.results_queue = get_env<std::string>("RESULTS_QUEUE"),
			.output_directory = get_env<std::string>("OUTPUT_DIRECTORY"),
			.processor_type = get_env<skeletonizer::skeletonizer_type>("PROCESSOR_TYPE"),
		};

		get_storage_backend(configuration);

		return configuration;
	}
}
