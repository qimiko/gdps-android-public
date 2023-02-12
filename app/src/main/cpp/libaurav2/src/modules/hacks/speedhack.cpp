#include "modules/hacks/speedhack.hpp"

namespace {
    void FMODAudioEngine_update(FMODAudioEngine* self, float dt) {
        HookHandler::orig<&FMODAudioEngine_update>(self, dt);
        auto control = get_from_offset<FMOD::ChannelControl *>(self, 0x11C);

        if (SpeedhackManagerCard::isSpeedhackActive()) {
            auto interval = SpeedhackManagerCard::getSpeedhackInterval();
            control->setPitch(interval);

            return;
        }

        // will need to determine if this is necessary
        control->setPitch(1.0f);
    }

    void PauseLayer_customSetup(PauseLayer *self) {
        bool speedhack_controls_enabled = GameManager::sharedState()->getGameVariable(
                GameVariable::SPEED_CONTROLS);
        if (speedhack_controls_enabled) {
            auto director = cocos2d::CCDirector::sharedDirector();
            auto screen_right = director->getScreenRight();
            auto screen_top = director->getScreenTop();

            auto card = SpeedhackManagerCard::create();
            self->addChild(card);
            card->setPosition({screen_right - 70.0f, screen_top - 70.0f});

            if (auto pl = GameManager::sharedState()->getPlayLayer(); pl != nullptr) {
                if (auto speedhack_pl_label = dynamic_cast<cocos2d::CCLabelBMFont *>(pl->getChildByTag(
                            661)); speedhack_pl_label != nullptr) {
                    card->setExternalLabel(speedhack_pl_label);
                }
            }
        }

        HookHandler::orig<&PauseLayer_customSetup>(self);
    }

    bool PlayLayer_init(PlayLayer *self, GJGameLevel *level) {
        spdlog::info("PlayLayer::init{speedhack} called");

        auto return_value = HookHandler::orig<&PlayLayer_init>(self, level);

        auto gm = GameManager::sharedState();
        auto director = cocos2d::CCDirector::sharedDirector();

        bool speedhack_controls_enabled = gm->getGameVariable(GameVariable::SPEED_CONTROLS);
        if (speedhack_controls_enabled) {
            auto time_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
            self->addChild(time_sprite, 21);

            auto screen_side = gm->getGameVariable(GameVariable::FLIP_PAUSE) ? director->getScreenRight() : director->getScreenLeft();
            auto screen_positioning_scale = gm->getGameVariable(GameVariable::FLIP_PAUSE) ? -1 : 1;
            auto screen_top = director->getScreenTop();

            time_sprite->setPositionX(screen_side + screen_positioning_scale * 15.0f);
            time_sprite->setPositionY(screen_top - 15.0f);

            time_sprite->setOpacity(127);

            auto speedhack_interval = SpeedhackManagerCard::getSpeedhackInterval();
            auto speedhack_string = cocos2d::CCString::createWithFormat("%.2fx", speedhack_interval);
            auto speedhack_label = cocos2d::CCLabelBMFont::create(
                    speedhack_string->getCString(), "bigFont.fnt");

            self->addChild(speedhack_label, 21, 661);
            speedhack_label->setPosition(screen_side + screen_positioning_scale * 55.0f, screen_top - 15.0f);
            speedhack_label->setScale(0.5f);
            speedhack_label->setOpacity(127);
            if (gm->getGameVariable(GameVariable::FLIP_PAUSE)) {
                speedhack_label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentRight);
            }

            SpeedhackManagerCard::setSpeedhackValue(speedhack_interval);

            auto lifecycle = SpeedhackLifecycleWatcher::create();
            self->addChild(lifecycle);

            lifecycle->beginSpeedhack();

            spdlog::get("global")->info("speedhack controls enabled with interval {}", speedhack_interval);
        }

        return return_value;
    }

    void PlayLayer_resetLevel(PlayLayer* self) {
        HookHandler::orig<&PlayLayer_resetLevel>(self);

        // skip next check if noclip enabled
        bool noclip_enabled = GameManager::sharedState()->getGameVariable(GameVariable::IGNORE_DAMAGE);
        if (noclip_enabled) {
            return;
        }

        // check if user is still using speed hack
        if (!SpeedhackManagerCard::isSpeedhackActive()) {
            if (auto pl_ext = dynamic_cast<PlayLayerExt *>(self->getUserObject()); pl_ext != nullptr) {
                spdlog::info("clearing legitimacy check for speedhack");
                pl_ext->setIsIllegitimate(false);
            }
        }
    }

    void EditorPauseLayer_customSetup(EditorPauseLayer* self) {
        bool speedhack_controls_enabled = GameManager::sharedState()->getGameVariable(
                GameVariable::SPEED_CONTROLS);

        if (speedhack_controls_enabled) {
            auto director = cocos2d::CCDirector::sharedDirector();
            auto screen_right = director->getScreenRight();
            auto screen_top = director->getScreenTop();

            auto card = SpeedhackManagerCard::create();
            self->addChild(card);
            card->setPosition({screen_right - 50.0f, screen_top - 50.0f});
        }

        HookHandler::orig<&EditorPauseLayer_customSetup>(self);
    }

    bool LevelEditorLayer_init(LevelEditorLayer* self, GJGameLevel* lvl) {
        if (HookHandler::orig<&LevelEditorLayer_init>(self, lvl)) {
            auto speedhack_interval = SpeedhackManagerCard::getSpeedhackInterval();
            SpeedhackManagerCard::setSpeedhackValue(speedhack_interval);

            auto lifecycle = SpeedhackLifecycleWatcher::create();
            self->addChild(lifecycle);

            lifecycle->beginSpeedhack();

            return true;
        }

        return false;
    }
}

namespace Hacks::Speedhack {
    void Module::on_initialize() {
        HookHandler::get_handler()
                .add_hook(&PauseLayer::customSetup, PauseLayer_customSetup)
                .add_hook("_ZN9PlayLayer10resetLevelEv", PlayLayer_resetLevel)
                .add_hook("_ZN15FMODAudioEngine6updateEf", &FMODAudioEngine_update)
                .add_hook("_ZN16EditorPauseLayer11customSetupEv", &EditorPauseLayer_customSetup)
                .add_hook("_ZN16LevelEditorLayer4initEP11GJGameLevel", &LevelEditorLayer_init)
                .add_hook("_ZN9PlayLayer4initEP11GJGameLevel", PlayLayer_init);
    }
}