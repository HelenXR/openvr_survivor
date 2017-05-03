cmake_minimum_required (VERSION 2.6)

#project name:openvr_survivor
project(openvr_survivor)

#set include path
include_directories(include third_party/openvr/include)

#set output path
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})

#generate dll
add_library(driver_openvr_survivor SHARED src/openvr_survivor.cc)
