#pragma once
#ifndef _LAYERS_EDITOR_LEVELEDITORLAYER_HPP
#define _LAYERS_EDITOR_LEVELEDITORLAYER_HPP

#include <cocos2d.h>

#include <robtop/objects/levelsettingsobject.hpp>

class LevelEditorLayer : public cocos2d::CCLayer {
public:
		bool init(GJGameLevel*);
		void updateVisibility(float);

		CCLayer* getGameLayer() const;
		LevelSettingsObject* getLevelSettings() const;
};

#endif
