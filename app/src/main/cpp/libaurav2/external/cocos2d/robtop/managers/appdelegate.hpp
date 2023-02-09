#pragma once
#ifndef _MANAGERS_APPDELEGATE_HPP
#define _MANAGERS_APPDELEGATE_HPP

#include <cocos2d.h>

class AppDelegate : public cocos2d::CCApplicationProtocol {
public:
    static AppDelegate* get();
    cocos2d::CCNode* getScenePointer() const;

    void trySaveGame();

    float bgScale();

    bool getIsIOS() const;
};

#endif
