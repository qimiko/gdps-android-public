#pragma once
#ifndef _MANAGERS_LOCALLEVELMANAGER_HPP
#define _MANAGERS_LOCALLEVELMANAGER_HPP

#include <cocos2d.h>
#include <robtop/objects/gjgamelevel.hpp>

class LevelTools {
public:
    static std::string getAudioTitle(int);
    static std::string getAudioFileName(int);
    static int artistForAudio(int);
    static std::string urlForAudio(int);
    static int getAudioBPM(int);

    static std::string fbURLForArtist(int);
    static std::string ngURLForArtist(int);
    static std::string ytURLForArtist(int);
    static std::string nameForArtist(int);

    static GJGameLevel* getLevel(int, bool isVerify);
};

class LocalLevelManager {
public:
    static LocalLevelManager* sharedState();
    std::string getMainLevelString(int levelID);
};

#endif
