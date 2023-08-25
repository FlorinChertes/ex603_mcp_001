#include <coroutine>
#include <exception>
#include <iostream>
#include <list>
#include <string>
#include <utility>


struct Chat {

	struct promise_type {
		std::string _msgOut{};
		std::string _msgIn{};

		void					unhandled_exception() noexcept { }

		Chat					get_return_object() { return Chat{ this }; }

		std::suspend_always		initial_suspend() { return {}; }

		// called by co_yield
		std::suspend_always		yield_value(std::string msg) noexcept {
			_msgOut = std::move(msg);
			return {}; }

		// called by co_await
		auto await_transform(std::string) noexcept {

			struct awaiter {
				promise_type& pt;

				constexpr bool	await_ready() const noexcept {
					return true; // do not suspend, call immhediately await_resume
				}
				void			await_suspend(std::coroutine_handle<>) const noexcept {}

				std::string		await_resume() const noexcept {
					return pt._msgIn;
				}
			};
			return awaiter(*this);
		}

		// called by co_return
		void					return_value(std::string msg) noexcept {
			_msgOut = std::move(msg); }

		std::suspend_always		final_suspend() noexcept { return {}; }

	};

	using CoroHdl = std::coroutine_handle<promise_type>;
	CoroHdl mCoroHdl {};

	explicit Chat(promise_type* p) : mCoroHdl{CoroHdl::from_promise(*p)} {}
	Chat(Chat&& rhs) : mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)} {}

	~Chat() {
		if(mCoroHdl) {mCoroHdl.destroy();}
	}

	std::string listen () {
		if (mCoroHdl.done() == false) {
			mCoroHdl.resume();
		}

		return mCoroHdl.promise()._msgOut;
	}

	void answer (std::string msg) {
		mCoroHdl.promise()._msgIn = msg;

		if (mCoroHdl.done() == false) {
			mCoroHdl.resume();
		}
	}

};
