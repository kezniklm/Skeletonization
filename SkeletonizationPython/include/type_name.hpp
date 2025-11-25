#pragma once

#include <string_view>

#if defined(__clang__) || defined(__GNUC__)
template <class T>
constexpr std::string_view type_name()
{
	constexpr std::string_view p = __PRETTY_FUNCTION__;

	const auto a = p.find("T = ") + 4;

	const auto b = p.rfind(']');

	return p.substr(a, b - a);
}
#elif defined(_MSC_VER)
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
