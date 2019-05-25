
#include <numeric>
#include <iterator>
#include <algorithm>
#include <queue>
#include <map>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace funclib
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------

	template <typename F, typename R>
	R mapf(F&& f, R r)
	{
		std::transform(
			std::begin(r), std::end(r),
			std::begin(r),
			std::forward<F>(f));
		return r;
	}


	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	template<typename F, typename T, typename U>
	std::map<T, U> mapf(F&& f, std::map<T, U> const& m)
	{
		std::map<T, U> r;

		for (auto const kvp : m)
		{
			r.insert(f(kvp));
		}
		return r;
	}

	template<typename F, typename T>
	std::queue<T> mapf(F&& f, std::queue<T> q)
	{
		std::queue<T> r;

		while (!q.empty())
		{
			r.push(f(q.front()));
			q.pop();
		}
		return r;
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	template <typename F, typename R, typename T>
	constexpr T foldl(F&& f, R&& r, T i)
	{
		return std::accumulate(
			std::begin(r), std::end(r),
			std::move(i),
			std::forward<F>(f));
	}

	template <typename F, typename R, typename T>
	constexpr T foldr(F&& f, R&& r, T i)
	{
		return std::accumulate(
			std::rbegin(r), std::rend(r),
			std::move(i),
			std::forward<F>(f));
	}
}


namespace func_lib_ex
{
	template <typename F, typename T1, typename T2>
	auto foldl(F&& f, T1 arg1, T2 arg2)
	{
		return f(arg1, arg2);
	}

	template <typename F, typename T, typename... Ts>
	auto foldl(F&& f, T head, Ts... rest)
	{
		return f(head, foldl(std::forward<F>(f), rest...));
	}
}
