#pragma once
#ifndef _LAYERS_GJSONGBROWSER_HPP
#define _LAYERS_GJSONGBROWSER_HPP

#include "gjdropdownlayer.hpp"
#include <robtop/objects/levelsettingsobject.hpp>

class GJSongBrowser : public GJDropDownLayer {
public:
    static GJSongBrowser* create(LevelSettingsObject*);

    void loadPage(int page);
    void customSetup();
};

#endif
