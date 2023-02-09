#pragma once
#ifndef MODULES_SHARE_HPP
#define MODULES_SHARE_HPP

#include <support/base64.h>
#include <cstdio>

#include "base/module.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"
#include "base/jni_funcs.hpp"

namespace Share
{
    class Module : public Base::Module
    {
    protected:
        void on_initialize();
    };
} // namespace Limits

#endif