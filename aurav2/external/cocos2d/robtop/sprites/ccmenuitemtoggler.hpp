#pragma once
#ifndef _SPRITES_CCMENUITEMTOGGLER_HPP
#define _SPRITES_CCMENUITEMTOGGLER_HPP

#include <cocos2d.h>

class CCMenuItemToggler : public cocos2d::CCMenuItem {
public:
    static CCMenuItemToggler* create(cocos2d::CCNode* normalButton, cocos2d::CCNode* selectedButton, cocos2d::CCObject* rec, cocos2d::SEL_MenuHandler selector);
    void activate();
    cocos2d::CCNode* activeItem();

		bool getIsActive() const;

		void setDontToggle(bool);
		bool getDontToggle() const;

		void setEnabled(bool);

    void toggle(bool);
};

#endif