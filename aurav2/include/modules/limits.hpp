#pragma once
#ifndef MODULES_LIMITS_HPP
#define MODULES_LIMITS_HPP

#include <vector>

#include "base/module.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

namespace Limits
{
	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
} // namespace Limits

#endif