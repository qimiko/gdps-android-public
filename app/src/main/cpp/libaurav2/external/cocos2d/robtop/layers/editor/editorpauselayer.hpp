//
// Created by dslkjkld on 1/15/23.
//

#ifndef _LAYERS_EDITOR_EDITORPAUSELAYER_HPP
#define _LAYERS_EDITOR_EDITORPAUSELAYER_HPP

#include <cocos2d.h>

#include <robtop/protocols/flalertlayerprotocol.hpp>
#include <robtop/layers/editor/leveleditorlayer.hpp>

class EditorPauseLayer : public cocos2d::CCLayerColor, public FLAlertLayerProtocol {
private:
    bool init(LevelEditorLayer*);

public:
    EditorPauseLayer* create(LevelEditorLayer*);
    void customSetup();
};

#endif //_LAYERS_EDITOR_EDITORPAUSELAYER_HPP
