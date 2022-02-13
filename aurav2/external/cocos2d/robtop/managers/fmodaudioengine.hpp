#pragma once
#ifndef _MANAGERS_FMODAUDIOENGINE_HPP
#define _MANAGERS_FMODAUDIOENGINE_HPP

#include <cocos2d.h>
#include <fmod.hpp>

class FMODAudioEngine : public cocos2d::CCNode {
public:
    static FMODAudioEngine* sharedEngine();
};

#endif
