#pragma once
#ifndef _MANAGERS_APPDELEGATE_HPP
#define _MANAGERS_APPDELEGATE_HPP

#include <cocos2d.h>

class AppDelegate : public cocos2d::CCApplicationProtocol {
public:
    static AppDelegate* get();

    float bgScale();

    void resumeSound();

    void pauseGame();

    bool getPaused() const;

    bool getLoadingFinished() const;

    cocos2d::CCNode* getScenePointer() const;

    bool getIsIOS() const;
};

#endif
