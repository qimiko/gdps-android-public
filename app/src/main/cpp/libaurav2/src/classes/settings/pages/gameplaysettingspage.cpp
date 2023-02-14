#include "classes/settings/pages/gameplaysettingspage.hpp"

void GameplaySettingsPage::createPage() {
    float width = this->_dimensions.width;
    float height = this->_dimensions.height;

    auto title = cocos2d::CCLabelBMFont::create("Gameplay", "bigFont.fnt");
    this->addChild(title);

    title->setPosition(width / 2, height - 10.0f);
    title->setScale(0.75f);

    auto gm = GameManager::sharedState();

    this->addToggle(
            "Auto-Retry",
            gm->getGameVariable(GameVariable::AUTOMATIC_RETRY),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<26>));

    this->addToggle(
            "Auto-Checkpoints",
            gm->getGameVariable(GameVariable::AUTOMATIC_CHECKPOINTS),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<27>),
            "Automatically place checkpoints while in practice mode.");

    this->addToggle(
            "Safe Noclip",
            gm->getGameVariable(GameVariable::IGNORE_DAMAGE),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4099>),
            "Prevents the player from dying. Disables level completions.");

    this->addToggle(
            "Speedhack Controls",
            gm->getGameVariable(GameVariable::SPEED_CONTROLS),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4093>),
            "Enables controlling the speed of levels. Disables level completions if used during an attempt.");

    this->addToggle(
            "Fast Practice Reset",
            gm->getGameVariable(GameVariable::FAST_PRACTICE_RESET),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4095>),
            "Shortens reset time in practice mode from 1 second to 0.5 seconds.");
}
