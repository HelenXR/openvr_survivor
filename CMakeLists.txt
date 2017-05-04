cmake_minimum_required (VERSION 2.6)

#project name:openvr_survivor
project(openvr_survivor)

#set include path
include_directories(include third_party/openvr/include third_party/glog/include)

#set output path
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})

#find lib absolute path
find_library(GLOG_LIBRARY libglog.lib HINTS third_party/glog/lib)

#cp dll to output directory
file(COPY  third_party/glog/lib/libglog.dll DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})

#generate dll
add_library(driver_openvr_survivor SHARED src/openvr_survivor.cc src/server_provider.cc)

#set link libraries
target_link_libraries(driver_openvr_survivor ${GLOG_LIBRARY})