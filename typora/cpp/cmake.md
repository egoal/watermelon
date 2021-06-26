```cmake
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

PROJECT_SOURCE_DIR # 当前作用域内上一个project的路径

# 启用c++11
set(CMAKE_CXX_FLAGS "-std=c++11")
# 专业版
inlcude(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORT_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORT_CXX0X)
if(COMPILER_SUPPORT_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_definitions(-DCOMPILEDWITHC11)
elseif(COMPILER_SUPPORT_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	add_definitions(-DCOMPILEDWITHC0X)
else()
	message(FATAL_ERROR "the compiler ${CMAKE_CXX_COMPILER} has no c++11 support.")
endif()

aux_source_directory(. SRC_LST)
```



`cmake -D CMAKE_BUILD_TYPE=Debug ..`



```cmake
# 常用库
# eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# gflags
find_package(Gflags REQUIRED)
if(GFLAGS_FOUND)
${GFLAGS_INCLUDE_DIRS}
${GFLAGS_LIBRARIES}
```

