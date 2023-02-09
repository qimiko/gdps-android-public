#pragma once

#ifndef _LAYERS_LEVELBROWSERLAYER_HPP
#define _LAYERS_LEVELBROWSERLAYER_HPP

#include <cocos2d.h>

#include <robtop/objects/gjsearchobject.hpp>

class LevelBrowserLayer : public cocos2d::CCLayer {
public:
    static cocos2d::CCScene* scene(GJSearchObject*);
    bool init(GJSearchObject*);
};


#endif