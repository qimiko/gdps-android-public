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

/*
    // code for position testing

    auto h_top = cocos2d::CCLabelBMFont::create("-T-", "bigFont.fnt");
    internal_layer->addChild(h_top, 3);
    h_top->setPosition(width / 2, height);
    h_top->setOpacity(127);

    auto h_left = cocos2d::CCLabelBMFont::create("-L-", "bigFont.fnt");
    internal_layer->addChild(h_left, 3);
    h_left->setPosition(0.0f, height / 2);
    h_left->setOpacity(127);
    h_left->setRotation(270.0f);

    auto h_bottom = cocos2d::CCLabelBMFont::create("-B-", "bigFont.fnt");
    internal_layer->addChild(h_bottom, 3);
    h_bottom->setPosition(width / 2, 0.0f);
    h_bottom->setOpacity(127);
    h_bottom->setRotation(180.0f);

    auto h_right = cocos2d::CCLabelBMFont::create("-R-", "bigFont.fnt");
    internal_layer->addChild(h_right, 3);
    h_right->setPosition(width, height / 2);
    h_right->setOpacity(127);
    h_right->setRotation(90.0f);
*/

    auto version_string = cocos2d::CCString::createWithFormat(
            "%s-%s\nL%i C%i",
            CMakeConfiguration::BRANCH, CMakeConfiguration::HASH,
            Config::USE_LOGGING, Config::ENABLE_CRASH_DUMPS);

    auto label = cocos2d::CCLabelBMFont::create(version_string->getCString(),
                                                "chatFont.fnt");
    internal_layer->addChild(label);
    label->setColor({  0xBB, 0xBB, 0xBB });

    label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentLeft);
    label->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
    label->setScale(0.5f);
    label->setPosition(cocos2d::CCPoint(0.0f, 0.0f));

    auto penguin = cocos2d::CCSprite::create("penguin.png");

    auto penguin_offset_x = -2.0f;
    auto penguin_offset_y = 10.0f;

    internal_layer->addChild(penguin);

    penguin->setPosition({ (width / 2) + penguin_offset_x, (height / 2) + penguin_offset_y });
    penguin->setScale(1.0f);

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

    c_top->setPositionX((width / 2) + penguin_offset_x);
    c_top->setPositionY((height / 2) + 68.0f + penguin_offset_y);
    c_top->setScaleX(0.78f);

    c_bottom->setPositionX((width / 2) + penguin_offset_x);
    c_bottom->setPositionY((height / 2) - 68.0f + penguin_offset_y);
    c_bottom->setScaleX(0.78f);

    c_left->setPositionX((width / 2) - 136.0f + penguin_offset_x);
    c_left->setPositionY((height / 2) + penguin_offset_y);
    c_left->setScaleY(4.4f);

    c_right->setPositionX((width / 2) + 136.0f + penguin_offset_x);
    c_right->setPositionY((height / 2) + penguin_offset_y);
    c_right->setScaleY(4.4f);

    auto legal_sprite = ButtonSprite::create(
            "Legal", 220, 0, 0.4f, false, "bigFont.fnt", "GJ_button_04.png", 25.0f);

    auto legal_button = CCMenuItemSpriteExtra::create(
            legal_sprite, nullptr, this,
            static_cast<cocos2d::SEL_MenuHandler>(&AboutSettingsPage::onLicense));

    this->_internal_menu->addChild(legal_button);
    this->_menu_objects.push_back(legal_button);

    legal_button->setPosition((-_window_dimensions.width / 2) + width - 20.0f, -(_window_dimensions.height / 2) + 30.0f);

    auto source_sprite = ButtonSprite::create(
            "Source", 220, 0, 0.4f, false, "bigFont.fnt", "GJ_button_04.png", 25.0f);

    auto source_button = CCMenuItemSpriteExtra::create(
            source_sprite, nullptr, this,
            static_cast<cocos2d::SEL_MenuHandler>(&AboutSettingsPage::onSource));

    this->_internal_menu->addChild(source_button);
    this->_menu_objects.push_back(source_button);

    source_button->setPosition((-_window_dimensions.width / 2) + width - 90.0f, -(_window_dimensions.height / 2) + 30.0f);
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

<cy>spdlog</c>
Copyright (c) 2016 Gabi Melman.

These works are licensed under the terms of the MIT license.
For a copy, see <https://opensource.org/licenses/MIT>.

<cy>Geometry Dash (v1.9)</c>
Copyright (c) 2013-2014 RobtopGames

This application is provided under the restriction that the user owns a license to the original product.)###",
    "OK", nullptr, 400.0f, true, 300.0f);

    license_dialog->show();
}

void AboutSettingsPage::onSource(cocos2d::CCObject*)
{
    cocos2d::CCApplication::sharedApplication()->openURL(Config::SOURCE_URL);
}
