#pragma once
#ifndef HOOKS_HPP
#define HOOKS_HPP

#include <algorithm>
#include <fstream>
#include <cstring> // memcpy
#include <dlfcn.h>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

#include <cocos2dcpp.h>
#include <dobby.h>

extern void* GLOBAL_BASE;

bool create_hook(uintptr_t target_fn, uintptr_t hook_fn, uintptr_t o_fn);
[[deprecated("this fn currently doesn't work with new hook system")]] bool remove_hook(uintptr_t addr);

void setup_handle();
void* get_sym_addr(const char*);

uintptr_t get_global_base();

uintptr_t page_of(uintptr_t addr);
bool patch_single_byte(uint8_t* addr, uint8_t byte);
bool patch_multiple_byte(uint8_t* addr, std::vector<uint8_t> bytes);

struct Hook {
    uintptr_t orig_fn;
    uintptr_t hook_fn;
    uintptr_t trampoline_fn;
};

template <typename T>
inline T* ptr_to_offset(void* base, unsigned int offset)
{
    return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(base) + offset);
};

template <typename T>
inline T get_from_offset(void* base, unsigned int offset)
{
    return *ptr_to_offset<T>(base, offset);
};

#endif
