#pragma once

#ifndef CLASSES_CALLBACKS_RELOADGAMECALLBACK_HPP
#define CLASSES_CALLBACKS_RELOADGAMECALLBACK_HPP

#include <cocos2dcpp.h>
#include <jni/JniHelper.h>

class ReloadGameCallback : public cocos2d::CCNode {

public:
    void onReload(cocos2d::CCNode*);
    CREATE_FUNC(ReloadGameCallback)
};

#endif //CLASSES_CALLBACKS_RELOADGAMECALLBACK_HPP
