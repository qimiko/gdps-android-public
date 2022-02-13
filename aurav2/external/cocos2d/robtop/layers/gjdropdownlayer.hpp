#pragma once
#ifndef _LAYERS_GJDROPDOWNLAYER_HPP
#define _LAYERS_GJDROPDOWNLAYER_HPP

#include <cocos2d.h>

class GJDropDownLayer : public cocos2d::CCLayerColor {
public:
    cocos2d::CCLayer* getInternalLayer() const;

    void showLayer(bool instant);
};

#endif
