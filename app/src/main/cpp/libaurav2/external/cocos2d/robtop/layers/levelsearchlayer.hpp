#pragma once
#ifndef _LAYERS_LEVELSEARCHLAYER_HPP
#define _LAYERS_LEVELSEARCHLAYER_HPP

#include <cocos2d.h>

#include <robtop/enums/searchtype.hpp>
#include <robtop/objects/gjsearchobject.hpp>

class LevelSearchLayer : public cocos2d::CCLayer {
public:
    void clearFilters();
    GJSearchObject* getSearchObject(SearchType type, std::string query);
};

#endif
