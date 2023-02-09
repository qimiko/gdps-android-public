cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANDROID_STL gnustl_static)

project(aurav2 VERSION 8.1.4)

option(GDMOD_USE_IMGUI "Enable ImGui and logging constants" OFF)
option(GDMOD_USE_SPDLOG "Enable spdlog" OFF)

set(GDMOD_SOURCES src/base/module.cpp
  src/base/hooks.cpp
  src/base/hook_handler.cpp
  src/base/jni_funcs.cpp
  src/aura.cpp
  src/classes/advancedcolorselectwidget.cpp
  src/classes/crashhelperpopup.cpp
  src/classes/tokenresponsehandler.cpp
  src/classes/controlleroverlay.cpp
  src/classes/loadingfailedhijacklayer.cpp
  src/classes/callbacks/showloadingcirclecallback.cpp
  src/classes/callbacks/reloadgamecallback.cpp
  src/classes/callbacks/inputcallback.cpp
  src/classes/callbacks/levelimportcallback.cpp
  src/classes/managers/controllermanager.cpp
  src/classes/managers/outfitmanager.cpp
  src/classes/speedhack/speedhackmanagercard.cpp
  src/classes/settings/settingspage.cpp
  src/classes/settings/togglesettingspage.cpp
  src/classes/settings/settingspopup.cpp
  src/classes/settings/texturesettingspopup.cpp
  src/classes/settings/pages/aboutsettingspage.cpp
  src/classes/settings/pages/gameplaysettingspage.cpp
  src/classes/settings/pages/interfacesettingspage.cpp
  src/classes/settings/pages/tweakssettingspage.cpp
  src/classes/settings/pages/editorsettingspage.cpp
  src/classes/settings/pages/songssettingspage.cpp
  src/classes/settings/pages/graphicssettingspage.cpp
  src/modules/default.cpp
  src/modules/levels.cpp
  src/modules/hacks.cpp
  src/modules/hacks/speedhack.cpp
  src/modules/limits.cpp
  src/modules/textures.cpp
  src/modules/badges.cpp
  src/modules/search.cpp
  src/modules/settings.cpp
  src/modules/urls.cpp
  src/modules/share.cpp
  src/modules/icons.cpp
  src/modules/input.cpp
  src/modules/editor.cpp
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

  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(aurav2 PRIVATE ${GDMOD_INCLUDE})

find_package(Git)
execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --always --abbrev=7 --dirty=+ --exclude \\*
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(COMMAND
  "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/base/cmake_config.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/base/cmake_config.hpp" @ONLY)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME rapidjson
  GIT_TAG 8261c1ddf43f10de00fd8c9a67811d1486b2c784
  GITHUB_REPOSITORY Tencent/rapidjson
  DOWNLOAD_ONLY YES
)

target_include_directories(aurav2
	PRIVATE
	$<BUILD_INTERFACE:${rapidjson_SOURCE_DIR}>/include/
)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
target_link_libraries(aurav2 fmt)

# fix dobby build with clang
add_compile_definitions(typeof=__typeof__)
CPMAddPackage(
  NAME dobby
  GIT_TAG 6a80a61ea062424d1595613058a174c64454fa57
  GITHUB_REPOSITORY jmpews/Dobby
  OPTIONS "DOBBY_DEBUG OFF" "DOBBY_GENERATE_SHARED OFF"
)
target_link_libraries(aurav2 dobby)

if (GDMOD_USE_IMGUI)
  target_compile_definitions(aurav2 PRIVATE -DGDMOD_ENABLE_LOGGING)

  add_subdirectory(cmake/external/imgui)
  target_link_libraries(aurav2 ImGui)
endif()

if (GDMOD_USE_SPDLOG)
  target_compile_definitions(aurav2 PRIVATE -DGDMOD_ENABLE_SPDLOG)
  CPMAddPackage(
    GITHUB_REPOSITORY gabime/spdlog
    VERSION 1.11.0
    OPTIONS "SPDLOG_FMT_EXTERNAL ON"
  )

  target_link_libraries(aurav2 spdlog)
endif()

if (GDMOD_USE_CRASH_DUMPS)
  target_compile_definitions(aurav2 PRIVATE -DGDMOD_ENABLE_CRASH_DUMPS)
  add_definitions(-D__STDC_FORMAT_MACROS)

  add_subdirectory(cmake/external/breakpad)
  target_link_libraries(aurav2 Breakpad)
endif()

if (GDMOD_CRASH_UPLOAD_URL)
  target_compile_definitions(aurav2 PRIVATE "-DGDMOD_CRASH_UPLOAD_URL=\"${GDMOD_CRASH_UPLOAD_URL}\"")
endif()

if (GDMOD_SOURCE_URL)
  target_compile_definitions(aurav2 PRIVATE "-DGDMOD_SOURCE_URL=\"${GDMOD_SOURCE_URL}\"")
endif()

target_link_libraries(aurav2 ${CMAKE_CURRENT_SOURCE_DIR}/external/lib/libcocos2dcpp.so)
target_link_libraries(aurav2 ${CMAKE_CURRENT_SOURCE_DIR}/external/lib/libfmod.so)
target_link_libraries(aurav2 android)
