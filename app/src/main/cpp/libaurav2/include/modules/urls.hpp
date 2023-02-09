#pragma once
#ifndef MODULES_URLS_HPP
#define MODULES_URLS_HPP

#include <cocos2dcpp.h>
#include <support/base64.h>
#include <support/zip_support/ZipUtils.h>

#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "base/hooks.hpp"
#include "base/module.hpp"
#include "base/jni_funcs.hpp"

#include "classes/loadingfailedhijacklayer.hpp"

namespace Urls {
class Module : public Base::Module {
protected:
    void on_initialize();
};
}

#endif