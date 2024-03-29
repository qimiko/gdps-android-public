#pragma once
#ifndef _MANAGERS_PLATFORMTOOLBOX_HPP
#define _MANAGERS_PLATFORMTOOLBOX_HPP

class PlatformToolbox {
public:
    static bool isNetworkAvailable();
    static void reportLoadingFinished();
    static bool isControllerConnected();
    static void onNativePause();
    static bool shouldResumeSound();
    static void onNativeResume();
    static void onToggleKeyboard();
};

#endif
