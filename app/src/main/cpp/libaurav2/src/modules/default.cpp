#include "modules/default.hpp"

namespace {
bool IS_GD_INSTALLED = false;

class MenuLayer_H : public MenuLayer {
private:
    void googlePlaySignedIn() override {};
    void FLAlert_Clicked(FLAlertLayer*, bool) override {};

public:
    void onCrash(cocos2d::CCObject* btn)
    {
        auto last_file = CrashHelperPopup::getLastCrashName();
        if (last_file.empty()) {
            reinterpret_cast<cocos2d::CCNode*>(btn)->setVisible(false);
            return;
        }

        auto popup = CrashHelperPopup::create();

        auto current_scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();

        popup->setTargetScene(current_scene);
        popup->setCrashBtn(reinterpret_cast<cocos2d::CCNode*>(btn));
        popup->show();
    }

    void onCreator(cocos2d::CCObject*)
    {
        spdlog::get("global")->info("MenuLayer::onCreator running");

        auto gm = GameManager::sharedState();
        /*
// messy logic, "caches" check
*/
        if (!IS_GD_INSTALLED && !gm->getGameVariable(GameVariable::BYPASS_PACKAGE_CHECK)) {
            gm->setShowedPirate(true);

            auto protocol_handler = ptr_to_offset<FLAlertLayerProtocol>(this, 0x118);

            auto err_string = "<cy>Online features</c> have been <cr>disabled</c>.\n\
To enable them, you must <cl>purchase and install</c> the official full release of <cg>Geometry Dash</c> onto your device. \
This allows the GDPS to stay up and also supports the development of the main game.\n\
<co>Thanks</c>!";

            auto msg_box = FLAlertLayer::create(
                protocol_handler, "Geometry Dash Required!", err_string,
                "OK :(", "Get it", 400.0);

            msg_box->setTag(2);

            auto app_delegate = AppDelegate::get();
            auto scene_pointer = app_delegate->getScenePointer();

            msg_box->setTargetScene(scene_pointer);
            msg_box->show();
            return;
        }

        if (!gm->getClickedEditor()) {
            gm->setClickedEditor(true);
        }

        // i'm too lazy to figure out how to correctly trampoline, gg
        this->willClose();

        auto director = cocos2d::CCDirector::sharedDirector();

        auto clScene = CreatorLayer::scene();
        auto fadeScene = cocos2d::CCTransitionFade::create(0.5, clScene);

        director->replaceScene(fadeScene);
    }
};

bool MenuLayer_init(MenuLayer* self)
{
    spdlog::get("global")->info("MenuLayer::init called");

    if (HookHandler::orig<&MenuLayer_init>(self)) {
        auto gm = GameManager::sharedState();

        if (!IS_GD_INSTALLED) {
            if (!JNI::is_gd_installed() && !gm->getGameVariable(GameVariable::BYPASS_PACKAGE_CHECK)) {
                auto menu_buttons = reinterpret_cast<cocos2d::CCMenu*>(
                    self->getChildren()->objectAtIndex(2));

                auto create_button = reinterpret_cast<CCMenuItemSpriteExtra*>(
                    menu_buttons->getChildren()->lastObject());

                create_button->setColor({ 0xA6, 0xA6, 0xA6 });
            } else {
                IS_GD_INSTALLED = true;
            }
        }

        if (!CrashHelperPopup::getLastCrashName().empty()) {
            auto crash_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_reportBtn_001.png");
            crash_sprite->setScale(0.75f);

            auto crash_btn = CCMenuItemSpriteExtra::create(
                crash_sprite, nullptr, self,
                static_cast<cocos2d::SEL_MenuHandler>(&MenuLayer_H::onCrash));

            auto menu = cocos2d::CCMenu::createWithItem(crash_btn);
            self->addChild(menu);

            auto director = cocos2d::CCDirector::sharedDirector();

            auto pos_x = director->getScreenLeft() + 23.0f;
            auto pos_y = director->getScreenTop() - 23.0f;

            menu->setPosition(pos_x, pos_y);

            crash_btn->setSizeMult(1.1f);
        }

        JNI::loaded_to_menu();
        return true;
    }

    return false;
}

const char*
GetLoadingString(LoadingLayer* /* self */)
{
    auto t = time(nullptr);
    auto tmp = localtime(&t);

    if (tmp == nullptr || tmp->tm_wday == 1) {
        // thank you stev
        return "Save your game often!";
    } else {
        std::array<const char*, 7> messages {
            { "Did you make a session?", "Be sure to support the original game!",
                "Tip: Blending colors goes beneath other colors.",
                "Sponsored by beans.com", "Its not over till its over...",
                "Check the options menu for special settings!",
                "Tip: Ensure your 3D lines are consistent" }
        };

        auto chosen_index = random() % messages.size();
        return messages.at(chosen_index);
    }
}

void LevelPage_onInfo(LevelPage* self, cocos2d::CCObject* target)
{
    auto gamelevel = get_from_offset<GJGameLevel*>(self, 0x11C);
    if (gamelevel != nullptr) {
        // game uses page -1 for last page
        if (gamelevel->getLevelID() == -1) {
            std::array<const char*, 4> messages {
                { "1.9 GDPS says:\n\
<cl>====================</c>\n\
<cp>====================</c>\n\
====================\n\
<cp>====================</c>\n\
<cl>====================</c>",
                    "<cr>Watch your step</c>, the <co>lasaga</c> approaches...",
                    "This <cp>secret message</c> was sponsored by <cy>beans.com</c>.",
                    "Special thanks to <cl>jamie;</c> for being <cg>the</c>." }
            };

            auto chosen_index = random() % messages.size();

            spdlog::get("global")->info("{} ~ chloe <3", messages.at(chosen_index));

            auto alert = FLAlertLayer::create(nullptr, "You found me..!",
                messages.at(chosen_index), "<3", nullptr, 350.0);
            alert->show();

            return;
        }
    }

    HookHandler::orig<&LevelPage_onInfo>(self, target);
}

class SupportLayerBtnHelper : public cocos2d::CCNode {
public:
    void onBypassBtn(cocos2d::CCObject* /* target */)
    {
        auto gm = GameManager::sharedState();
        gm->toggleGameVariable(GameVariable::BYPASS_PACKAGE_CHECK);
    }

