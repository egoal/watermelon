cmake_minimum_required(VERSION 2.8)

find_package(pybind11 REQUIRED)

pybind11_add_module(pyb pyb.cpp)

add_executable(pyb_main pyb.cpp)
target_include_directories(pyb_main PUBLIC /usr/include/python3.4)
target_link_libraries(pyb_main /usr/lib/python3.4/config-3.4m-x86_64-linux-gnu/libpython3.4.so)

pybind11_add_module(pyboo pyb_oo.cpp)
