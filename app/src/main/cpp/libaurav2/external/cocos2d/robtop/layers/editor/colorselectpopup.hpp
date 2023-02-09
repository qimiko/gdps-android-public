#pragma once
#ifndef _LAYERS_EDITOR_COLORSELECTPOPUP_HPP
#define _LAYERS_EDITOR_COLORSELECTPOPUP_HPP

#include <cocos2d.h>

#include <robtop/layers/flalertlayer.hpp>
#include <robtop/objects/gameobject.hpp>

class ColorSelectPopup : public FLAlertLayer {
public:
    bool init(GameObject* target, int colorID, int copyPlayerID, int isBlending);

    cocos2d::ccColor3B getColorValue();
    void selectColor(cocos2d::ccColor3B color);
    void colorValueChanged(cocos2d::ccColor3B color);
};

#endif
