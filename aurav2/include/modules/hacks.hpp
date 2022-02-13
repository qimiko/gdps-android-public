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

namespace Hacks {
class PlayLayer_ext : public cocos2d::CCObject {
    CC_SYNTHESIZE(bool, isIllegitimate_, IsIllegitimate);
};

class Module : public Base::Module {
protected:
    void on_initialize();
};
}

#endif