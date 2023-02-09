#include "classes/settings/pages/tweakssettingspage.hpp"

void TweaksSettingsPage::createPage()  {
    float width = this->_dimensions.width;
    float height = this->_dimensions.height;

    auto title = cocos2d::CCLabelBMFont::create("Tweaks", "bigFont.fnt");
    this->addChild(title);

    title->setPosition(width / 2, height - 10.0f);
    title->setScale(0.75f);

    auto gm = GameManager::sharedState();

    this->addToggle(
            "Smooth Fix",
            gm->getGameVariable(GameVariable::SMOOTH_FIX),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<23>),
            "Makes some optimizations that can reduce lag. Disable if game speed becomes inconsistent.");

    this->addToggle(
            "Disable explosion shake",
            gm->getGameVariable(GameVariable::DISABLE_EXPLOSION_SHAKE),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<14>));

    this->addToggle(
            "Unlock All",
            gm->getGameVariable(GameVariable::UNLOCK_ALL),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4097>));

    this->addToggle(
            "Create Session",
            gm->getGameVariable(GameVariable::AUTOMATIC_SESSION),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4090>));

    this->addToggle(
            "Keep Awake",
            gm->getGameVariable(GameVariable::KEEP_AWAKE),
            static_cast<cocos2d::SEL_MenuHandler>(&TweaksSettingsPage::onToggleKeepAwake),
            "Keeps screen awake during song playback and gameplay. May reduce battery life.");

    this->addToggle(
            "Free Copy",
            gm->getGameVariable(GameVariable::COPY_HACK),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4087>),
            nullptr);

}

void TweaksSettingsPage::onToggleKeepAwake(cocos2d::CCObject* target) {
    ToggleSettingsPage::onToggleVariable<4088>(target);

    if (!GameManager::sharedState()->getGameVariable(GameVariable::KEEP_AWAKE)) {
        // disable screen awake when variable turned off
        JNI::remove_screen_awake();
    }
}