#pragma once
#ifndef _ITEMS_ACHIEVEMENTBAR_HPP
#define _ITEMS_ACHIEVEMENTBAR_HPP

#include <cocos2d.h>

class AchievementBar : public cocos2d::CCNode {
public:
    static AchievementBar* create(const char*, const char*, const char*);
    void show();
};

#endif
