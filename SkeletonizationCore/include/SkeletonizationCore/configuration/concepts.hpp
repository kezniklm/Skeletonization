/**
*
* @file concepts.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines compile-time concepts for skeletonizer implementations.
*
* This header provides concept constraints for validating implementation types
* against backend markers and base skeletonizer template inheritance.
*
* Main responsibilities:
* - detect inheritance from skeletonizer template types
* - define backend-specific concept constraints
* - enforce compile-time type correctness for registrations
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <type_traits>

#include "SkeletonizationCore/skeletonizer/backend.hpp"
#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"

namespace configuration
{
	/**
	 * @class is_derived_from_template
	 * @brief Detects whether T derives from an instantiation of Template.
	 *
	 * This helper trait is used to validate that candidate algorithm types are
	 * derived from the skeletonizer template family.
	 */
	template <class T, template<int, int, int> class Template>
	struct is_derived_from_template
	{
		/**
		 * @brief Matches when pointer is convertible to target template instantiation.
		 * @return std::true_type when T derives from Template<A,B,C>.
		 */
		template <int A, int B, int C>
		static std::true_type test(const Template<A, B, C>*);
		/**
		 * @brief Fallback overload for non-matching types.
		 * @return std::false_type when conversion does not exist.
		 */
		static std::false_type test(...);
		static constexpr bool value = decltype(test(std::declval<const T*>()))::value;
	};

	/// True when T is a skeletonizer implementation type.
	template <class T>
	concept skeletonizer_implementation = is_derived_from_template<T, skeletonizer::skeletonizer>::value;

	/// True when T is tagged as CPU backend.
	template <class T>
	concept cpu_backend = std::is_base_of_v<skeletonizer::backend_cpu, T>;

	/// True when T is tagged as threaded backend.
	template <class T>
	concept threaded_backend = std::is_base_of_v<skeletonizer::backend_threads, T>;

	/// True when T is tagged as GPU backend.
	template <class T>
	concept gpu_backend = std::is_base_of_v<skeletonizer::backend_gpu<>, T>;
}
