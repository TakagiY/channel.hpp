# channel.hpp - Lightweight go-like inter-thread communication in C++

[![Build Status](https://travis-ci.org/takagiy/channel.hpp.svg?branch=master)](https://travis-ci.org/takagiy/channel.hpp)
[![codecov](https://codecov.io/gh/takagiy/channel.hpp/branch/master/graph/badge.svg)](https://codecov.io/gh/takagiy/channel.hpp)
[![license BSL-1.0](https://img.shields.io/badge/License-BSL--1.0-blue.svg)](LICENSE)

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

# License

Copyright (c) TakagiY 2018  
Distributed under the Boost Software License, Version 1.0.  
(See accompanying file [LICENSE](LICENSE) or copy at http://boost.org/LICENSE_1_0.txt)

