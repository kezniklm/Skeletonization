/**
*
* @file pipeline.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Defines a small move-based functional pipeline utility.
*
* This header provides a lightweight wrapper that chains transformations in a
* fluent style while preserving move semantics.
*
* Main responsibilities:
* - store a value for chained processing
* - apply transformation steps with move semantics
* - return final transformed value
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <functional>
#include <type_traits>
#include <utility>

/**
 * @class pipeline
 * @brief Wraps a value for fluent transformation chaining.
 *
 * This class executes chained callable steps and forwards intermediate values
 * by move to minimize copying.
 */
template <typename T>
class pipeline
{
public:
	/**
	 * @brief Constructs a new pipeline object.
	 *
	 * @param input Initial value for the pipeline.
	 */
	template <typename U>
	explicit pipeline(U&& input)
		: value_(std::forward<U>(input))
	{
	}

	/**
	 * @brief Applies a transformation function to the stored value.
	 *
	 * @param func Callable invoked with the current value.
	 * @return Pipeline containing the transformed result.
	 */
	template <typename Func>
	auto then(Func&& func) &&
	{
		using result_type = decltype(std::invoke(
			std::forward<Func>(func), std::move(value_)));

		return pipeline<result_type>(
			std::invoke(std::forward<Func>(func), std::move(value_)));
	}

	/**
	 * @brief Returns the current pipeline value.
	 *
	 * @return Stored value after chained transformations.
	 */
	T get() &&
	{
		return std::move(value_);
	}

private:
	/// Stored pipeline value.
	T value_;
};

template <typename U>
pipeline(U&&) -> pipeline<std::decay_t<U>>;
