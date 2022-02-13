#pragma once
#ifndef _LAYERS_LEVELINFOLAYER_HPP
#define _LAYERS_LEVELINFOLAYER_HPP

#include <cocos2d.h>

class LevelInfoLayer : public cocos2d::CCLayer {
public:
    int init(GJGameLevel*);
    void setupLevelInfo();
    void onPlayReplay(cocos2d::CCObject*);
};

#endif
