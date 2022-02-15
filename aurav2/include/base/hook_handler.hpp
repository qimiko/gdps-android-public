#pragma once
#ifndef HOOK_HANDLER_HPP
#define HOOK_HANDLER_HPP

#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#if GDMOD_ENABLE_SPDLOG
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#else
#include "spdlog_fill.hpp"
#endif

#include "config.hpp"
#include "hooks.hpp"

class HookHandler {
private:
    template <typename T>
    struct member_type : std::false_type {
    };

    template <typename R, typename C, typename... A>
    struct member_type<R (C::*)(A...)> {
        typedef C type;
    };

protected:
    void _add_hook_from_list(uintptr_t orig, uintptr_t hook, uintptr_t trampoline, int32_t priority = 0);
    uintptr_t _get_orig_fn(uintptr_t hook_fn);

    // <orig_fn_addr, <hook_fn_addr, trampoline_fn_addr>>
    typedef std::unordered_map<uintptr_t, std::pair<uintptr_t, uintptr_t>> _internal_hook_t;

    std::unordered_map<int, std::vector<Hook>> _hook_list;
    std::unordered_map<uintptr_t /* hook_fn */, uintptr_t /* orig_fn */> _orig_fn_list;

public:
    static HookHandler& get_handler();

    template <typename T>
    static inline uintptr_t resolve_address_from_fn(const T fn)
    {
        if constexpr (std::is_same<T, const char*>::value) {
            return reinterpret_cast<uintptr_t>(get_sym_addr(fn));
        } else if constexpr (std::is_member_function_pointer<T>::value) {
            // undefined behavior, this will lead to crashes if not careful
            union {
                T in;
                uintptr_t out;
            } u = { fn };

            uintptr_t ptr_addr = u.out;

            Dl_info fn_info;
            dladdr(reinterpret_cast<void*>(ptr_addr), &fn_info);

            if (__builtin_expect((ptr_addr <= reinterpret_cast<uintptr_t>(fn_info.dli_fbase) || fn_info.dli_fbase == nullptr), 0)) {
                // virtual function handling
                // this implementation is very compiler specific
                // probably crashes

                using Member = typename member_type<T>::type;

                if constexpr (std::is_destructible<Member>::value) {
                    auto class_inst = Member();
                    auto class_vftable = get_from_offset<uintptr_t*>(&class_inst, 0x0);
                    ptr_addr = get_from_offset<uintptr_t>(class_vftable, ptr_addr);
                }

            }

            return ptr_addr;
        } else {
            return reinterpret_cast<uintptr_t>(fn);
        }
    }

    // priority system is lower numbers take priority over lower ones, thus -1000 will probably always run first
    template <int32_t V = 0, typename T, typename U>
    inline HookHandler& add_hook(const T orig, const U* hook_fn)
    {
        if constexpr (Config::USE_LOGGING) {
            spdlog::get("global")->trace("adding hook for fn {}", typeid(T).name());
        }

        this->_add_hook_from_list(resolve_address_from_fn(orig), resolve_address_from_fn(hook_fn), reinterpret_cast<uintptr_t>(new uintptr_t()), V);
        return *this;
    };

    template <int32_t V = 0, typename T, typename U>
    inline HookHandler& add_hook(const T orig, const U hook_fn)
    {
        if constexpr (Config::USE_LOGGING) {
            spdlog::get("global")->trace("adding hook for fn {}", typeid(T).name());
        }

        this->_add_hook_from_list(resolve_address_from_fn(orig), resolve_address_from_fn(hook_fn), reinterpret_cast<uintptr_t>(new uintptr_t()), V);
        return *this;
    };

    template <const auto T, typename... Args>
    static inline auto orig(Args... fn_args) -> std::result_of_t<decltype(T)(Args...)>
    {
        auto resolved_addr = resolve_address_from_fn(T);
        auto orig_fn = *reinterpret_cast<uintptr_t*>(HookHandler::get_handler()._get_orig_fn(resolved_addr));

        // member function casts are fun (totally)
        union {
            uintptr_t in;
            decltype(T) out;
        } u = { orig_fn };

        auto orig_fn_callable = u.out;

        return std::bind(orig_fn_callable, fn_args...)();
    }

    HookHandler& install_patch(const uintptr_t rel_addr, const std::vector<uint8_t>& bytes)
    {
        auto patch_ptr = ptr_to_offset<uint8_t>(GLOBAL_BASE, rel_addr);

#if GDMOD_ENABLE_SPDLOG
        auto orig_bytes = std::vector<uint8_t>(
            patch_ptr, patch_ptr + (bytes.size() * sizeof(uint8_t))
        );

        spdlog::get("global")->trace("replacing bytes [{:#04x}] with [{:#04x}] for addr {:#x}", fmt::join(orig_bytes, ","), fmt::join(bytes, ","), rel_addr);
#endif

        if (!patch_multiple_byte(patch_ptr, bytes)) {
            spdlog::get("global")->warn("bytepatch for addr {} failed in some way..!", rel_addr);
        }

        return *this;
    }

    // this can and really only should be run once, beyond that and you'll overwrite things for no reason
    void install_hooks();

    HookHandler(HookHandler const&) = delete;
    void operator=(HookHandler const&) = delete;

protected:
    void _resolve_hook_to_map(Hook& hook, _internal_hook_t& hook_list);

    [[nodiscard]] std::vector<Hook>& _get_hook_list_by_priority(const int32_t pri)
    {
        // insert returns an <iterator, bool> even if keys conflict
        auto pri_it = this->_hook_list.insert({ pri, {} });

        // thus this strange access method lol
        return pri_it.first->second;
    }

private:
    HookHandler() { }
};

#endif