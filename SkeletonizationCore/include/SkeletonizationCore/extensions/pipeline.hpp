#pragma once

#include <functional>
#include <type_traits>
#include <utility>

template <typename T>
class pipeline
{
public:
	template <typename U>
	explicit pipeline(U&& input)
		: value_(std::forward<U>(input))
	{
	}

	template <typename Func>
	auto then(Func&& func) &&
	{
		using result_type = decltype(std::invoke(
			std::forward<Func>(func), std::move(value_)));

		return pipeline<result_type>(
			std::invoke(std::forward<Func>(func), std::move(value_)));
	}

	T get() &&
	{
		return std::move(value_);
	}

private:
	T value_;
};

template <typename U>
pipeline(U&&) -> pipeline<std::decay_t<U>>;
