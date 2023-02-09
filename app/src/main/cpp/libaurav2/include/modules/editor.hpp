#pragma once
#ifndef MODULES_EDITOR_HPP
#define MODULES_EDITOR_HPP

#include <fmt/format.h>

#include "base/module.hpp"
#include "base/hook_handler.hpp"
#include "base/game_variables.hpp"

#include "classes/advancedcolorselectwidget.hpp"

namespace Editor
{
    class Module : public Base::Module
    {
    protected:
        void on_initialize();
    };
}

#endif //MODULES_EDITOR_HPP
