#pragma once
#ifndef _LAYERS_FLALERTLAYER_HPP
#define _LAYERS_FLALERTLAYER_HPP

#include <cocos2d.h>

class FLAlertLayer : public cocos2d::CCLayerColor {
protected:
    cocos2d::CCMenu* internalMenu_;

    int controllersConnected_;
    int zOrder_;

    FLAlertLayerProtocol* pParent_;
    cocos2d::CCScene* targetScene_;

    bool reverseKeyBack_;
    cocos2d::CCLayer* internalLayer_;
    void* scrollLayer_;

    int scrollAction_;
    bool useTextArea_;

    bool noBtn2_;

public:
    static FLAlertLayer* create(FLAlertLayerProtocol*, const char* title,
        const char* text, const char* leftBtn,
        const char* rightBtn = nullptr);
    static FLAlertLayer* create(FLAlertLayerProtocol*, const char*,
        const char*, const char*, const char*,
        float width);
    static FLAlertLayer*
    create(FLAlertLayerProtocol*, const char*, const char*, const char*,
        const char*, float, bool isScrolling,
        float height); // scrolling dialog is created under assumption that
        // height >= 300

    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);

    virtual void registerWithTouchDispatcher();

    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes);

    int show();

    FLAlertLayerProtocol* getPParent() const;
    void setPParent(FLAlertLayerProtocol*);

    cocos2d::CCNode* getTargetScene() const;
    void setTargetScene(cocos2d::CCNode*);

    bool getReverseKeyBack() const;
    void setReverseKeyBack(bool);

    cocos2d::CCLayer* getInternalLayer() const;

    cocos2d::CCMenu* _getMenu() const {
        return this->internalMenu_;
    }
};

#endif
