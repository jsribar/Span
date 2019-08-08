#pragma once

#include <functional>
#include <stdexcept>

template <typename T, typename Less = std::less<T>, typename Equal = std::equal_to<T>>
class span
{
public:
	explicit span(const T& lower, const T& upper)
		: lower_m(lower)
		, upper_m(upper)
	{
		if (!less_or_equal(lower_m, upper_m))
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

	bool is_empty() const noexcept
	{
		return equal_m(lower_m, upper_m);
	}

	bool is_inside(const T& value) const
	{
		return !(less_m(value, lower_m)) && (less_m(value, upper_m));
	}

	bool is_inside(const span& other) const
	{
		return !(less_m(other.lower_m, lower_m)) && !(less_m(upper_m, other.upper_m));
	}

	span intersection(const span& other)
	{
		if (is_empty())
			return *this;
		if (other.is_empty())
			return other;
		const T& lower = std::max(lower_m, other.lower_m);
		const T& upper = std::min(upper_m, other.upper_m);
		return span{ lower, upper };
	}

private:
	T lower_m;
	T upper_m;
	Less less_m;
	Equal equal_m;

	bool less_or_equal(const T& first, const T& second)
	{
		return equal_m(first, second) || less_m(first, second);
	}
};

