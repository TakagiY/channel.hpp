// Copyright (c) TakagiY 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt)

#ifndef TY_CHANNEL_HPP
#define TY_CHANNEL_HPP

#include <array>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <utility>

namespace channel {
  template <class T, std::size_t Size = 0>
  struct chan;
}

namespace channel {
  template <class T, std::size_t Size>
  struct chan {
  private:
    std::array<T, Size> buffer;
    typename std::array<T, Size>::iterator write_head = buffer.begin();
    typename std::array<T, Size>::iterator read_head = buffer.begin();
    std::size_t length = 0;
    std::mutex mutex;
    std::condition_variable write_cv;
    std::condition_variable read_cv;

    auto advance(typename std::array<T, Size>::iterator &it) -> void {
      ++it;
      if (it == buffer.end()) {
        it = buffer.begin();
      }
    }

    template <class U>
    auto push_(U &&x) -> void {
      std::unique_lock<std::mutex> lock{mutex};
      write_cv.wait(lock, [this] { return length != Size; });
      advance(write_head);
      *write_head = std::forward<U>(x);
      ++length;
      read_cv.notify_one();
    }

  public:
    auto push(T const &x) -> void { push_(x); }
    auto push(T &&x) -> void { push_(std::move(x)); }

    auto pull() -> T {
      std::unique_lock<std::mutex> lock{mutex};
      read_cv.wait(lock, [this] { return length != 0; });
      advance(read_head);
      auto x = *read_head;
      --length;
      write_cv.notify_one();
      return x;
    }
  };

  template <class T>
  struct chan<T, 0> {
  private:
    T buffer;
    bool length = false;
    std::mutex mutex;
    std::condition_variable write_cv;
    std::condition_variable read_cv;

    template <class U>
    auto push_(U &&x) -> void {
      std::unique_lock<std::mutex> lock{mutex};
      write_cv.wait(lock, [this] { return !length; });
      buffer = std::forward<U>(x);
      length = true;
      read_cv.notify_one();
    }

  public:
    auto push(T const &x) -> void { push_(x); }
    auto push(T &&x) -> void { push_(std::move(x)); }

    auto pull() -> T {
      std::unique_lock<std::mutex> lock{mutex};
      read_cv.wait(lock, [this] { return length; });
      auto x = buffer;
      length = false;
      write_cv.notify_one();
      return x;
    }
  };
}

#endif
