/**
*
* @file class_name.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Provides compile-time extraction of unqualified class names.
*
* This header builds on type-name reflection and removes namespace prefixes
* to produce a short class identifier used for algorithm registration.
*
* Main responsibilities:
* - extract unqualified class names at compile time
* - strip namespace qualifiers from reflected type names
* - provide registration-friendly type identifiers
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string_view>

#include "type_name.hpp"

/**
 * @brief Returns the unqualified class name of T.
 *
 * @return Class name without namespace qualifiers.
 */
template <class T>
constexpr std::string_view class_name()
{
	constexpr std::string_view full_type_name = type_name<T>();

	constexpr auto position = full_type_name.rfind("::");

	return position == std::string_view::npos ? full_type_name : full_type_name.substr(position + 2);
}
