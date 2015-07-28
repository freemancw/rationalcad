CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
 
project(RATIONALCAD)

# 32 or 64 bit architecture?
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(RCAD_WIN_ARCH "x64")
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(RCAD_WIN_ARCH "Win32")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

# Specify where to find MPIR libs
include_directories(${RATIONALCAD_SOURCE_DIR}/dependencies/mpir/)
link_directories(${RATIONALCAD_SOURCE_DIR}/dependencies/mpir/build.vc12/${RCAD_WIN_ARCH}/${CMAKE_BUILD_TYPE})

add_subdirectory(geometry)
add_subdirectory(utility)
add_subdirectory(rationalcad)
