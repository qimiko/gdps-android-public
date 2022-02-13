#pragma once
#ifndef MODULES_TEXTURES_HPP
#define MODULES_TEXTURES_HPP

#include <vector>
#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/config.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

namespace Textures
{
	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
} // namespace Textures

#endif
