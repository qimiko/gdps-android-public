#pragma once
#ifndef _MANAGERS_GAMELEVELMANAGER_HPP
#define _MANAGERS_GAMELEVELMANAGER_HPP

#include <cocos2d.h>

#include <robtop/enums/gjhttptype.hpp>
#include <robtop/objects/gjgamelevel.hpp>
#include <robtop/objects/gjsearchobject.hpp>

class GameLevelManager : public cocos2d::CCNode {
public:
    static GameLevelManager* sharedState();
    GJGameLevel* getMainLevel(int, bool);
    void setMainLevels(cocos2d::CCDictionary*);
    cocos2d::CCDictionary* getMainLevels() const;

    bool getBoolForKey(const char*);
    void setBoolForKey(bool, const char*);

    void getOnlineLevels(GJSearchObject*);
    void ProcessHttpRequest(std::string url, std::string postdata,
        std::string key, GJHttpType);

    GJGameLevel* getSavedLevel(int);
};

#endif
