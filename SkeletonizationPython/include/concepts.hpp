#pragma once

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

template <class T, template<int, int, int> class Template>
struct is_derived_from_template
{
	template <int A, int B, int C>
	static std::true_type test(const Template<A, B, C>*);
	static std::false_type test(...);
	static constexpr bool value = decltype(test(std::declval<const T*>()))::value;
};

template <class T>
concept skeletonizer_subclass = is_derived_from_template<T, skeletonizer::skeletonizer>::value;
