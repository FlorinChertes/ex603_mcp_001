#include <iterator>
#include <algorithm>

template <typename... Ts>
auto add(Ts... args)
{
	return (... + args);
}

template <typename... Ts>
auto add_to_one(Ts... args)
{
	return (1 + ... + args);
}

template <typename T>
struct wrapper
{
	T const& value;
};

template <typename T>
constexpr auto operator<(wrapper<T> const& lhs,
	wrapper<T> const& rhs)
{
	return wrapper<T> {
		lhs.value < rhs.value ? lhs.value : rhs.value};
}

template <typename... Ts>
constexpr auto min(Ts && ... args)
{
	return (wrapper<Ts>{args} < ...).value;
}

namespace funclib
{
	template <typename F, typename R>
	R mapf(F&& f, R r)
	{
		std::transform(
			std::begin(r), std::end(r), std::begin(r),
			std::forward<F>(f));
		return r;
	}
}
