cmake_minimum_required(VERSION 2.8)

project(test)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(${PROJECT_NAME} test.cpp Solution.cpp)
