// Copyright (c) TakagiY 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt)

#include "../channel.hpp"
#include <cstddef>
#include <functional>
#include <string>
#include <thread>
#define CATCH_CONFIG_MAIN
#include "./catch.hpp"

TEST_CASE("Example on README.md", "[chan] [readme]") {
  channel::chan<std::string> chan;

  auto t1 = std::thread(
      [](channel::chan<std::string> &chan) {
        chan.push("Who are you?");
        std::string name = chan.pull();
        chan.push("Nice to meet you, " + name + ".");
      },
      std::ref(chan));

  auto t2 = std::thread(
      [](channel::chan<std::string> &chan) {
        chan.pull();
        chan.push("John");
        std::string message = chan.pull();
        CHECK(message == "Nice to meet you, John.");
      },
      std::ref(chan));

  t1.join();
  t2.join();
}

TEST_CASE("chan<int, 3> can buffer three integers whitout blocking.",
          "[chan]") {
  channel::chan<int, 3> chan;
  for (std::size_t i = 0; i < 97; i += 3) {
    chan.push(i);
    chan.push(i + 1);
    chan.push(i + 2);
    CHECK(chan.pull() == i);
    CHECK(chan.pull() == i + 1);
    CHECK(chan.pull() == i + 2);
  }
}

TEST_CASE("chan<int> can move integers between threads.",
          "[chan] [bufferless]") {
  channel::chan<int> chan;
  auto produce = [](channel::chan<int> &chan) {
    for (std::size_t i = 0; i < 100; ++i) {
      chan.push(i);
    }
  };
  auto check = [](channel::chan<int> &chan) {
    for (std::size_t i = 0; i < 100; ++i) {
      CHECK(chan.pull() == i);
    }
  };

  auto producer = std::thread(produce, std::ref(chan));
  auto checker = std::thread(check, std::ref(chan));
  producer.join();
  checker.join();
}

TEST_CASE("chan<int, 1> can move integers between threads.", "[chan]") {
  channel::chan<int, 1> chan;
  auto produce = [](channel::chan<int, 1> &chan) {
    for (std::size_t i = 0; i < 100; ++i) {
      chan.push(i);
    }
  };
  auto check = [](channel::chan<int, 1> &chan) {
    for (std::size_t i = 0; i < 100; ++i) {
      CHECK(chan.pull() == i);
    }
  };

  auto producer = std::thread(produce, std::ref(chan));
  auto checker = std::thread(check, std::ref(chan));
  producer.join();
  checker.join();
}

TEST_CASE("chan<int, 10> can move integers between threads.", "[chan]") {
  channel::chan<int, 10> chan;
  auto produce = [](channel::chan<int, 10> &chan) {
    for (std::size_t i = 0; i < 100; ++i) {
      chan.push(i);
    }
  };
  auto check = [](channel::chan<int, 10> &chan) {
    for (std::size_t i = 0; i < 100; ++i) {
      CHECK(chan.pull() == i);
    }
  };

  auto producer = std::thread(produce, std::ref(chan));
  auto checker = std::thread(check, std::ref(chan));
  producer.join();
  checker.join();
}
