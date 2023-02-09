#pragma once
#ifndef _MANAGERS_FMODAUDIOENGINE_HPP
#define _MANAGERS_FMODAUDIOENGINE_HPP

#include <cocos2d.h>
#include <fmod.hpp>

class FMODAudioEngine : public cocos2d::CCNode {
public:
    static FMODAudioEngine* sharedEngine();

    void playBackgroundMusic(std::string path, bool loop, bool paused);
    void pauseBackgroundMusic();
    void stopBackgroundMusic(bool);
    bool isBackgroundMusicPlaying();

    void setTimeOffset(int);
    int getTimeOffset() const;

    void update(float dt) override;
};

#endif
