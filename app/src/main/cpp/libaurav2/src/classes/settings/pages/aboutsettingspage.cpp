#include "classes/settings/pages/aboutsettingspage.hpp"

void AboutSettingsPage::createPage() {
    float width = this->_dimensions.width;
    float height = this->_dimensions.height;

    auto internal_layer = cocos2d::CCLayer::create();
    this->addChild(internal_layer);

    auto title_string = cocos2d::CCString::createWithFormat("1.9 GDPS u%s", CMakeConfiguration::VERSION);

    auto title = cocos2d::CCLabelBMFont::create(title_string->getCString(), "bigFont.fnt");
    internal_layer->addChild(title);
    title->setPosition(width / 2, height - 10.0f);
    title->setScale(0.75f);

    auto version_string = cocos2d::CCString::createWithFormat(
            "%s-%s\nL%i C%i",
            CMakeConfiguration::BRANCH, CMakeConfiguration::HASH,
            Config::USE_LOGGING, Config::ENABLE_CRASH_DUMPS);

    auto label = cocos2d::CCLabelBMFont::create(version_string->getCString(),
                                                "chatFont.fnt");
    internal_layer->addChild(label);

    label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentLeft);
    label->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
    label->setScale(0.5f);
    label->setPosition(cocos2d::CCPoint(0.0f, 0.0f));

    auto penguin = cocos2d::CCSprite::create("penguin.png");
    internal_layer->addChild(penguin);

    penguin->setPosition(cocos2d::CCPoint(width / 2, height / 2));
    penguin->setScale(0.9f);

    auto c_top = cocos2d::CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
    auto c_left = cocos2d::CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    auto c_bottom = cocos2d::CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
    c_bottom->setFlipY(true);

    auto c_right = cocos2d::CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    c_right->setFlipX(true);

    internal_layer->addChild(c_top, 2);
    internal_layer->addChild(c_bottom, 2);
    internal_layer->addChild(c_left, 1);
    internal_layer->addChild(c_right, 1);

    c_top->setPositionX(width / 2);
    c_top->setPositionY(height - 45.0f);
    c_top->setScaleX(0.70f);

    c_bottom->setPositionX(width / 2);
    c_bottom->setPositionY(45.0f);
    c_bottom->setScaleX(0.70f);

    c_left->setPositionX(0.0f);
    c_left->setPositionY(height / 2);
    c_left->setScaleY(4.0);

    c_right->setPositionX(width);
    c_right->setPositionY(height / 2);
    c_right->setScaleY(4.0);

    auto legal_sprite = ButtonSprite::create(
            "Legal", 220, 0, 0.4f, false, "bigFont.fnt", "GJ_button_04.png", 25.0f);

    auto legal_button = CCMenuItemSpriteExtra::create(
            legal_sprite, nullptr, this,
            static_cast<cocos2d::SEL_MenuHandler>(&AboutSettingsPage::onLicense));

    this->_internal_menu->addChild(legal_button);
    this->_menu_objects.push_back(legal_button);

    legal_button->setPosition((-_window_dimensions.width / 2) + width - 20.0f, -(_window_dimensions.height / 2) + 35.0f);

    auto source_sprite = ButtonSprite::create(
            "Source", 220, 0, 0.4f, false, "bigFont.fnt", "GJ_button_04.png", 25.0f);

    auto source_button = CCMenuItemSpriteExtra::create(
            source_sprite, nullptr, this,
            static_cast<cocos2d::SEL_MenuHandler>(&AboutSettingsPage::onSource));

    this->_internal_menu->addChild(source_button);
    this->_menu_objects.push_back(source_button);

    source_button->setPosition((-_window_dimensions.width / 2) + width - 85.0f, -(_window_dimensions.height / 2) + 35.0f);
}

void AboutSettingsPage::onLicense(cocos2d::CCObject* /* target */)
{
    // due to character restrictions, we might have to change this at some point
    // limit is 1000 characters, as defined in
    // MultilineBitmapFont::stringWithMaxWidth you should be able to change it
    // by modifing the instruction at 0x19F19E
    auto license_dialog = FLAlertLayer::create(nullptr, "Legal",
                                               R"###(<cy>cocos2d-x (v2.2)</c>
Copyright (c) 2010-2011 - cocos2d-x community.
(see each file to see the different copyright owners)

This work is licensed under the terms of the MIT license.
For a copy, see <https://opensource.org/licenses/MIT>.

<cy>spdlog</c>
Copyright (c) 2016 Gabi Melman.

This work is licensed under the terms of the MIT license.
For a copy, see <https://opensource.org/licenses/MIT>.

<cy>Geometry Dash (v1.9)</c>
Copyright (c) 2013-2014 RobtopGames

This modified application is provided under the assumption and restriction that the user owns a license to the original product.)###",
    "OK", nullptr, 400.0f, true, 300.0f);

    license_dialog->show();
}

void AboutSettingsPage::onSource(cocos2d::CCObject*)
{
    cocos2d::CCApplication::sharedApplication()->openURL(Config::SOURCE_URL);
}
