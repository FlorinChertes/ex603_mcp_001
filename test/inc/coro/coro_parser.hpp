// Copyright (c) Andreas Fertig.
// SPDX-License-Identifier: MIT

#include <cassert>

#include <coroutine>

#include <cstdio>
#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>
#include <version>

using std::byte;
std::byte operator""_B(char c)
{
  return static_cast<byte>(c);
}

std::byte operator""_B(unsigned long long c)
{
  return static_cast<byte>(c);
}

template<typename T, typename G,
         typename... Bases>  // #A Allow multiple bases for awaiter
struct promise_type_base : public Bases... {

  T mValue;

  auto yield_value(T value)
  {
    mValue = value;
    return std::suspend_always{};
  }

  auto get_return_object() { return G{this}; };

  std::suspend_always initial_suspend() { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void                return_void() {}
  void                unhandled_exception() { std::terminate(); }
};


namespace coro_iterator {
  template<typename PT>
  struct iterator {

    using coro_handle = std::coroutine_handle<PT>;

    coro_handle mCoroHdl{nullptr};
    bool        mDone{true};

    using RetType = decltype(mCoroHdl.promise().mValue);

    void resume()
    {
      mCoroHdl.resume();
      mDone = mCoroHdl.done();
    }

    iterator() = default;

    iterator(coro_handle hco)
    : mCoroHdl{hco}
    {
      resume();
    }

    iterator& operator++()
    {
      resume();
      return *this;
    }

    bool operator==(const iterator& o) const { return mDone == o.mDone; }

    const RetType& operator*() const { return mCoroHdl.promise().mValue; }
    const RetType* operator->() const { return &(operator*()); }
  };
}  // namespace coro_iterator

template<typename T>
struct generator {
    using promise_type = promise_type_base<T, generator>;
    using PromiseTypeHandle = std::coroutine_handle<promise_type>;
    using iterator = coro_iterator::iterator<promise_type>;

    iterator begin() { return { mCoroHdl }; }
    iterator end() { return {}; }

    generator(generator const&) = delete;
    generator(generator&& rhs)
        : mCoroHdl(rhs.mCoroHdl)
    {
        rhs.mCoroHdl = nullptr;
    }

    ~generator()
    {
        if (mCoroHdl) { mCoroHdl.destroy(); }
    }

private:
    friend promise_type;  // #A As the default ctor is private G needs to be a friend
    explicit generator(promise_type* p)
        : mCoroHdl(PromiseTypeHandle::from_promise(*p))
    {}

    PromiseTypeHandle mCoroHdl;
};


template<typename T>
struct awaitable_promise_type_base {

  std::optional<T> mRecentSignal;

  struct awaiter {

    std::optional<T>& mRecentSignal;

    bool await_ready() const { return mRecentSignal.has_value(); }
    void await_suspend(std::coroutine_handle<>) {}

    T await_resume()
    {
      assert(mRecentSignal.has_value());
      auto tmp = *mRecentSignal;
      mRecentSignal.reset();
      return tmp;
    }
  };

  // A promise_type must provide a method await_transform
  // to be an Awaitbale type.
  // This method takes a type as a single argument and
  // returns an Awaiter.
  [[nodiscard]] awaiter await_transform(T) {
      return awaiter{mRecentSignal};
  }
};


template<typename T, typename U>
struct [[nodiscard]] async_generator
{
  using promise_type = promise_type_base<   T,
                                            async_generator,
                                            awaitable_promise_type_base<U>>;

  using PromiseTypeHandle = std::coroutine_handle<promise_type>;

  T operator()()
  {
    // #A the move also clears the mValue of the promise
    auto tmp{std::move(mCoroHdl.promise().mValue)};

    // #B but we have to set it to a defined state
    mCoroHdl.promise().mValue.clear();

    return tmp;
  }

  void SendSignal(U signal)
  {
    mCoroHdl.promise().mRecentSignal = signal;

    if(not mCoroHdl.done()) {
        mCoroHdl.resume();
    }
  }

  async_generator(const async_generator&) = delete;
  async_generator(async_generator&& rhs)
  : mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)}
  {}

  ~async_generator()
  {
    if(mCoroHdl) { mCoroHdl.destroy(); }
  }

private:
  friend promise_type;  // #C As the default ctor is private G needs to be a friend
  explicit async_generator(promise_type* p)
    : mCoroHdl(PromiseTypeHandle::from_promise(*p))
  {}

  PromiseTypeHandle mCoroHdl;
};


static const byte ESC{'H'};
static const byte SOF{0x10};

using FSM = async_generator<std::string, byte>;
FSM Parse()
{
    while(true) {

        // look for either an operator co_await(byte) or
        // for await_transform(byte) and call it
        //
        // calls await_transform
        // creates the awaiter with the existing async_generator.mRecentSignal
        // Before this call async_generator.SendSignal,
        // that puts the byte in async_generator.mRecentSignal
        // and awaiter.await_resume returnds the byte value
        // becase awaiter.await_ready returns true (mRecentSignal not empty) do not wait,
        byte b = co_await byte{};

        if(ESC != b) {
            continue;
        }

        b = co_await byte{};
        if(SOF != b) {
            continue;   // #A  not looking at a start sequence
        }

        std::string frame{};
        while(true) {  // #B capture the full frame

            b = co_await byte{};

            if(ESC == b) {
                // #C skip this byte and look at the next one
                b = co_await byte{};

                if(SOF == b) {
                    // call yield_value and put the frame : std::string
                    // in async_generator...mValue
                    co_yield frame;
                    break;
                }
                else if(ESC != b) {  // #D out of sync
                    break;
                }
            }

            frame += static_cast<char>(b);
        }
    }
}

using Byte_generator = generator<std::byte>;
Byte_generator sender(std::vector<byte> fakeBytes)
{
  for(const auto& b : fakeBytes)
  {
      // call yield_value and put the b : std::byte
      // in generator...mValue
      co_yield b;
  }
}

void HandleFrame(const std::string& frame)
{
    printf("%s\n", frame.c_str());
}

void ProcessStream(Byte_generator& stream, FSM& parse)
{

    for(const auto& b : stream) {
        // get the byte value from the generator.mValue

        // #A Send the new byte to the waiting Parse coroutine
        // put it in the async_generator.mRecentSignal
        parse.SendSignal(b);

        // #B Check whether we have a complete frame
        if(const auto& res = parse(); res.length()) {
            // get the frame from the async_generator.mValue
            HandleFrame(res);
        }
    }

}


