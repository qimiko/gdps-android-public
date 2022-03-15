cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANDROID_STL gnustl_static)

project(aurav2 VERSION 8.1.2.1)

option(GDMOD_USE_IMGUI "Enable ImGui and logging constants" OFF)
option(GDMOD_USE_SPDLOG "Enable spdlog" OFF)

set(GDMOD_SOURCES src/base/module.cpp
  src/base/hooks.cpp
  src/base/hook_handler.cpp
  src/aura.cpp
  src/modules/default.cpp
  src/modules/levels.cpp
  src/modules/hacks.cpp
  src/modules/limits.cpp
  src/modules/textures.cpp
  src/modules/badges.cpp
  src/modules/search.cpp
  src/modules/settings.cpp
  src/modules/urls.cpp
  src/modules/token.cpp)

if (GDMOD_USE_IMGUI)
  set(GDMOD_SOURCES ${GDMOD_SOURCES} src/modules/overlay.cpp)
endif()

add_library(aurav2 SHARED ${GDMOD_SOURCES})

set_property(TARGET aurav2 PROPERTY CXX_CLANG_TIDY "${ANDROID_LLVM_TOOLCHAIN_PREFIX}clang-tidy${ANDROID_TOOLCHAIN_SUFFIX}")

set(GDMOD_INCLUDE 
  ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d/
  ${CMAKE_CURRENT_SOURCE_DIR}/external/fmod/

  ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d/incl/cocos2dx
  ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d/incl/cocos2dx/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d/incl/cocos2dx/kazmath/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d/incl/cocos2dx/platform/android
  ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d/incl/extensions/

  ${CMAKE_CURRENT_SOURCE_DIR}/external/rapidjson/include/

  ${CMAKE_CURRENT_SOURCE_DIR}/external/Dobby/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (GDMOD_USE_SPDLOG)
  set(GDMOD_SOURCES ${GDMOD_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog/include)
endif()

target_include_directories(aurav2 PRIVATE ${GDMOD_INCLUDE})

find_package(Git)
execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --always --abbrev=7 --dirty=+ --exclude \*
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(COMMAND
  "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

string(TIMESTAMP COMPILE_TIME "%Y.%m.%d %H:%M:%S")

add_custom_target(invalidate_files ALL COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/include/base/cmake_config.hpp.in)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/base/cmake_config.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/base/cmake_config.hpp" @ONLY)

set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/Dobby)
target_link_libraries(aurav2 dobby)

if (GDMOD_USE_IMGUI)
  target_compile_definitions(aurav2 PRIVATE -DGDMOD_ENABLE_LOGGING)

  add_subdirectory(external/imgui)
  target_link_libraries(aurav2 ImGui)
endif()

if (GDMOD_USE_SPDLOG)
  target_compile_definitions(aurav2 PRIVATE -DGDMOD_ENABLE_SPDLOG)

  add_subdirectory(external/spdlog)
  target_link_libraries(aurav2 spdlog)
endif()

if (GDMOD_USE_CRASH_DUMPS)
  target_compile_definitions(aurav2 PRIVATE -DGDMOD_ENABLE_CRASH_DUMPS)
endif()

if (GDMOD_CRASH_UPLOAD_URL)
  target_compile_definitions(aurav2 PRIVATE "-DGDMOD_CRASH_UPLOAD_URL=\"${GDMOD_CRASH_UPLOAD_URL}\"")
endif()

if (GDMOD_DATA_PATH)
  target_compile_definitions(aurav2 PRIVATE "-DGDMOD_DATA_PATH=\"${GDMOD_DATA_PATH}\"")
endif()

if (GDMOD_SOURCE_URL)
  target_compile_definitions(aurav2 PRIVATE "-DGDMOD_SOURCE_URL=\"${GDMOD_SOURCE_URL}\"")
endif()

add_definitions(-D__STDC_FORMAT_MACROS)
add_subdirectory(external/breakpad)
target_link_libraries(aurav2 Breakpad)

target_link_libraries(aurav2 libcocos2dcpp.so)
target_link_libraries(aurav2 libfmod.so)
target_link_libraries(aurav2 android)
