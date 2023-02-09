#pragma once

#ifndef CLASSES_SETTINGS_SONGSSETTINGSPAGE_HPP
#define CLASSES_SETTINGS_SONGSSETTINGSPAGE_HPP

#include <cocos2dcpp.h>
#include <fmt/format.h>

#include "base/game_variables.hpp"

#include "classes/settings/togglesettingspage.hpp"

class SongsSettingsPage : public ToggleSettingsPage, TextInputDelegate {
private:
    CCTextInputNode* offset_input_{nullptr};
public:
    void createPage() override;
    void onClose() override;
    void onToggleVisibility(bool visible) override;

    void onSongBrowser(cocos2d::CCObject* /* target */);

    CREATE_FUNC(SongsSettingsPage); // NOLINT(modernize-use-auto)
};

#endif //CLASSES_SETTINGS_SONGSSETTINGSPAGE_HPP
