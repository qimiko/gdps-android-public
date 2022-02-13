#pragma once
#ifndef _LAYERS_EDITOR_EDITORUI_HPP
#define _LAYERS_EDITOR_EDITORUI_HPP

#include <cocos2d.h>

#include <robtop/layers/editor/leveleditorlayer.hpp>

class EditorUI : public cocos2d::CCLayer {
public:
    void setupCreateMenu();
    CreateMenuItem* getCreateBtn(int objectID, int categoryID, bool highlighted);
    CreateMenuItem* getCreateBtn(int objectID, int categoryID);

    void onCopy(cocos2d::CCObject*);
    std::string copyObjects(cocos2d::CCArray*);

    void updateButtons();

    int init(LevelEditorLayer*);
};

#endif
