#pragma once
#ifndef _SPRITES_CCSPRITEPLUS_HPP
#define _SPRITES_CCSPRITEPLUS_HPP

#include <cocos2d.h>

class CCSpritePlus : public cocos2d::CCSprite {
private:
    cocos2d::CCArray* followers_;
    CCSpritePlus* following_;
    bool hasFollowers_;

    bool initWithSpriteFrameName(const char*) override;

public:
    static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame*);
    static CCSpritePlus* createWithSpriteFrameName(const char*);

    void addFollower(cocos2d::CCNode*);
    void removeFollower(cocos2d::CCNode*);

    void followSprite(CCSpritePlus*);
    void stopFollow();

    void setPosition(const cocos2d::CCPoint&) override;
    void setRotation(float) override;
    void setScale(float) override;
    void setScaleX(float) override;
    void setScaleY(float) override;

    virtual void setFlipX(bool);
    virtual void setFlipY(bool);
};

#endif
