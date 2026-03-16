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

#include <string_view>

#if defined(__clang__) || defined(__GNUC__)
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
