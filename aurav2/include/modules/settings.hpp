#pragma once
#ifndef MODULES_SETTINGS_HPP
#define MODULES_SETTINGS_HPP

#include <cocos2dcpp.h>

#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "base/hooks.hpp"
#include "base/module.hpp"

namespace Settings {

class SettingsPopup : public FLAlertLayer {
protected:
    bool init(int page);

    virtual void keyBackClicked();

    void onClose(cocos2d::CCObject*);

    void onToggleLayer(cocos2d::CCObject* target);

    void toggleLayer(uint32_t layer);

    cocos2d::CCArray* buttons_;
    cocos2d::CCArray* layers_;

public:
    static SettingsPopup* create(int page = -1) {
        auto pRet = new SettingsPopup();
        if (pRet && pRet->init(page)) {
            pRet->autorelease();
            return pRet;
        } else {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
};

class Module : public Base::Module {
protected:
    void on_initialize();
};
}

#endif