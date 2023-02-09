#pragma once
#ifndef MODULES_DEFAULT_HPP
#define MODULES_DEFAULT_HPP

#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <array>
#include <vector>

#include <cocos2dcpp.h>
#include <support/zip_support/ZipUtils.h>

#include "classes/crashhelperpopup.hpp"

#include "base/module.hpp"
#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"
#include "base/jni_funcs.hpp"
#include "base/game_variables.hpp"

#include "token.hpp"

namespace Default
{
	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
}

#endif