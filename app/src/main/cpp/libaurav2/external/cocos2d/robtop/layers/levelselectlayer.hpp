#pragma once
#ifndef _LAYERS_LEVELSELECTLAYER_HPP
#define _LAYERS_LEVELSELECTLAYER_HPP

#include <cocos2d.h>

class LevelSelectLayer : public cocos2d::CCLayer {
public:
    // don't ever actually call this but we want a good pointer to it
    bool init(int);
};

#endif
