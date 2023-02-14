#pragma once
#ifndef MODULES_HACKS_HPP
#define MODULES_HACKS_HPP

#include <algorithm>
#include <cocos2dcpp.h>
#include <sstream>
#include <vector>

#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "base/hooks.hpp"
#include "base/module.hpp"
#include "base/jni_funcs.hpp"
#include "base/game_variables.hpp"

#include "classes/managers/controllermanager.hpp"
#include "classes/extensions/playlayerext.hpp"

#include "hacks/speedhack.hpp"

#include <fmt/format.h>

namespace Hacks {


class Module : public Base::Module {
protected:
    void on_initialize();
};
}

#endif