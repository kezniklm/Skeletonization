/**
*
* @file creators.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines creator helpers and algorithm entry metadata templates.
*
* This header provides utilities for creating skeletonizer factories and
* describing per-algorithm backend availability in a strongly typed form.
*
* Main responsibilities:
* - generate creator callbacks for concrete implementations
* - model backend availability using algorithm entry templates
* - append creators for requested backend types
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>

#include "SkeletonizationCore/skeletonizer/skeletonizer.hpp"
#include "SkeletonizationCore/reflection/class_name.hpp"

#include "concepts.hpp"
#include "types.hpp"

namespace configuration
{
	/**
	 * @brief Creates a callable that instantiates SkeletonizerImplementation.
	 *
	 * @return Creator callback returning a new skeletonizer instance.
	 */
	template <skeletonizer_implementation SkeletonizerImplementation>
	skeletonizer_creator make_creator()
	{
		return []() -> std::unique_ptr<skeletonizer::skeletonizer<>>
		{
			return std::make_unique<SkeletonizerImplementation>();
		};
	}

	/**
	 * @class algorithm_entry
	 * @brief Describes available backend implementations for one algorithm.
	 *
	 * This template stores algorithm metadata and exposes helper logic that
	 * appends creator callbacks based on requested backend type.
	 */
	template <
		class CpuImplementation = std::nullptr_t,
		class ThreadedImplementation = std::nullptr_t,
		class GpuImplementation = std::nullptr_t>
	struct algorithm_entry
	{
		/// Normalized algorithm name.
		const std::string_view name;

		using cpu_type = CpuImplementation;
		using thread_type = ThreadedImplementation;
		using gpu_type = GpuImplementation;

		static constexpr bool has_cpu =
			!std::is_same_v<CpuImplementation, std::nullptr_t>;
		static constexpr bool has_thread =
			!std::is_same_v<ThreadedImplementation, std::nullptr_t>;
#if SKELETONIZATION_WITH_GPU
		static constexpr bool has_gpu =
			!std::is_same_v<GpuImplementation, std::nullptr_t>;
#else
		static constexpr bool has_gpu = false;
#endif

		static_assert(has_cpu || has_thread || has_gpu,
		              "AlgorithmEntry must have at least one implementation "
		              "type (cpu/thread/gpu)");

		static_assert(!has_cpu || (skeletonizer_implementation<CpuImplementation> && cpu_backend<CpuImplementation>),
		              "CpuImplementation must derive from skeletonizer<>");
		static_assert(
			!has_thread || (skeletonizer_implementation<ThreadedImplementation> && threaded_backend<
				ThreadedImplementation>),
			"ThreadedImplementation must derive from skeletonizer<>");
#if SKELETONIZATION_WITH_GPU
		static_assert(!has_gpu || (skeletonizer_implementation<GpuImplementation> && gpu_backend<GpuImplementation>),
		              "GpuImplementation must derive from skeletonizer<>");
#endif

		/**
		 * @brief Appends creators matching the selected backend type.
		 *
		 * @param type Requested backend type.
		 * @param out Output creator collection.
		 */
		static void push_creators_for_type(skeletonizer::skeletonizer_type type,
		                                   std::vector<skeletonizer_creator>& out)
		{
			if constexpr (has_cpu)
			{
				if (type == skeletonizer::skeletonizer_type::cpu)
				{
					out.push_back(make_creator<CpuImplementation>());
				}
			}

			if constexpr (has_thread)
			{
				if (type == skeletonizer::skeletonizer_type::thread)
				{
					out.push_back(make_creator<ThreadedImplementation>());
				}
			}

#if SKELETONIZATION_WITH_GPU
			if constexpr (has_gpu)
			{
				if (type == skeletonizer::skeletonizer_type::gpu)
				{
					out.push_back(make_creator<GpuImplementation>());
				}
			}
#endif
		}
	};

	/**
	 * @brief Creates an algorithm entry for provided implementation types.
	 *
	 * @return Algorithm entry with inferred algorithm name.
	 */
	template <class CpuT = std::nullptr_t,
	          class ThreadT = std::nullptr_t,
	          class GpuT = std::nullptr_t>
	constexpr algorithm_entry<CpuT, ThreadT, GpuT> make_entry()
	{
		return algorithm_entry<CpuT, ThreadT, GpuT>{class_name<CpuT>()};
	}
}
