#pragma once
#ifndef MODULES_SEARCH_HPP
#define MODULES_SEARCH_HPP

#include <vector>
#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/config.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

#include "classes/extensions/gjsearchobjectext.hpp"

namespace Search
{
	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
}

#endif