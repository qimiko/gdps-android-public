#pragma once
#ifndef _LAYERS_MENULAYER_HPP
#define _LAYERS_MENULAYER_HPP

#include <cocos2d.h>
#include <robtop/protocols/googleplaydelegate.hpp>

class MenuLayer : public cocos2d::CCLayer, public GooglePlayDelegate, public FLAlertLayerProtocol {
public:
    static cocos2d::CCScene* scene(bool);

    void googlePlaySignedIn();
    void FLAlert_Clicked(FLAlertLayer*, bool);

    void onPlay(cocos2d::CCObject*);
    void onCreator(cocos2d::CCObject*);
    void onGarage(cocos2d::CCObject*);

    void showPirateMessage();

    void willClose();

    void onRobTop(cocos2d::CCObject*);
    void onTwitter(cocos2d::CCObject*);
    void onYouTube(cocos2d::CCObject*);
    void onFacebook(cocos2d::CCObject*);

    void openOptions(bool isVideo);

    bool init();
};

#endif
