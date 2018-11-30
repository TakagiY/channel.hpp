# channel.hpp - Lightweight go-like inter-thread communication in C++

[![Build Status](https://travis-ci.org/TakagiY/channel.hpp.svg?branch=master)](https://travis-ci.org/TakagiY/channel.hpp)
[![codecov](https://codecov.io/gh/TakagiY/channel.hpp/branch/master/graph/badge.svg)](https://codecov.io/gh/TakagiY/channel.hpp)

```C++
#include "channel.hpp"
#include <string>
#include <cassert>

int main() {
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
        assert(message == "Nice to meet you, John.");
      },
      std::ref(chan));

  t1.join();
  t2.join();
}
```

