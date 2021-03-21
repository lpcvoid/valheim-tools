cmake_minimum_required(VERSION 3.17)
project(valheim_tools)

set(CMAKE_CXX_STANDARD 20)

add_executable(playerdb_reader playerdb_reader_main.cpp ZPackage.cpp ZPackage.h)