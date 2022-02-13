#pragma once
#ifndef MODULES_DEFAULT_HPP
#define MODULES_DEFAULT_HPP

#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <array>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <android/keycodes.h>

#include <cocos2dcpp.h>
#include <support/zip_support/ZipUtils.h>

#include "base/module.hpp"
#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

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