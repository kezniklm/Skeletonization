module;

#include <memory>
#include <functional>
#include <map>
#include <string>

export module configuration:skeletonizer_configuration;

import skeletonizer_cpu;
import skeletonizer_gpu;
import skeletonizer;

using skeletonizer_creator = std::function<std::unique_ptr<skeletonizer::skeletonizer>()>;

using skeletonizer_map = std::map<skeletonizer::skeletonizer_type, skeletonizer_creator>;

export struct image_benchmark_metadata
{
	std::string name;
	std::string path;
	skeletonizer_map skeletonizers;
};

export inline std::vector<image_benchmark_metadata> skeletonizer_configuration = {
	{
		"Image 1",
		R"(C:\Users\matej.keznikl\OneDrive - Thermo Fisher Scientific\Documents\Master's Diploma Thesis\Image datasets\leafsnap-dataset\dataset\images\field\abies_concolor\12995307070714.jpg)",
		{
			{
				skeletonizer::skeletonizer_type::cpu, []
				{
					return std::make_unique<skeletonizer::cpu::algorithms::zhang_suen_cpu>();
				}
			},
			{
				skeletonizer::skeletonizer_type::gpu, []
				{
					return std::make_unique<skeletonizer::gpu::algorithms::zhang_suen_gpu>();
				}
			}
		}
	},
	{
		"Image 2",
		R"(C:\Users\matej.keznikl\OneDrive - Thermo Fisher Scientific\Documents\Master's Diploma Thesis\Image datasets\leafsnap-dataset\dataset\images\field\acer_ginnala\13291762511387.jpg)",
		{
			{
				skeletonizer::skeletonizer_type::cpu, []
				{
					return std::make_unique<skeletonizer::cpu::algorithms::zhang_suen_cpu>();
				}
			},
			{
				skeletonizer::skeletonizer_type::gpu, []
				{
					return std::make_unique<skeletonizer::gpu::algorithms::zhang_suen_gpu>();
				}
			}
		}
	}
};
