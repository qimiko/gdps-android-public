#pragma once
#ifndef MODULES_OVERLAY_HPP
#define MODULES_OVERLAY_HPP

#include <cocos2dcpp.h>
#include <jni/JniHelper.h>
#include <jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h>

#include <cxxabi.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_android.h>
#include <backends/imgui_impl_opengl3.h>

#include <android/native_window_jni.h>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ringbuffer_sink.h>

#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "base/hooks.hpp"
#include "base/module.hpp"
#include "base/game_variables.hpp"

#include "classes/settings/settingspopup.hpp"

namespace Overlay {
class Module : public Base::Module {
protected:
    void on_initialize();
};
}

#endif