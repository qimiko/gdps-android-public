#include "base/hook_handler.hpp"

HookHandler& HookHandler::get_handler()
{
    static HookHandler handler_instance;
    return handler_instance;
}

void HookHandler::_add_hook_from_list(uintptr_t orig, uintptr_t hook, uintptr_t trampoline, int32_t priority)
{
    this->_get_hook_list_by_priority(priority).push_back({ orig, hook, trampoline });
}

void HookHandler::_resolve_hook_to_map(Hook& hook, _internal_hook_t& hook_list)
{
    // conflict found :open_mouth:
    if (auto conflict_hook = hook_list.find(hook.orig_fn);
        __builtin_expect(conflict_hook != hook_list.end(), 0)) {
        hook.orig_fn = conflict_hook->second.first;
    }

    hook_list.insert({ hook.orig_fn, { hook.hook_fn, hook.trampoline_fn } });
}

uintptr_t HookHandler::_get_orig_fn(uintptr_t hook_fn) {
    return this->_orig_fn_list[hook_fn];
}

void HookHandler::install_hooks()
{
    _internal_hook_t final_list;

    // convert unordered map to vector so we can add ordering
    std::vector<std::pair<int, std::vector<Hook>>> v(this->_hook_list.begin(), this->_hook_list.end());

    // sort by priority
    std::sort(v.begin(), v.end(), [](const auto& le, const auto& re) {
        return le.first < re.first;
    });

    // double iterator! the vector contains the priority and a hook vector
    for (auto& hook_pair : v) {
        for (auto& hook : hook_pair.second) {
            this->_resolve_hook_to_map(hook, final_list);
        }
    }

    for (const auto& hook : final_list) {
        this->_orig_fn_list.insert({ hook.second.first, hook.second.second });
        create_hook(hook.first, hook.second.first, hook.second.second);
    }
}