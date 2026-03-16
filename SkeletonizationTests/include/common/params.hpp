/**
*
* @file params.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines parametrization utilities for test case generation.
*
* This header provides helpers for building parameter pairs and readable test
* names for parametrized test suites.
*
* Main responsibilities:
* - define parameter tuple aliases
* - generate full parameter cartesian products
* - build readable parameterized test names
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once


#include <vector>
#include <string>
#include <sstream>

namespace skeltest
{
	/// Parameter pair containing triple index and image index.
	using param = std::pair<int, int>;

	/**
	 * @brief Creates all parameter pairs for triples and images.
	 *
	 * @param triple_count Number of algorithm triples.
	 * @param image_count Number of test images.
	 * @return Vector of parameter pairs.
	 */
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

	/**
	 * @brief Builds readable test parameter name.
	 *
	 * @param triple_name Triple name.
	 * @param image_idx Image index.
	 * @param w Image width.
	 * @param h Image height.
	 * @return Formatted parameter name.
	 */
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
