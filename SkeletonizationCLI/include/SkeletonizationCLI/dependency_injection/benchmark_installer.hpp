#pragma once

#include <boost/di.hpp>

#include "SkeletonizationCLI/benchmark/default_runner_factory.hpp"
#include "SkeletonizationCLI/benchmark/exporter.hpp"
#include "SkeletonizationCLI/interfaces/i_exporter.hpp"
#include "SkeletonizationCLI/interfaces/i_image_loader.hpp"
#include "SkeletonizationCLI/interfaces/i_image_preprocessor.hpp"
#include "SkeletonizationCLI/interfaces/i_runner_factory.hpp"
#include "SkeletonizationCLI/interfaces/i_visualizer.hpp"
#include "SkeletonizationCLI/utils/opencv_image_loader.hpp"
#include "SkeletonizationCLI/utils/standard_image_preprocessor.hpp"
#include "SkeletonizationCLI/visual_inspector/visualiser.hpp"

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
			di::bind<interfaces::i_image_preprocessor>.to<utils::standard_image_preprocessor>().in(di::singleton)
		);
	}
}
