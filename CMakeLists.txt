cmake_minimum_required(VERSION 3.15)
project(valheim_tools)

set(CMAKE_CXX_STANDARD 20)

add_executable(playerdb_reader playerdb_reader.cpp Player.cpp Player.h Helpers.h)