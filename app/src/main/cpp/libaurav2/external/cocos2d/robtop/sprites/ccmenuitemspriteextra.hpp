#pragma once
#ifndef _SPRITES_CCMENUITEMSPRITEEXTRA_HPP
#define _SPRITES_CCMENUITEMSPRITEEXTRA_HPP

#include <cocos2d.h>

class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
public:
    static CCMenuItemSpriteExtra* create(CCNode* normalSprite,
        cocos2d::CCNode* selectedSprite,
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler selector);
    void setSizeMult(float);
    void setScaleVar(float);
};

#endif
