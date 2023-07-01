cmake_minimum_required(VERSION 3.16)
project(readcore)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

add_executable(make_core make_core.cc)
add_executable(readcore readcore.cc)
