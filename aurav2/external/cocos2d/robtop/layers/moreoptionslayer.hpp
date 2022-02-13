#pragma once
#ifndef _LAYERS_MOREOPTIONSLAYER_HPP
#define _LAYERS_MOREOPTIONSLAYER_HPP

#include "flalertlayer.hpp"
#include <cocos2d.h>

class MoreOptionsLayer : public FLAlertLayer {
public:
    static MoreOptionsLayer* create();

    bool init();
    void addToggle(const char* name, const char* save_key,
        const char* description);
    void onToggle(cocos2d::CCObject* target);

    // CCArray of CCStrings
    cocos2d::CCArray* objectsForPage(int);
    static cocos2d::CCString* pageKey(int);
    static cocos2d::CCString* objectKey(int);
    static cocos2d::CCString* layerKey(int);
};

#endif
