"base config"

COPTS_INCLUDE = ["-I/usr/local/include/eigen3"]

COPTS_BASE = [
    "-Wall",
    "-Wno-strict-aliasing",
    "-std=c++17",
    "-O3",
    "-fpermissive",
]

COPTS_STATIC_LIB = ["-fPIC"]

COPTS_ADD_RTI = [
    "-DRTI_UNIX",
    "-DRTI_LINUX",
    "-DRTI_64BIT",
    "-DRTI_STATIC",
]

LINK_BASE = ["-L/usr/local/lib/"]
