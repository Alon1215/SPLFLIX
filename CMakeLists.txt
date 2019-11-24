cmake_minimum_required(VERSION 3.15)
project(SPLFLIX)

set(CMAKE_CXX_STANDARD 11)

add_executable(SPLFLIX main.cpp src/Action.cpp src/Session.cpp src/Watchable.cpp src/User.cpp )