#pragma once
#ifndef _LAYERS_LOADINGCIRCLE_HPP
#define _LAYERS_LOADINGCIRCLE_HPP

#include <cocos2d.h>

class LoadingCircle : public cocos2d::CCLayerColor {
public:
    static LoadingCircle* create();
		bool init();
    void show();
    void fadeAndRemove();

    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
		virtual void registerWithTouchDispatcher();

    virtual void draw();

    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, circle_, Circle);
    CC_SYNTHESIZE(cocos2d::CCNode*, targetScene_, TargetScene);
    CC_SYNTHESIZE(bool, darkOverlay_, DarkOverlay);
};

#endif
