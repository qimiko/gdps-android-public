#pragma once

#ifndef CLASSES_LOADINGFAILEDHIJACKLAYER_HPP
#define CLASSES_LOADINGFAILEDHIJACKLAYER_HPP

#include <cocos2dcpp.h>

#include "base/spdlog.hpp"
#include "base/hook_handler.hpp"

class LoadingFailedHijackLayer : public cocos2d::CCLayer, FLAlertLayerProtocol {
public:
    static bool gHasDestroyedState;

public:

    static bool hasDestroyedState();

    void FLAlert_Clicked(FLAlertLayer*, bool) override;
    bool init() override;

    void showLoadFailedDialog();

    void onEnterDelayFinished(cocos2d::CCNode*);
    void onEnter() override;

    static void destroyGameState();
    static void restoreGameState();

    static cocos2d::CCScene* scene();

    CREATE_FUNC(LoadingFailedHijackLayer)
};

#endif //CLASSES_LOADINGFAILEDHIJACKLAYER_HPP
