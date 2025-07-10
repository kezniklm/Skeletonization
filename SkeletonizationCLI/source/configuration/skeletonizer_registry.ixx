module;

#include <memory>
#include <functional>
#include <map>
#include <string>

export module configuration:skeletonizer_configuration;

import skeletonizer_cpu;
import skeletonizer_gpu;
import skeletonizer;

using skeletonizer_creator = std::function<std::unique_ptr<skeletonizer>()>;

using skeletonizer_map = std::map<skeletonizer_type, skeletonizer_creator>;

export struct image_benchmark_metadata
{
	std::string name;
	std::string path;
	skeletonizer_map skeletonizers;
};

export inline std::vector<image_benchmark_metadata> skeletonizer_configuration = {
	{"Image 1", R"(C:\Users\matej.keznikl\OneDrive - Thermo Fisher Scientific\Documents\Master's Diploma Thesis\Image datasets\leafsnap-dataset\dataset\images\field\abies_concolor\12995307070714.jpg)", {{skeletonizer_type::cpu, []
										{ return std::make_unique<skeletonizer_cpu>(); }},
									   {skeletonizer_type::gpu, []
										{ return std::make_unique<skeletonizer_gpu>(); }}}},
	{"Image 2", R"(C:\Users\matej.keznikl\OneDrive - Thermo Fisher Scientific\Documents\Master's Diploma Thesis\Image datasets\leafsnap-dataset\dataset\images\field\acer_ginnala\13291762511387.jpg)", {{skeletonizer_type::cpu, []
										{ return std::make_unique<skeletonizer_cpu>(); }},
									   {skeletonizer_type::gpu, []
										{ return std::make_unique<skeletonizer_gpu>(); }}}}};
