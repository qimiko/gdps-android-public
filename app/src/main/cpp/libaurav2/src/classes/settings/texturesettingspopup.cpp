#include "classes/settings/texturesettingspopup.hpp"

bool TextureSettingsPopup::init() {
    if (this->initWithColor({ 0, 0, 0, 75 })) {
        constexpr auto width = 360.0f;
        constexpr auto height = 195.0f;

        auto ccd = cocos2d::CCDirector::sharedDirector();
        ccd->getTouchDispatcher()->incrementForcePrio();
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        this->controllersConnected_ = -1;
        this->scrollAction_ = -1;

        this->internalLayer_ = cocos2d::CCLayer::create();
        this->addChild(this->internalLayer_);

        auto window_size = ccd->getWinSize();
        auto bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        bg->setContentSize(cocos2d::CCSize(width, height));
        bg->setPosition(window_size.width / 2, window_size.height / 2);

        this->internalLayer_->addChild(bg);

        this->internalMenu_ = cocos2d::CCMenu::create();
        this->internalLayer_->addChild(this->internalMenu_);

        auto close_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto close_btn = CCMenuItemSpriteExtra::create(
                close_sprite,
                nullptr,
                this,
                static_cast<cocos2d::SEL_MenuHandler>(&TextureSettingsPopup::onClose));

        this->internalMenu_->addChild(close_btn);
        close_btn->setPosition(-((width / 2) - 5), (height / 2) - 5);

        auto apply_sprite = ButtonSprite::create("Apply Pack", 220, 0, 1.0f, false);
        auto apply_btn = CCMenuItemSpriteExtra::create(
                apply_sprite,
                nullptr,
                this,
                static_cast<cocos2d::SEL_MenuHandler>(&TextureSettingsPopup::onTextures));

        this->internalMenu_->addChild(apply_btn);
        apply_btn->setPosition(0.0f, 30.0f);

        auto clear_sprite = ButtonSprite::create("Clear Pack", 220, 0, 1.0f, false);
        auto clear_btn = CCMenuItemSpriteExtra::create(
                clear_sprite,
                nullptr,
                this,
                static_cast<cocos2d::SEL_MenuHandler>(&TextureSettingsPopup::onClear));

        this->internalMenu_->addChild(clear_btn);
        clear_btn->setPosition(0.0f, -10.0f);

        auto classic_sprite = ButtonSprite::create("Use Classic Pack", 220, 0, 1.0f, false);
        auto classic_btn = CCMenuItemSpriteExtra::create(
                classic_sprite,
                nullptr,
                this,
                static_cast<cocos2d::SEL_MenuHandler>(&TextureSettingsPopup::onClassic));

        this->internalMenu_->addChild(classic_btn);
        classic_btn->setPosition(0.0f, -60.0f);

        auto title = cocos2d::CCLabelBMFont::create("Texture Settings", "bigFont.fnt");
        this->internalLayer_->addChild(title);

        title->setPosition(window_size.width / 2, (window_size.height / 2) + (height / 2) - 25.0f);
        title->setScale(0.75f);

        return true;
    }

    return false;
}

void TextureSettingsPopup::keyBackClicked()
{
    this->onClose(nullptr);
};

void TextureSettingsPopup::onClose(cocos2d::CCObject*)
{
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void TextureSettingsPopup::onTextures(cocos2d::CCObject*)
{
    JNI::open_texture_picker();
}

void TextureSettingsPopup::onClear(cocos2d::CCObject*)
{
    if (isTexturePackInstalled()) {
        JNI::wipe_textures_directory();
    } else {
        FLAlertLayer::create(nullptr, "Info", "No changes were made to ingame resources.", "OK", nullptr)->show();
    }
}

void TextureSettingsPopup::onClassic(cocos2d::CCObject*) {
    JNI::apply_classic_pack();
}

bool TextureSettingsPopup::isTexturePackInstalled() {
    auto files_exist = false;

    auto directory = opendir(JNI::get_textures_directory().c_str());
    if (directory) {
        while (auto ep = readdir(directory)) {
            if (ep->d_type == DT_REG)
            {
                files_exist = true;
            }
        }
        closedir(directory);
    }

    return files_exist;
}
