#include "modules/settings.hpp"

namespace {

void OptionsLayer_onOptions(OptionsLayer* /* self */, cocos2d::CCObject* /* target */)
{
    SettingsPopup::create()->show();
}

class LevelInfoLayerExt : public cocos2d::CCLayer {
public:
    void onOptions(cocos2d::CCObject * /* target */)
    {
        SettingsPopup::create()->show();
    }
};

void add_options_btn(cocos2d::CCLayer* self)
{
    auto options_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    options_sprite->setScale(0.75f);

    auto options_btn = CCMenuItemSpriteExtra::create(
        options_sprite, nullptr, self,
        static_cast<cocos2d::SEL_MenuHandler>(&LevelInfoLayerExt::onOptions));

    auto menu = cocos2d::CCMenu::createWithItem(options_btn);
    self->addChild(menu);

    auto director = cocos2d::CCDirector::sharedDirector();

    auto pos_x = director->getScreenLeft() + 70.0f;
    auto pos_y = director->getScreenTop() - 23.0f;

    menu->setPosition(pos_x, pos_y);

    options_btn->setSizeMult(1.1f);
}

bool LevelInfoLayer_init(LevelInfoLayer* self, GJGameLevel* level)
{
    if (HookHandler::orig<&LevelInfoLayer_init>(self, level)) {
        add_options_btn(self);

        auto gm = GameManager::sharedState();
        bool secret_enabled = gm->getGameVariable(GameVariable::REPLAY_CONTROLS);

        if (secret_enabled) {
            auto replay_sprite = cocos2d::CCSprite::createWithSpriteFrameName(
                "GJ_playEditorBtn_001.png");
            replay_sprite->setScale(0.75f);

            auto replay_btn = CCMenuItemSpriteExtra::create(
                replay_sprite, nullptr, self,
                static_cast<cocos2d::SEL_MenuHandler>(&LevelInfoLayer::onPlayReplay));

            auto menu = cocos2d::CCMenu::createWithItem(replay_btn);
            self->addChild(menu);

            auto director = cocos2d::CCDirector::sharedDirector();

            auto pos_x = director->getScreenLeft() + 110.0f;
            auto pos_y = director->getScreenTop() - 23.0f;

            menu->setPosition(pos_x, pos_y);

            replay_btn->setSizeMult(1.1f);
        }
        return true;
    }

    return false;
}

bool EditLevelLayer_init(EditLevelLayer* self, GJGameLevel* level)
{
    auto result = HookHandler::orig<&EditLevelLayer_init>(self, level);

    add_options_btn(self);

    return result;
}

void MenuLayer_openOptions(MenuLayer* self, bool openVideoSettings) {
    if (openVideoSettings) {
        auto ol = OptionsLayer::create();
        self->addChild(ol, 99);
        ol->showLayer(true);

        auto sp = SettingsPopup::create(5);
        self->addChild(sp, 100);
        return;
    } else {
        HookHandler::orig<&MenuLayer_openOptions>(self, openVideoSettings);
    }
}

bool CCFileUtilsAndroid_init(cocos2d::CCFileUtilsAndroid* self) {
    self->addSearchPath(JNI::get_textures_directory().c_str());

    // secondary assets should override base assets
    if (JNI::is_launcher_build()) {
        auto dir = JNI::get_secondary_assets_directory();
        spdlog::info("loading secondary assets from {}", dir);
        self->addSearchPath(dir.c_str());
    }

    return HookHandler::orig<&CCFileUtilsAndroid_init>(self);
}

bool GameManager_getGameVariable(GameManager* self, const char* variable) {
    auto tag = std::string("gv_").append(variable);
    return self->getValueKeeper()->valueForKey(tag)->boolValue();
}

int GameManager_getIntGameVariable(GameManager* self, const char* variable) {
    auto tag = std::string("gv_").append(variable);
    return self->getValueKeeper()->valueForKey(tag)->intValue();
}

}

namespace Settings
{
void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook("_ZN12OptionsLayer9onOptionsEPN7cocos2d8CCObjectE", OptionsLayer_onOptions)
        .add_hook("_ZN14LevelInfoLayer4initEP11GJGameLevel", LevelInfoLayer_init)
        .add_hook("_ZN14EditLevelLayer4initEP11GJGameLevel", EditLevelLayer_init)
        .add_hook("_ZN7cocos2d18CCFileUtilsAndroid4initEv", CCFileUtilsAndroid_init)
        .add_hook(&GameManager::getGameVariable, &GameManager_getGameVariable)
        .add_hook(&GameManager::getIntGameVariable, &GameManager_getIntGameVariable)
        .add_hook(&MenuLayer::openOptions, &MenuLayer_openOptions);
}

} // namespace Search
