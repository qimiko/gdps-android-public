#ifndef _MODULES_HACKS_SPEEDHACK_HPP
#define _MODULES_HACKS_SPEEDHACK_HPP

#include <cocos2dcpp.h>

#include "base/hook_handler.hpp"
#include "base/module.hpp"
#include "base/game_variables.hpp"

#include "classes/speedhack/speedhackmanagercard.hpp"
#include "classes/speedhack/speedhacklifecyclewatcher.hpp"

#include <fmt/format.h>

namespace Hacks::Speedhack {
    class Module : public Base::Module {
    protected:
        void on_initialize();
    };
}

#endif //_MODULES_HACKS_SPEEDHACK_HPP
