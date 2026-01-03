#pragma once

#include <algorithm>
#include <cctype>
#include <string>

inline std::string to_lower(std::string s)
{
	std::ranges::transform(s, s.begin(), tolower);

	return s;
}

constexpr bool equals_ascii(std::string_view a, std::string_view b) noexcept
{
	return a.size() == b.size() && std::ranges::equal(a, b, [](const unsigned char x, const unsigned char y)
	{
		return static_cast<unsigned char>(std::tolower(x)) == static_cast<unsigned char>(std::tolower(y));
	});
}
