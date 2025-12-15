#pragma once

#include <algorithm>
#include <cctype>
#include <string>

inline std::string to_lower(std::string s)
{
	std::ranges::transform(s, s.begin(), tolower);

	return s;
}
