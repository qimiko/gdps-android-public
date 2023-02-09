#pragma once
#ifndef _OBJECTS_LEVELSETTINGSOBJECT_HPP
#define _OBJECTS_LEVELSETTINGSOBJECT_HPP

#include <robtop/objects/settingscolorobject.hpp>
#include <robtop/objects/gjgamelevel.hpp>

class LevelSettingsObject : public cocos2d::CCNode {
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, backgroundColor_, BackgroundColor);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, groundColor_, GroundColor);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, lineColor_, LineColor);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, objectColor_, ObjectColor);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, dLineColor_, DLineColor); // not even kidding, that's how rob names it
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, customColor01_, CustomColor01);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, customColor02_, CustomColor02);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, customColor03_, CustomColor03);
    CC_SYNTHESIZE_RETAIN(SettingsColorObject*, customColor04_, CustomColor04);
    CC_SYNTHESIZE(int, startMode_, StartMode);
    CC_SYNTHESIZE(int, startSpeed_, StartSpeed);
    CC_SYNTHESIZE(bool, startMiniMode_, StartMiniMode);
    CC_SYNTHESIZE(bool, startDualMode_, StartDualMode);
    CC_SYNTHESIZE(bool, twoPlayerMode_, TwoPlayerMode);
    CC_SYNTHESIZE(float, songOffset_, SongOffset);
    CC_SYNTHESIZE(bool, fadeIn_, FadeIn);
    CC_SYNTHESIZE(bool, fadeOut_, FadeOut);
    CC_SYNTHESIZE(int, BGIdx_, BGIdx);
    CC_SYNTHESIZE(int, GIdx_, GIdx);
    CC_SYNTHESIZE(bool, isLimited_, IsLimited);
    CC_SYNTHESIZE(bool, isGravityFlipped_, IsGravityFlipped);
    CC_SYNTHESIZE(GJGameLevel*, level_, Level);
    CC_SYNTHESIZE(std::string, songString_, SongString);
    CC_SYNTHESIZE(bool, songChanged_, SongChanged);
};

#endif