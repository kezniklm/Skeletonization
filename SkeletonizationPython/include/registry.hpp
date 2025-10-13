#pragma once

#include <string>
#include <vector>
#include <utility>
#include <pybind11/pybind11.h>

namespace skelpy
{
	struct algorithm_info
	{
		std::string id; // C++ class name (or whatever you choose)
		std::string py; // Python callable name
		std::string display; // Human label
		std::string description; // Optional
		int fg{1}, sk{1}, bg{0};
		std::vector<std::string> tags;

		algorithm_info() = default;

		algorithm_info(std::string id,
		               std::string py,
		               std::string display,
		               std::string description,
		               const int fg = 1, const int sk = 1, const int bg = 0)
			: id(std::move(id)),
			  py(std::move(py)),
			  display(std::move(display)),
			  description(std::move(description)),
			  fg(fg), sk(sk), bg(bg)
		{
		}
	};

	inline std::vector<algorithm_info>& registry()
	{
		static std::vector<algorithm_info> v;

		return v;
	}

	inline void add_algo(const std::string_view id,
	                     const std::string_view py,
	                     const std::string_view display,
	                     const std::string_view description = {},
	                     int fg = 1, int sk = 1, int bg = 0)
	{
		registry().emplace_back(std::string{id}, std::string{py},
		                        std::string{display}, std::string{description},
		                        fg, sk, bg);
	}

	inline void bind_discovery(pybind11::module_& module)
	{
		module.def("algorithms", []
		{
			pybind11::list output_list;

			for (const auto& [id,
				     py,
				     display,
				     description,
				     fg,
				     sk,
				     bg,
				     tags] : registry())
			{
				pybind11::dict dictionary;

				dictionary["id"] = id;
				dictionary["py"] = py;
				dictionary["display"] = display;
				dictionary["description"] = description;
				dictionary["fg"] = fg;
				dictionary["sk"] = sk;
				dictionary["bg"] = bg;
				dictionary["tags"] = tags;
				output_list.append(std::move(dictionary));
			}
			return output_list;
		}, "Return metadata for all registered algorithms.");
	}
}
