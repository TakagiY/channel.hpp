# Copyright (c) TakagiY 2018
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt)

CXX = clang++
CXXFLAGS = -std=c++11

.PHONY: test test/clean

test: test/test
	test/test

test/test: test/catch.hpp test/test.cpp channel.hpp
	$(CXX) $(CXXFLAGS) -pthread test/test.cpp -o test/test

test/catch.hpp:
	wget -P test https://github.com/catchorg/Catch2/releases/download/v2.4.2/catch.hpp

test/clean:
	rm -f test/catch.hpp
	rm -f test/test
