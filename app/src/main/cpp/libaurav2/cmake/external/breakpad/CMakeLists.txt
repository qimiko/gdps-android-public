cmake_minimum_required(VERSION 3.5)
project(Breakpad)
enable_language(ASM)

# Attempt to recreate the Android build file in CMake

include(../../CPM.cmake)
CPMAddPackage(
	NAME breakpad
	VERSION 2021.08.09
	GIT_REPOSITORY https://chromium.googlesource.com/breakpad/breakpad.git
)

if (breakpad_ADDED)
	include(FetchContent)

	FetchContent_Declare(lss
			GIT_REPOSITORY https://chromium.googlesource.com/linux-syscall-support.git
			GIT_TAG e1e7b0ad8ee99a875b272c8e33e308472e897660
			SOURCE_DIR ${breakpad_SOURCE_DIR}/src/third_party/lss
	)

	FetchContent_MakeAvailable(lss)

	add_library(Breakpad STATIC
		${breakpad_SOURCE_DIR}/src/client/linux/crash_generation/crash_generation_client.cc
		${breakpad_SOURCE_DIR}/src/client/linux/dump_writer_common/thread_info.cc
		${breakpad_SOURCE_DIR}/src/client/linux/dump_writer_common/ucontext_reader.cc
		${breakpad_SOURCE_DIR}/src/client/linux/handler/exception_handler.cc
		${breakpad_SOURCE_DIR}/src/client/linux/handler/minidump_descriptor.cc
		${breakpad_SOURCE_DIR}/src/client/linux/log/log.cc
		${breakpad_SOURCE_DIR}/src/client/linux/microdump_writer/microdump_writer.cc
		${breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/linux_dumper.cc
		${breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
		${breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/minidump_writer.cc
		${breakpad_SOURCE_DIR}/src/client/minidump_file_writer.cc
		${breakpad_SOURCE_DIR}/src/common/convert_UTF.cc
		${breakpad_SOURCE_DIR}/src/common/md5.cc
		${breakpad_SOURCE_DIR}/src/common/string_conversion.cc
		${breakpad_SOURCE_DIR}/src/common/linux/breakpad_getcontext.S
		${breakpad_SOURCE_DIR}/src/common/linux/elfutils.cc
		${breakpad_SOURCE_DIR}/src/common/linux/file_id.cc
		${breakpad_SOURCE_DIR}/src/common/linux/guid_creator.cc
		${breakpad_SOURCE_DIR}/src/common/linux/linux_libc_support.cc
		${breakpad_SOURCE_DIR}/src/common/linux/memory_mapped_file.cc
		${breakpad_SOURCE_DIR}/src/common/linux/safe_readlink.cc
	)

	target_include_directories(Breakpad PUBLIC
		${breakpad_SOURCE_DIR}/src/common/android/include/
		${breakpad_SOURCE_DIR}/src/
		${breakpad_SOURCE_DIR}/src/common/android/include/
	)
endif()

