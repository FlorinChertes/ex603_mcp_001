
#include <iostream>

#include <numeric>
#include <iterator>
#include <algorithm>
#include <queue>
#include <map>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename... Ts>
constexpr auto add(Ts... args)
{
	return (... + args);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template <typename... Ts>
constexpr auto add_to_one(Ts... args) {
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

	template<typename ...Args>
	void fold_print(Args&&... args) {
		(std::cout << ... << std::forward<Args>(args)) << '\n';
	}

	template<typename ...Args>
	void fold_separate_line(Args&&... args) {

		auto separate_line = [](const auto& v) {
			std::cout << v << '\n';
		};

		( ... , separate_line(std::forward<Args>(args)) ); // over comma operator
	}

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

namespace func_lib
{

	template<typename R, typename T>
	bool in(R const& range, T const& value)
	{
		for (auto const& x : range)
		{
			if (x == value)
			{
				return true;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	template <typename T>
	concept Equality_comparable = requires (T a, T b)
	{
		{a == b} -> std::convertible_to<bool>;
		{a != b} -> std::convertible_to<bool>;
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	template <typename T, typename U>
	concept Same = std::is_same<T, U>::value;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	// The value_type of a class is a member type
	template<typename T>
	struct value_type;

	template<typename T>
	using value_type_t = typename value_type<T>::type;


	// The value_type of a class is a member type
	template<typename T>
	requires requires {typename T::value_type; }
	struct value_type<T>
	{
		using type = typename T::value_type;
	};

	// The value_type of a pointer is the type of element pointed to.
	template<typename T>
	struct value_type<T*>
	{
		using type = T;
	};

	// The value_type of an array is its element type.
	template<typename T, int N>
	struct value_type<T[N]>
	{
		using type = T;
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	template <typename R>
	concept Range = requires (R range)
	{
		range.begin();
		range.end();
	};

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	template<Range R, Equality_comparable T>
	requires Same<value_type_t<R>, T>
		bool in_c(R const& range, T const& value)
	{
		for (const auto& x : range) {
			if (x == value) {
				return true;
			}
		}
		return false;
	}


	template<typename T> 
	concept C = requires(T x) { x + x; };
	
	template<typename T>
	requires C<T>
	T add(T a, T b) { return a + b; }


}
