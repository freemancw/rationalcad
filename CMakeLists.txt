#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
 
project(RATIONALCAD)

include_directories(${RATIONALCAD_SOURCE_DIR}/dependencies/mpir/Win32/Release)
link_directories(${RATIONALCAD_SOURCE_DIR}/dependencies/mpir/Win32/Release)

add_subdirectory(logging)
add_subdirectory(mathlib)
add_subdirectory(rationalcad)