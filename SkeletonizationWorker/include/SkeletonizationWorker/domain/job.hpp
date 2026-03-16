/**
*
* @file job.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares the domain job model.
 *
 * This file defines domain models representing worker jobs and image tasks.
 *
 * Main responsibilities:
 * - define job and task domain data structures
 * - capture algorithm and output format settings
 * - provide typed payload model for processing pipeline
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <vector>

#include "SkeletonizationWorker/domain/output_format.hpp"

namespace job
{
	using image_path = std::string;
	using algorithm_name = std::string;

	/**
	 * @class image_task
	 * @brief Represents one image-processing task within a job.
	 */
	struct image_task
	{
		/// Object-storage or local path to source image.
		image_path path;
		/// Algorithm identifier requested for this task.
		algorithm_name algorithm;
		/// Indicates whether preprocessing should run before thinning.
		bool should_run_preprocessing = true;
		/// Desired output image format.
		output_format format = output_format::png;
	};

	/**
	 * @class job
	 * @brief Represents one worker job containing multiple image tasks.
	 */
	struct job
	{
		/// Unique job identifier.
		std::string id;
		/// Task collection to process under this job.
		std::vector<image_task> tasks;
	};
}
