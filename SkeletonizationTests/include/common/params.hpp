#pragma once


#include <vector>
#include <string>
#include <sstream>

namespace skeltest
{
	using param = std::pair<int, int>;

	inline std::vector<param> all_params(int triple_count, int image_count)
	{
		std::vector<param> params;

		for (int t = 0; t < triple_count; ++t)
		{
			for (int i = 0; i < image_count; ++i)
			{
				params.emplace_back(t, i);
			}
		}

		return params;
	}

	inline std::string param_name(const std::string& triple_name,
	                              int image_idx,
	                              int w,
	                              int h)
	{
		std::ostringstream o;
		o << triple_name << "_Img" << image_idx << "_" << w << "x" << h;
		return o.str();
	}
}
