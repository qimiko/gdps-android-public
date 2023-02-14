#pragma once
#ifndef MODULES_INPUT_HPP
#define MODULES_INPUT_HPP

#include <unordered_map>
#include <android/keycodes.h>
#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/hook_handler.hpp"
#include "base/jni_funcs.hpp"
#include "base/game_variables.hpp"

#include "classes/managers/controllermanager.hpp"

namespace Input
{
    class Module : public Base::Module
    {
    protected:
        void on_initialize();
    };
}

#endif //MODULES_INPUT_HPP
