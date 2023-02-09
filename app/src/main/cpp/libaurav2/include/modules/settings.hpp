#pragma once
#ifndef MODULES_SETTINGS_HPP
#define MODULES_SETTINGS_HPP

#include <cocos2dcpp.h>
#include <CCFileUtilsAndroid.h>
#include <dirent.h>

#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "base/hooks.hpp"
#include "base/module.hpp"
#include "base/jni_funcs.hpp"
#include "base/game_variables.hpp"

#include "classes/settings/settingspopup.hpp"

#include <fmt/format.h>

namespace Settings {
    class Module : public Base::Module {
    protected:
        void on_initialize();
    };
}

#endif