#pragma once

#ifndef _LAYERS_EDITOR_CUSTOMSONGLAYER_HPP
#define _LAYERS_EDITOR_CUSTOMSONGLAYER_HPP

#include <robtop/protocols/flalertlayerprotocol.hpp>
#include <robtop/protocols/textinputdelegate.hpp>

#include <robtop/objects/levelsettingsobject.hpp>

#include <robtop/layers/flalertlayer.hpp>

class CustomSongLayer : public FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate {
public:
    static CustomSongLayer* create(LevelSettingsObject*);
    bool init(LevelSettingsObject*);

    void onNewgrounds(cocos2d::CCObject*);
};

#endif //_LAYERS_EDITOR_CUSTOMSONGLAYER_HPP
