/**
*
* @file benchmark_installer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares benchmark dependency injection installer.
 *
 * This file defines dependency injection wiring for benchmark services.
 *
 * Main responsibilities:
 * - register benchmark-layer service bindings
 * - compose benchmark runner factory dependencies
 * - install benchmark orchestration components
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <boost/di.hpp>

#include "SkeletonizationCLI/benchmark/default_runner_factory.hpp"
#include "SkeletonizationCLI/benchmark/exporter.hpp"
#include "SkeletonizationCLI/interfaces/i_exporter.hpp"
#include "SkeletonizationCLI/interfaces/i_image_loader.hpp"
#include "SkeletonizationCLI/interfaces/i_runner_factory.hpp"
#include "SkeletonizationCLI/interfaces/i_visualizer.hpp"
#include "SkeletonizationCLI/utils/opencv_image_loader.hpp"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"
#include "SkeletonizationCore/extensions/standard_image_processor.hpp"

namespace cli::dependency_injection
{
	namespace di = boost::di;

	inline auto make_benchmark_installer()
	{
		return di::make_injector(
			di::bind<interfaces::i_runner_factory>.to<skeletonization_benchmark::default_runner_factory>().in(
				di::singleton),
			di::bind<interfaces::i_exporter>.to<skeletonization_benchmark::exporter>().in(di::singleton),
			di::bind<interfaces::i_visualizer>.to<visual_inspector::visualiser>().in(di::singleton),
			di::bind<interfaces::i_image_loader>.to<utils::opencv_image_loader>().in(di::singleton),
			di::bind<standard_image_processor>.to<standard_image_processor>().in(di::singleton)
		);
	}
}
