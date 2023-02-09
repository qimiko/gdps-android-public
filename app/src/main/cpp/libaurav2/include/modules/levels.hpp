#pragma once
#ifndef MODULES_LEVELS_HPP
#define MODULES_LEVELS_HPP

#include <vector>
#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/config.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

namespace Levels {
	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
}

#endif