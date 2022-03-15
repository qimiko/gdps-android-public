cmake_minimum_required(VERSION 3.4.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANDROID_STL gnustl_static)

# ANDROID_ABI doesn't seem to be set otherwise for whatever reason
set(ANDROID_ABI armeabi-v7a)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/gdps/lib/${ANDROID_ABI}/)

project(gdps VERSION 8.1.2.1)

add_subdirectory(aurav2)

install(TARGETS aurav2 LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/gdps/lib/armeabi-v7a)
install(CODE "execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build-apk.sh)")
