#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "runner.hpp"

#include "SkeletonizationCore/configuration/types.hpp"

namespace skeletonization_benchmark
{
	class manager
	{
	public:
		void register_all();

		std::string run_all();

		void add_runner(const configuration::image_benchmark_metadata& image_metadata);

		void delete_runner(const std::string& name);

		void show_results(const std::string& benchmark_json) const;

	private:
		std::vector<std::pair<std::string, std::unique_ptr<runner>>> runners_;
		std::vector<configuration::image_benchmark_metadata> configurations_;
	};
}
