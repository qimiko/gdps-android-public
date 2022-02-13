#pragma once
#ifndef _MANAGERS_OBJECTTOOLBOX_HPP
#define _MANAGERS_OBJECTTOOLBOX_HPP

#include <cocos2d.h>

class ObjectToolbox : public cocos2d::CCNode {
public:
    static ObjectToolbox* sharedState();
    bool init();
};

#endif