    void onResetAuthKey(cocos2d::CCObject* /* target */)
    {
        if (GameToolbox::doWeHaveInternet()) {
            auto trh = TokenResponseHandler::create();
            trh->deleteAuthKey();
        }
    }
};

void SupportLayer_customSetup(SupportLayer* self)
{
    spdlog::get("global")->info("SupportLayer::customSetup called");

    HookHandler::orig<&SupportLayer_customSetup>(self);

    auto internal_layer = self->getInternalLayer();

    auto director = cocos2d::CCDirector::sharedDirector();
    auto win_size = director->getWinSize();

    auto gm = GameManager::sharedState();
    auto current_status = gm->getGameVariable(GameVariable::BYPASS_PACKAGE_CHECK);

    auto point = cocos2d::CCPoint(
        director->getScreenLeft() + (win_size.width / 2.0f) - 150.0f,
        (2 * director->getScreenTop()) - 77.0f);

    auto legal_menu = reinterpret_cast<cocos2d::CCMenu*>(
        internal_layer->getChildren()->objectAtIndex(5));

    // bypass button
    self->createToggleButton("",
        static_cast<cocos2d::SEL_MenuHandler>(
            &SupportLayerBtnHelper::onBypassBtn),
        !current_status, legal_menu, point);

    auto reset_authkey_sprite = ButtonSprite::create(
        "Reset", 220, 0, 0.4f, false, "bigFont.fnt", "GJ_button_03.png", 25.0f);

    auto reset_authkey_button = CCMenuItemSpriteExtra::create(
        reset_authkey_sprite, nullptr, self,
        static_cast<cocos2d::SEL_MenuHandler>(
            &SupportLayerBtnHelper::onResetAuthKey));

    legal_menu->addChild(reset_authkey_button);

    auto reset_authkey_position = cocos2d::CCPoint(142.0f, -120.0f);
    reset_authkey_button->setPosition(reset_authkey_position);

    return;
}

} // namespace

namespace Default {

void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook(&LoadingLayer::getLoadingString, GetLoadingString)
        .add_hook(&MenuLayer::onCreator, &MenuLayer_H::onCreator)
        .add_hook("_ZN9MenuLayer4initEv", &MenuLayer_init)
        .add_hook(&SupportLayer::customSetup, SupportLayer_customSetup)
        .add_hook(&LevelPage::onInfo, LevelPage_onInfo)
        .install_patch(0x1A879A, { 0x1B, 0x46 }) // patches early text field truncation
        .install_patch(0x1AE47E, { 0x21, 0xE0, 0x00, 0x00 }) // skips adding google play button to menu
        .install_patch(0x1AC13C, { 0x00 }) // sets default pixel format to RGBA8888
        .install_patch(0x1AC142, { 0x00 }); // turns off premultiplied alpha
}
} // namespace Default