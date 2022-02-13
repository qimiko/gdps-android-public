#pragma once
#ifndef _SPRITES_BUTTONSPRITE_HPP
#define _SPRITES_BUTTONSPRITE_HPP

#include <cocos2d.h>

class ButtonSprite : public cocos2d::CCSprite {
public:
    static ButtonSprite* create(const char* text);
    static ButtonSprite* create(const char* text,
        int buttonWidth,
        int widthLimit,
        float scale,
        bool limitWidth);
    static ButtonSprite*
    create(const char* text,
        int buttonWidth, // 0 by default
        int widthLimit, // seems to be 0 usually
        float scale, // might be width.
        bool limitWidth, // not actually sure, false by default
        const char* fontName, // font name
        const char* textureName, // might be texture name
        float labelHeight);
    void setColor(cocos2d::_ccColor3B);
};

#endif
