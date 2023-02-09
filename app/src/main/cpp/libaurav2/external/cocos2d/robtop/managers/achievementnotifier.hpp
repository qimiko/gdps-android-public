#pragma once
#ifndef _MANAGERS_ACHIEVEMENTNOTIFIER_HPP
#define _MANAGERS_ACHIEVEMENTNOTIFIER_HPP

#include <cocos2d.h>

class AchievementNotifier : public cocos2d::CCNode {
public:
    static AchievementNotifier* sharedState();
    void notifyAchievement(const char* title, const char* description,
        const char* icon);
};

#endif
