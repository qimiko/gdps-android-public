#pragma once
#ifndef _MANAGERS_GAMETOOLBOX_HPP
#define _MANAGERS_GAMETOOLBOX_HPP

#include <cocos2d.h>

#include <robtop/sprites/ccmenuitemtoggler.hpp>

class GameToolbox {
public:
    static bool doWeHaveInternet();
    static CCMenuItemToggler* createToggleButton(
        std::string label, cocos2d::SEL_MenuHandler callback, bool active, cocos2d::CCMenu*,
        cocos2d::CCPoint position, cocos2d::CCNode*, cocos2d::CCNode*);
    static CCMenuItemToggler* createToggleButton(
        std::string label, cocos2d::SEL_MenuHandler callback, bool active, cocos2d::CCMenu* toggleLayer,
        cocos2d::CCPoint position, cocos2d::CCNode* callbackObject, cocos2d::CCNode* labelLayer,
        float toggleScale, float labelMaxScale, float labelLimit,
        cocos2d::CCPoint labelOffset, const char* font, bool labelOverToggle);
};

#endif
