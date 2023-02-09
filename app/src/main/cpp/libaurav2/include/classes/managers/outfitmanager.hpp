#pragma once

#ifndef CLASSES_MANAGERS_OUTFITMANAGER_HPP
#define CLASSES_MANAGERS_OUTFITMANAGER_HPP

#include <cocos2dcpp.h>

#include "base/spdlog.hpp"
#include "base/hook_handler.hpp"

class OutfitManager {
public:
    static OutfitManager& get_manager();
    cocos2d::ccColor3B get_color(int idx);

private:
    cocos2d::CCDictionary* _outfits = nullptr;

    void init();
    void install_patches();

    ~OutfitManager();
    OutfitManager();
};

#endif //CLASSES_MANAGERS_OUTFITMANAGER_HPP
