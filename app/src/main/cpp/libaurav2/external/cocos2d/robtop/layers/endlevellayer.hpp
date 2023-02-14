#pragma once

#ifndef _LAYERS_ENDLEVELLAYER_HPP
#define _LAYERS_ENDLEVELLAYER_HPP

#include <robtop/layers/gjdropdownlayer.hpp>

class EndLevelLayer : public GJDropDownLayer {
public:
    void onReplay(cocos2d::CCObject*);
};

#endif //_LAYERS_ENDLEVELLAYER_HPP
