#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "glog/logging.h"

#include "SkeletonizationCLI/benchmark/manager.hpp"
#include "SkeletonizationCLI/exceptions/benchmark_exception.hpp"

namespace skeletonization_benchmark
{
	void manager::register_all()
	{
		const auto configs = args_provider_->configuration_path().empty()
			                     ? config_loader_->load(args_provider_->skeletonizer_configurations())
			                     : config_loader_->load(args_provider_->configuration_path());

		configurations_ = configs;

		presenter_.set_configurations(configs);

		for (const auto& image_metadata : configs)
		{
			add_runner(image_metadata);
		}
	}

	std::string manager::run_all() const
	{
		if (registry_.empty())
		{
			throw cli::exceptions::no_benchmarks_registered_exception();
		}

		return executor_.run(registry_);
	}

	void manager::add_runner(const configuration::image_benchmark_metadata& image_metadata)
	{
		registry_.add(image_metadata);
	}

	void manager::delete_runner(const std::string& name)
	{
		registry_.remove(name);
	}

	void manager::show_results(const std::string& benchmark_json) const
	{
		if (benchmark_json.empty())
		{
			LOG(WARNING) << "Empty benchmark JSON provided.";
			return;
		}

		const auto visualizer_path = std::filesystem::path(VISUALIZER_ASSETS_DIR);
		const auto output_path = std::filesystem::path(args_provider_->benchmark_output_path());

		presenter_.present(registry_, benchmark_json, visualizer_path, output_path);
	}
}
