#pragma once
#ifndef _SPRITES_TEXTAREA_HPP
#define _SPRITES_TEXTAREA_HPP

#include <cocos2d.h>

class TextArea : public cocos2d::CCSprite {
public:
    void colorAllCharactersTo(cocos2d::_ccColor3B);
    void setString(const char*);
};

#endif