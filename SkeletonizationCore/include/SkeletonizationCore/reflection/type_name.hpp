/**
*
* @file type_name.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Provides compile-time extraction of type names.
*
* This header exposes a constexpr utility that extracts a readable type name
* from compiler-specific function signature strings.
*
* Main responsibilities:
* - extract type names at compile time
* - support compiler-specific signature formats
* - provide a constexpr reflection utility for templates
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <cstddef>
#include <string_view>

#if defined(__clang__)
/**
 * @brief Returns the compile-time type name of T.
 *
 * @return Type name extracted from compiler signature metadata.
 */
template <class T>
constexpr std::string_view type_name()
{
	constexpr std::string_view p = __PRETTY_FUNCTION__;

	constexpr auto a = p.find("T = ") + 4;

	constexpr auto b = p.rfind(']');

	return p.substr(a, b - a);
}
#elif defined(__GNUC__)
/**
 * @brief Returns the compile-time type name of T.
 *
 * @return Type name extracted from compiler signature metadata.
 */
template <class T>
constexpr std::string_view type_name()
{
	constexpr std::string_view p = __PRETTY_FUNCTION__;

	constexpr auto start = [p]() constexpr
	{
		constexpr std::string_view key_with_space = "T = ";
		constexpr std::string_view key_no_space = "T =";
		constexpr std::string_view key_compact = "T=";

		auto pos = p.find(key_with_space);
		if (pos != std::string_view::npos)
		{
			return pos + key_with_space.size();
		}

		pos = p.find(key_no_space);
		if (pos != std::string_view::npos)
		{
			pos += key_no_space.size();
			return (pos < p.size() && p[pos] == ' ') ? pos + 1 : pos;
		}

		pos = p.find(key_compact);
		return pos == std::string_view::npos ? pos : pos + key_compact.size();
	}();

	if (start == std::string_view::npos)
	{
		return p;
	}

	constexpr auto end = [p, start]() constexpr
	{
		std::size_t angle_depth = 0;
		std::size_t paren_depth = 0;
		std::size_t bracket_depth = 0;

		for (std::size_t i = start; i < p.size(); ++i)
		{
			const char c = p[i];

			switch (c)
			{
			case '<':
				++angle_depth;
				break;
			case '>':
				if (angle_depth > 0)
				{
					--angle_depth;
				}
				break;
			case '(':
				++paren_depth;
				break;
			case ')':
				if (paren_depth > 0)
				{
					--paren_depth;
				}
				break;
			case '[':
				++bracket_depth;
				break;
			case ']':
				if (bracket_depth > 0)
				{
					--bracket_depth;
					break;
				}
				if (angle_depth == 0 && paren_depth == 0)
				{
					return i;
				}
				break;
			case ';':
				if (angle_depth == 0 && paren_depth == 0 && bracket_depth == 0)
				{
					return i;
				}
				break;
			case ',':
				if (angle_depth == 0 && paren_depth == 0 && bracket_depth == 0)
				{
					return i;
				}
				break;
			default:
				break;
			}
		}

		return std::string_view::npos;
	}();

	return end == std::string_view::npos ? p.substr(start) : p.substr(start, end - start);
}
#elif defined(_MSC_VER)
/**
 * @brief Returns the compile-time type name of T.
 *
 * @return Type name extracted from compiler signature metadata.
 */
template <class T>
constexpr std::string_view type_name()
{
	constexpr std::string_view p = __FUNCSIG__;

	constexpr auto a = p.find("type_name<") + 10;
	constexpr auto b = p.find(">(void)");

	return p.substr(a, b - a);
}
#else
# error Unsupported compiler for type_name()
#endif
