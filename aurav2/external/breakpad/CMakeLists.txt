cmake_minimum_required(VERSION 3.5)
project(Breakpad)
enable_language(ASM)

# Attempt to recreate the Android build file in CMake

include(FetchContent)

FetchContent_Declare(lss
    GIT_REPOSITORY https://chromium.googlesource.com/linux-syscall-support.git
		GIT_TAG e1e7b0ad8ee99a875b272c8e33e308472e897660
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src/third_party/lss
)

FetchContent_MakeAvailable(lss)

add_library(Breakpad STATIC
  breakpad/src/client/linux/crash_generation/crash_generation_client.cc
	breakpad/src/client/linux/dump_writer_common/thread_info.cc
  breakpad/src/client/linux/dump_writer_common/ucontext_reader.cc
  breakpad/src/client/linux/handler/exception_handler.cc
  breakpad/src/client/linux/handler/minidump_descriptor.cc
  breakpad/src/client/linux/log/log.cc
  breakpad/src/client/linux/microdump_writer/microdump_writer.cc
	breakpad/src/client/linux/minidump_writer/linux_dumper.cc
	breakpad/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
	breakpad/src/client/linux/minidump_writer/minidump_writer.cc
	breakpad/src/client/minidump_file_writer.cc
	breakpad/src/common/convert_UTF.cc
	breakpad/src/common/md5.cc
	breakpad/src/common/string_conversion.cc
	breakpad/src/common/linux/breakpad_getcontext.S
	breakpad/src/common/linux/elfutils.cc
	breakpad/src/common/linux/file_id.cc
	breakpad/src/common/linux/guid_creator.cc
	breakpad/src/common/linux/linux_libc_support.cc
	breakpad/src/common/linux/memory_mapped_file.cc
	breakpad/src/common/linux/safe_readlink.cc
	)

target_include_directories(Breakpad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src/common/android/include/
	${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src/
	${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src/common/android/include/
)