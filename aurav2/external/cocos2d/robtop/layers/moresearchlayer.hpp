#pragma once
#ifndef _LAYERS_MORESEARCHLAYER_HPP
#define _LAYERS_MORESEARCHLAYER_HPP

#include <cocos2d.h>

class MoreSearchLayer : public FLAlertLayer {
public:
    bool init();
    cocos2d::CCMenuItemToggle*
    createToggleButton(std::string name, cocos2d::SEL_MenuHandler onToggle,
        bool toggled, cocos2d::CCMenu* menu,
        cocos2d::CCPoint location);
};

#endif
