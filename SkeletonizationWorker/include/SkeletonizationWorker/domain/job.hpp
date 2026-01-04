#pragma once

#include <string>
#include <vector>

#include "SkeletonizationWorker/domain/output_format.hpp"

namespace job
{
	using image_path = std::string;
	using algorithm_name = std::string;

	struct image_task
	{
		image_path path;
		algorithm_name algorithm;
		bool should_run_preprocessing = true;
		output_format format = output_format::png;
	};

	struct job
	{
		std::string id;
		std::vector<image_task> tasks;
	};
}
