#pragma once
#ifndef _ITEMS_SEARCHBUTTON_HPP
#define _ITEMS_SEARCHBUTTON_HPP

class SearchButton : public cocos2d::CCSprite {
public:
    static SearchButton* create(const char* buttonTexture, const char* label, float labelScale, const char* icon);
};

#endif
