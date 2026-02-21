#pragma once

#include <string_view>

#include "type_name.hpp"

template <class T>
constexpr std::string_view class_name()
{
	constexpr std::string_view full_type_name = type_name<T>();

	constexpr auto position = full_type_name.rfind("::");

	return position == std::string_view::npos ? full_type_name : full_type_name.substr(position + 2);
}
