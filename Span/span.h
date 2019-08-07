#pragma once

#include <functional>
#include <stdexcept>

template <typename T, typename Compare = std::less<T>>
class span
{
public:
	explicit span(const T& lower, const T& upper)
		: lower_m(lower)
		, upper_m(upper)
	{
		if (!(compare_m(lower_m, upper_m)))
			throw std::invalid_argument("lower limit must be less than upper limit");
	}

	const T& lower() const noexcept
	{
		return lower_m;
	}

	const T& upper() const noexcept
	{
		return upper_m;
	}

	bool is_inside(const T& value) const
	{
		return !(compare_m(value, lower_m)) && (compare_m(value, upper_m));
	}

	bool is_inside(const span& other) const
	{
		return !(compare_m(other.lower_m, lower_m)) && !(compare_m(upper_m, other.upper_m));
	}

private:
	T lower_m;
	T upper_m;
	Compare compare_m;
};

