#pragma once
#ifndef _LAYERS_PAUSELAYER_HPP
#define _LAYERS_PAUSELAYER_HPP

#include <cocos2d.h>

class PauseLayer : public cocos2d::CCLayer {
public:
    void init(bool);
    void customSetup();
    void onRestart(cocos2d::CCObject*);

    void createToggleButton(std::string name, cocos2d::SEL_MenuHandler onToggle,
        bool toggled, cocos2d::CCMenu* menu,
        cocos2d::CCPoint location);

    void onProgressBar(cocos2d::CCObject*);
};

#endif
