#pragma once

#include <type_traits>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace configuration
{
	template <class T, template<int, int, int> class Template>
	struct is_derived_from_template
	{
		template <int A, int B, int C>
		static std::true_type test(const Template<A, B, C>*);
		static std::false_type test(...);
		static constexpr bool value = decltype(test(std::declval<const T*>()))::value;
	};

	template <class T>
	concept skeletonizer_implementation = is_derived_from_template<T, skeletonizer::skeletonizer>::value;

	template <class T>
	concept cpu_backend = std::is_base_of_v<skeletonizer::backend_cpu, T>;

	template <class T>
	concept threaded_backend = std::is_base_of_v<skeletonizer::backend_threads, T>;

	template <class T>
	concept gpu_backend = std::is_base_of_v<skeletonizer::backend_gpu<>, T>;
}
