#pragma once
#ifndef _SPRITES_CCSPRITEPLUS_HPP
#define _SPRITES_CCSPRITEPLUS_HPP

#include <cocos2d.h>

class CCSpritePlus : public cocos2d::CCSprite {
private:
    bool hasFollowers;
    cocos2d::CCArray* followers;

    CCSpritePlus* following;
};

#endif
