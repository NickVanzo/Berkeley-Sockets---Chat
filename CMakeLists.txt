cmake_minimum_required(VERSION 3.20)
project(ChatApplication)

set(CMAKE_CXX_STANDARD 20)

add_executable(server server.cpp socketutil.cpp)
add_executable(client client.cpp socketutil.cpp)

target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR})
