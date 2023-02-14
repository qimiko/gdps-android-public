#pragma once

#ifndef _LAYERS_RETRYLEVELLAYER_HPP
#define _LAYERS_RETRYLEVELLAYER_HPP

#include <robtop/layers/gjdropdownlayer.hpp>

class RetryLevelLayer : public GJDropDownLayer {
public:
    void onReplay(cocos2d::CCObject*);
};

#endif //_LAYERS_RETRYLEVELLAYER_HPP
