#pragma once
#ifndef _ITEMS_CUSTOMSONGWIDGET_HPP
#define _ITEMS_CUSTOMSONGWIDGET_HPP

#include <cocos2d.h>

#include <robtop/objects/songinfoobject.hpp>
#include <robtop/protocols/flalertlayerprotocol.hpp>

class CustomSongWidget : public cocos2d::CCNode, public FLAlertLayerProtocol {
public:
    void updateSongInfo();
    void updatePlaybackBtn();

    void songStateChanged();

    SongInfoObject* getSongObject() const;

    void FLAlert_Clicked(FLAlertLayer*, bool);
};

#endif
