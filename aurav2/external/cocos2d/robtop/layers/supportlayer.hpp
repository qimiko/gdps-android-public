#pragma once
#ifndef _LAYERS_SUPPORTLAYER_HPP
#define _LAYERS_SUPPORTLAYER_HPP

#include "gjdropdownlayer.hpp"
#include <cocos2d.h>

class SupportLayer : public GJDropDownLayer {
public:
    void onRobTop();
    void customSetup();
    void createToggleButton(std::string name, cocos2d::SEL_MenuHandler onToggle,
        bool toggled, cocos2d::CCMenu* menu,
        cocos2d::CCPoint location);
};

#endif
