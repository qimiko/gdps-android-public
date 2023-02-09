#pragma once
#ifndef MODULES_ICONS_HPP
#define MODULES_ICONS_HPP

#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/hook_handler.hpp"

#include "classes/managers/outfitmanager.hpp"

namespace Icons
{
    class Module : public Base::Module
    {
    protected:
        void on_initialize();
    };
}

#endif
