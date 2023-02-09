#pragma once

#ifndef CLASSES_SETTINGS_TWEAKSSETTINGSPAGE_HPP
#define CLASSES_SETTINGS_TWEAKSSETTINGSPAGE_HPP

#include <cocos2dcpp.h>

#include "base/game_variables.hpp"
#include "base/jni_funcs.hpp"

#include "classes/settings/togglesettingspage.hpp"

class TweaksSettingsPage : public ToggleSettingsPage {
public:
    void createPage() override;

    CREATE_FUNC(TweaksSettingsPage); // NOLINT(modernize-use-auto)

private:
    void onToggleKeepAwake(cocos2d::CCObject* target);
};

#endif //CLASSES_SETTINGS_TWEAKSSETTINGSPAGE_HPP
