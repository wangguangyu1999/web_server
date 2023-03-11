cmake_minimum_required(VERSION 3.24)
project(web_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(web_server server/epoller.cpp server/epoller.h pool/thread_pool.cpp)
