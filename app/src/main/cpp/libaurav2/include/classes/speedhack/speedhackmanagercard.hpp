#pragma once

#ifndef CLASSES_SPEEDHACK_SPEEDHACKMANAGERCARD_HPP
#define CLASSES_SPEEDHACK_SPEEDHACKMANAGERCARD_HPP

#include <cocos2dcpp.h>
#include <fmt/format.h>

#include "base/spdlog.hpp"
#include "base/game_variables.hpp"

#include "classes/extensions/playlayerext.hpp"

class SpeedhackManagerCard : public cocos2d::CCNode {
private:
    static bool gHackActive_;
    static int gLifetimeCounter_;
    cocos2d::CCLabelBMFont* externalLabel_{nullptr};
    cocos2d::CCLabelBMFont* optionsLabel_{nullptr};

    static float getIntervalForValue(float value);
    static int getSaveValueForInterval(int old_val, int change);

    static void setGlobalTimeScale(float scale);

    static void setSpeedhackActive(bool active);

public:

    static void decrementLifetime();
    static void incrementLifetime();

    static bool isSpeedhackActive();
    static float getSpeedhackInterval();
    static void setSpeedhackValue(float interval);

    void setExternalLabel(cocos2d::CCLabelBMFont* label);
    void updateSpeedhackLabel();

    void onBtnDown(cocos2d::CCObject * /* target */);
    void onBtnUp(cocos2d::CCObject * /* target */);

    bool init() override;

    CREATE_FUNC(SpeedhackManagerCard);
};

#endif //CLASSES_SPEEDHACK_SPEEDHACKMANAGERCARD_HPP
