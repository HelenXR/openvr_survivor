cmake_minimum_required (VERSION 2.8)

#project name:openvr_survivor
project(openvr_survivor)

#set include
FILE(GLOB_RECURSE HEADER_FILES_GLOB third_party/openvr/include/*.h include/*.h third_party/glog/include/*.h third_party/glm/*.hpp src/*.h third_party/ximmerse/include/*.h third_party/nolo/include/*.h)
set(HEADER_FILES include third_party/openvr/include third_party/glog/include third_party/glm src/ third_party/ximmerse/include third_party/nolo/include)
set(SOURCE_FILES src/openvr_survivor.cc src/server_provider.cc src/head_mount_display_device.cc src/keyboard_monitor.cc src/hand_controller_device.cc 
					src/glm_adapter.cc src/simple_math.cc)
include_directories(${HEADER_FILES})

#set output path
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE openvr_survivor/bin/win${BITS})

#find lib absolute path
find_library(GLOG_LIBRARY libglog.lib HINTS third_party/glog/lib)
find_library(XIMMERSE_DEVICE_LIBRARY xdevice.lib HINTS third_party/ximmerse/lib)
find_library(NOLO_USBHID_LIBRARY NoLo_USBHID.lib HINTS third_party/nolo/lib)
find_library(XIMMERSE_DEVICE_LIBRARY xdevice.lib HINTS third_party/ximmerse/lib)

#copy dll to output directory
file(COPY  third_party/glog/lib/libglog.dll DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
file(COPY  third_party/ximmerse/lib/xdevice.dll DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
file(COPY  third_party/ximmerse/lib/bleapi.dll DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
file(COPY  third_party/ximmerse/lib/alg_factory.dll DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
file(COPY  third_party/nolo/lib/NoLo_USBHID.dll DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})

#copy resources to driver directory
file(COPY resources/icons DESTINATION openvr_survivor/resources)
file(COPY resources/settings DESTINATION openvr_survivor/resources)

#generate dll
add_library(driver_openvr_survivor SHARED ${SOURCE_FILES} ${HEADER_FILES} ${HEADER_FILES_GLOB})

#set link libraries
target_link_libraries(driver_openvr_survivor ${GLOG_LIBRARY} ${XIMMERSE_DEVICE_LIBRARY} ${NOLO_USBHID_LIBRARY})