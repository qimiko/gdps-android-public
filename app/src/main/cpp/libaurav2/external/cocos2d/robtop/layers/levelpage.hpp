#pragma once
#ifndef _LAYERS_LEVELPAGE_HPP
#define _LAYERS_LEVELPAGE_HPP

#include <cocos2d.h>

class LevelPage : public cocos2d::CCLayer {
public:
    void updateDynamicPage(GJGameLevel*);
    void onInfo(cocos2d::CCObject*);
};

#endif
