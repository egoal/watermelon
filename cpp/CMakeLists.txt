cmake_minimum_required(VERSION 2.8)

project(cpp)

set(CMAKE_CXX_FLAGS "-std=c++11")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

# libs
find_package(Gflags REQUIRED)
include_directories(${GFLAGS_INCLUDE_DIRS})

find_package(Boost REQUIRED)

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

set(CMN_LIBS ${GFLAGS_LIBRARIES} ${CERES_LIBRARIES})

##
macro(SIMPLE_OPTION_BUILD filename on_off)
    option(${filename} "build ${filename}" ${on_off})

    if(${filename})
        add_executable(${filename} ${filename}.cpp)
        target_link_libraries(${filename} ${CMN_LIBS})

    endif()
    # link specified includes & libs 

endmacro()

SIMPLE_OPTION_BUILD(sphere_test OFF)
SIMPLE_OPTION_BUILD(delaunay ON)

# pybind11
option(pyb_test "c++ pybind" OFF)
if(pyb_test)
    add_subdirectory(pyb_test)
endif()
