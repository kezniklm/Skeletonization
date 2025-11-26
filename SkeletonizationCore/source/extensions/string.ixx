module;

#include <algorithm>
#include <cctype>
#include <string>

export module string_extensions;

inline std::string to_lower(std::string s)
{
	std::ranges::transform(s, s.begin(), tolower);

	return s;
}
