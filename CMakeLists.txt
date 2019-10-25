cmake_minimum_required(VERSION 3.14)
project(simpleConsoleRW)

set(CMAKE_CXX_STANDARD 14)

add_executable(simpleConsoleRW main.cpp SimpleSerial.cpp SimpleSerial.h)