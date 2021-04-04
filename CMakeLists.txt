cmake_minimum_required(VERSION 3.15)
project(valheim_tools)

set(CMAKE_CXX_STANDARD 20)

add_executable(playerdb_reader playerdb_reader.cpp Player.cpp Player.h Helpers.h Types.h)
add_executable(whereismystuff whereismystuff.cpp Helpers.h World.cpp World.h Types.h)

target_link_libraries(playerdb_reader stdc++fs)
target_link_libraries(whereismystuff stdc++fs)