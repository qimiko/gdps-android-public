#pragma once
#ifndef MODULES_OVERLAY_HPP
#define MODULES_OVERLAY_HPP

#include <cocos2dcpp.h>
#include <jni/JniHelper.h>
#include <jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h>

#include <cxxabi.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_android.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <android/native_window_jni.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ringbuffer_sink.h>

#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "base/hooks.hpp"
#include "base/module.hpp"

#include "modules/settings.hpp"

namespace Overlay {
class Module : public Base::Module {
protected:
    void on_initialize();
};
}

#endif