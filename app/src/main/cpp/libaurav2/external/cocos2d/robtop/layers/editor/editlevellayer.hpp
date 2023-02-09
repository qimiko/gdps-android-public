#pragma once
#ifndef _LAYERS_EDITOR_EDITLEVELLAYER_HPP
#define _LAYERS_EDITOR_EDITLEVELLAYER_HPP

#include <cocos2d.h>

class EditLevelLayer : public cocos2d::CCLayer {
public:
    static cocos2d::CCScene* scene(GJGameLevel*);
    bool init(GJGameLevel*);
    void onShare(cocos2d::CCObject*);
    void onClone();
};

#endif
