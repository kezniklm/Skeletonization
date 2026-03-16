/**
*
* @file string.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Provides small ASCII-oriented string utility helpers.
*
* This header defines utility functions used for case normalization and
* case-insensitive ASCII comparisons in configuration and lookup logic.
*
* Main responsibilities:
* - normalize string content to lowercase
* - compare ASCII strings case-insensitively
* - support robust algorithm and backend lookup parsing
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <algorithm>
#include <cctype>
#include <string>

/**
 * @brief Converts a string to lowercase.
 *
 * @param s Input string.
 * @return Lowercased string.
 */
inline std::string to_lower(std::string s)
{
	std::ranges::transform(s, s.begin(), tolower);

	return s;
}

/**
 * @brief Compares two ASCII strings without case sensitivity.
 *
 * @param a First string.
 * @param b Second string.
 * @return True when both strings match ignoring case.
 */
constexpr bool equals_ascii(std::string_view a, std::string_view b) noexcept
{
	return a.size() == b.size() && std::ranges::equal(a, b, [](const unsigned char x, const unsigned char y)
	{
		return static_cast<unsigned char>(std::tolower(x)) == static_cast<unsigned char>(std::tolower(y));
	});
}
