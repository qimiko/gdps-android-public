#pragma once
#ifndef _MANAGERS_GAMEMANAGER_HPP
#define _MANAGERS_GAMEMANAGER_HPP

#include <cocos2d.h>
#include <robtop/layers/playlayer.hpp>

#include "gmanager.hpp"

class GameManager : public GManager {
public:
    static GameManager* sharedState();

    void fadeInMusic(const char* title);

    bool isColorUnlocked(int id, bool colorType);
    bool isIconUnlocked(int id, IconType iconType);

    bool getClickedEditor() const;
    void setClickedEditor(bool);

    PlayLayer* getPlayLayer() const;

    bool getShowedPirate();
    void setShowedPirate(bool);

    void toggleGameVariable(const char* variableName);
    void setGameVariable(const char* variableName, bool);
    bool getGameVariable(const char* variableName);

    void setIntGameVariable(const char* keyName, int);
    int getIntGameVariable(const char* keyName);

    void setEditorCopyString(std::string);
    std::string getEditorCopyString() const;

    void returnToLastScene();

    void firstLoad();

    bool getShowProgressBar() const;
    void setShowProgressBar(bool);

    void reloadAll(bool, bool, bool);

    void reloadAllStep2();
    void reloadAllStep3();
    void reloadAllStep4();
    void reloadAllStep5();

    cocos2d::TextureQuality getTexQuality() const;
    void setTexQuality(cocos2d::TextureQuality);
};

#endif
