#pragma once
#ifndef MODULES_TOKEN_HPP
#define MODULES_TOKEN_HPP

#include <vector>
#include <ctime>
#include <cocos2dcpp.h>

#include "base/module.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"
#include "base/game_variables.hpp"

#include "classes/extensions/gjaccountmanagerext.hpp"
#include "classes/tokenresponsehandler.hpp"

namespace Token
{
	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
}

#endif