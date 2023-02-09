#pragma once
#ifndef _MANAGERS_GAMESOUNDMANAGER_HPP
#define _MANAGERS_GAMESOUNDMANAGER_HPP

#include <cocos2d.h>

class GameSoundManager : public cocos2d::CCNode {
public:
    static GameSoundManager* sharedManager();
	void enableMetering();
	void disableMetering();

	void playBackgroundMusic(std::string, bool, bool);
	void stopBackgroundMusic();
};

#endif