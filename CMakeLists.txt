CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
 
project(RATIONALCAD)

include_directories(${RATIONALCAD_SOURCE_DIR}/dependencies/mpir/)
link_directories(${RATIONALCAD_SOURCE_DIR}/dependencies/mpir/build.vc12/Win32/Debug)

add_subdirectory(geometry)
add_subdirectory(utility)
add_subdirectory(rationalcad)
