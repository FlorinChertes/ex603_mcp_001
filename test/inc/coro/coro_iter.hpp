#include <coroutine>
#include <utility>
#include <iostream>

template<typename T, typename G>
struct promise_type_base {

	T mValue; // The value yielded or returned from a coroutine

	auto yield_value(T value)
	{	// Invoked by co_yield or co_return
		// Store the yielded value for access outside the coroutine
		mValue = std::move(value);

		return std::suspend_always{}; // Suspend the coroutine here
	}

	auto get_return_object() {
		return G{ this };
	}; // Return generator

	std::suspend_always initial_suspend() const { return {}; }
	std::suspend_always final_suspend() const  noexcept { return {}; }
	void return_void() const  {}
	void unhandled_exception() const {
		std::terminate();
	}

	static auto get_return_object_on_allocation_failure() {
		return G{nullptr};
	}
};

namespace coro_iterator {
template < typename PT>
struct iterator {

	std::coroutine_handle<PT> mCoroHdl{nullptr};

	void resume()
	{
		if (mCoroHdl.done() == false) {
			mCoroHdl.resume();
		}
	}

	iterator() = default;

	iterator(std::coroutine_handle<PT> hco)
		:mCoroHdl{ hco }
	{
		resume();
	}

	void operator++ () { resume(); }
	bool operator== (const iterator&) const
	{
		return mCoroHdl.done();
	}

	const auto& operator* () const
	{
		return mCoroHdl.promise().mValue;
	}
};
} // namespace coro_iterator


template < typename T>
struct generator {

	using promise_type =
		promise_type_base<T, generator>; // The PromiseType

	using PromiseTypeHandle = std::coroutine_handle<promise_type>;

	// Make the generator iterable
	using iterator = coro_iterator::iterator<promise_type>;
	iterator begin() { return { mCoroHdl }; }
	iterator end() { return {}; }

	generator(generator const&) = delete;

	generator(generator&& rhs) noexcept
		:mCoroHdl(std::exchange(rhs.mCoroHdl, nullptr)) {}

	~generator() {
		// We have to maintain the life - time of the coroutine
		if (mCoroHdl)
			mCoroHdl.destroy();
	}

private:
	friend promise_type; // As the default ctor is private
						 // promise_type needs to be a friend

	explicit generator(promise_type* p)
		: mCoroHdl{ PromiseTypeHandle::from_promise(*p) }
	{}

	PromiseTypeHandle mCoroHdl; // The coroutine handle
};
