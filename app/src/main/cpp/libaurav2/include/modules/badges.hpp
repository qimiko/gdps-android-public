#pragma once
#ifndef MODULES_BADGES_HPP
#define MODULES_BADGES_HPP

#include <bitset>
#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/config.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

#include "classes/extensions/gjcommentext.hpp"

namespace Badges {
class Module : public Base::Module
{
protected:
	void on_initialize();
};
}

#endif