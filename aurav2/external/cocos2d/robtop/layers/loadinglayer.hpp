#pragma once
#ifndef _LAYERS_LOADINGLAYER_HPP
#define _LAYERS_LOADINGLAYER_HPP

#include <cocos2d.h>

class LoadingLayer : public cocos2d::CCLayer {
public:
    static const char* getLoadingString();
    void loadAssets();
    void loadingFinished();
};

#endif
