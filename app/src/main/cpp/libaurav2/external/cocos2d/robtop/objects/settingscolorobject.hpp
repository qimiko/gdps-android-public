#pragma once
#ifndef _OBJECTS_SETTINGSCOLOROBJECT_HPP
#define _OBJECTS_SETTINGSCOLOROBJECT_HPP

class SettingsColorObject : public cocos2d::CCNode {
	CC_SYNTHESIZE(cocos2d::ccColor3B, color_, Color);
	CC_SYNTHESIZE(bool, blend_, Blend);
	CC_SYNTHESIZE(int, custom_, Custom); // pcol
};

#endif
