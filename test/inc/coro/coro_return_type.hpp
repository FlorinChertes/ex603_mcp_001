#include <coroutine>

struct ReturnType
{
	struct promise_type {

		int value_ {0};

		ReturnType get_return_object() {
			return ReturnType { this};
		}

		std::suspend_always initial_suspend() { return {}; }

		void return_void() {}
		void unhandled_exception() {}
		std::suspend_always final_suspend() noexcept { return {}; }
	};

	std::coroutine_handle<promise_type> hdl;

	explicit ReturnType(promise_type* p)
		: hdl(std::coroutine_handle<promise_type>::from_promise(*p)) {
	}

	~ReturnType() {
		if (hdl) { hdl.destroy(); }
	}

	void resume() { hdl.resume(); }

	struct TheAwaiter
	{
		int value_{ 0 };
		std::coroutine_handle<promise_type> hdl;

		TheAwaiter(int value)
			: value_{ value } {
		}

		bool await_ready() const noexcept { return false; }

		void await_suspend(std::coroutine_handle<promise_type> h) {
			hdl = h;
			hdl.promise().value_ = value_;
		}

		int await_resume() noexcept {
			return hdl.promise().value_;
		}
	};

	int get_value() {
		return hdl.promise().value_;
	}

	void set_value( int value) {
		hdl.promise().value_ = value;
	}
};

