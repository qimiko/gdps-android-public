#include "modules/hacks.hpp"

namespace {

struct ConditionalBytepatch {
    int rel_addr;
    std::vector<uint8_t> patch_bytes;
    std::vector<uint8_t> orig_bytes;
};

uint8_t* get_patch_ptr(int rel_addr)
{
    return reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(GLOBAL_BASE) + rel_addr);
}

void perform_conditional_patch(ConditionalBytepatch& patch, bool option)
{
    std::vector<uint8_t> bytes = option ? patch.patch_bytes : patch.orig_bytes;

    HookHandler::get_handler().install_patch(patch.rel_addr, bytes);
}

class PauseLayerCallbackHelper : public cocos2d::CCNode {
public:
    void onPercentage(cocos2d::CCObject* /* target */) {
        auto gm = GameManager::sharedState();
        gm->toggleGameVariable(GameVariable::SHOW_PERCENTAGE);

        if (auto pl = gm->getPlayLayer(); pl != nullptr) {
            if (auto percent_label = dynamic_cast<cocos2d::CCLabelBMFont*>(pl->getChildByTag(662)); percent_label != nullptr) {
                percent_label->setVisible(gm->getGameVariable(GameVariable::SHOW_PERCENTAGE));
            }
        }
    }
};

// restart button
void PauseLayer_customSetup(PauseLayer* self)
{
    auto gm = GameManager::sharedState();
    bool restart_button_enabled = gm->getGameVariable(GameVariable::SHOW_RESTART);

    spdlog::get("global")->info("PauseLayer::customSetup called ({})", restart_button_enabled);

    ConditionalBytepatch patch = {
        0x1ECE40, // PauseLayer::customSetup
        { 0x00, 0xBF }, // nop
        { 0x12, 0xD1 } // ble
    };

    perform_conditional_patch(patch, restart_button_enabled);

    HookHandler::orig<&PauseLayer_customSetup>(self);

    auto child_count = self->getChildren()->count();

    auto menu_offset = PlatformToolbox::isControllerConnected() ? -3 : 0;

    auto checkbox_menu = reinterpret_cast<cocos2d::CCMenu*>(self->getChildren()->objectAtIndex(child_count - 5 + menu_offset));

    auto bar_text = reinterpret_cast<cocos2d::CCLabelBMFont*>(self->getChildren()->objectAtIndex(child_count - 6 + menu_offset));
    bar_text->setString("Bar");
    bar_text->setScale(0.42f);

    self->createToggleButton(
        "%",
        static_cast<cocos2d::SEL_MenuHandler>(
            &PauseLayerCallbackHelper::onPercentage),
        !(gm->getGameVariable(GameVariable::SHOW_PERCENTAGE)),
        checkbox_menu,
        cocos2d::CCPoint(
            bar_text->getPositionX() + 45.0f,
            bar_text->getPositionY()));
}

void PauseLayer_onProgressBar(PauseLayer* self, cocos2d::CCObject* target) {
    HookHandler::orig<&PauseLayer_onProgressBar>(self, target);

    auto gm = GameManager::sharedState();

    if (auto pl = gm->getPlayLayer(); pl != nullptr) {
        if (auto percent_label = dynamic_cast<cocos2d::CCLabelBMFont*>(pl->getChildByTag(662)); percent_label != nullptr) {
            auto director = cocos2d::CCDirector::sharedDirector();
            auto w_size = director->getWinSize();

            if (gm->getShowProgressBar()) {
                percent_label->setPositionX((w_size.width / 2) + 110.0f);
                percent_label->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
            } else {
                percent_label->setPositionX(w_size.width / 2);
                percent_label->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
            }
        }
    }
}

// practice music
void PlayLayer_togglePracticeMode(PlayLayer* self, bool change)
{
    auto gm = GameManager::sharedState();
    bool practice_music_enabled = gm->getGameVariable(GameVariable::PRACTICE_MUSIC);

    spdlog::get("global")->info("PlayLayer::togglePracticeMode called ({})", practice_music_enabled);

    std::vector<ConditionalBytepatch> patches {
        {
            0x1B8C70, // PlayLayer::togglePracticeMode
            { 0x03, 0xE0 }, // nop
            { 0x1D, 0xB1 } // cbz
        },
        {
            0x1B8ADA, // PlayLayer::resetLevel
            { 0x00, 0xBF }, // nop
            { 0x79, 0xD1 } // bne
        },
        {
            0x1B5CF6, // PlayLayer::destroyPlayer
            { 0x00, 0xBF }, // nop
            { 0x93, 0xB9 } // cbnz
        },
        {
            0x1B5D22, // PlayLayer::destroyPlayer
            { 0x00, 0xBF }, // nop
            { 0x1B, 0xB9 } // cbnz
        },
        {
            0x001B60B8, // PlayLayer::updateVisibility
            { 0x1B, 0x45 }, // cmp r3, r3
            { 0x00, 0x2B } // cmp r3, #0x0
        }
    };

    for (auto& patch : patches) {
        perform_conditional_patch(patch, practice_music_enabled);
    }

    HookHandler::orig<&PlayLayer_togglePracticeMode>(self, change);
}

// Unlock All
bool GJGarageLayer_init(GJGarageLayer* self)
{
    auto gm = GameManager::sharedState();
    bool unlock_all_enabled = gm->getGameVariable(GameVariable::UNLOCK_ALL);

    spdlog::get("global")->info("GJGarageLayer::init called");

    std::vector<ConditionalBytepatch> patches {
        {
            0x1BD7AA, // GameManager::isColorUnlocked
            { 0x01, 0x20, 0x1C, 0xBD }, // mov r0, #0x1; pop {r2, r3, r4, pc}
            { 0x01, 0x24, 0x03, 0x29 } // mov r4, #0x1; cmp r1, #0x3
        },
        {
            0x1BD6AA, // GameManager::isColorUnlocked
            { 0x01, 0x20, 0x1C, 0xBD }, // mov r0, #0x1; pop {r2, r3, r4, pc}
            { 0x0A, 0xB9, 0x04, 0x29 } // cbnz r2, #0x6; cmp r1, #0x4
        },
    };

    for (auto& patch : patches) {
        perform_conditional_patch(patch, unlock_all_enabled);
    }

    return HookHandler::orig<&GJGarageLayer_init>(self);
}

// noclip
void PlayLayer_destroyPlayer(PlayLayer* self, PlayerObject* player)
{
    auto gm = GameManager::sharedState();
    bool noclip_enabled = gm->getGameVariable(GameVariable::IGNORE_DAMAGE);
    bool fast_reset_enabled = gm->getGameVariable(GameVariable::FAST_PRACTICE_RESET);

    // yes this disables special noclip
    if (noclip_enabled)
        return;

    // maybe make this dynamic in the future? honestly idk
    ConditionalBytepatch fast_reset_patch = {
        0x1B5D7E, // PlayLayer::destroyPlayer
        { 0x4F, 0xF0, 0x7C, 0x55 }, // mov.w r5, #0x3F000000
        { 0x4F, 0xF0, 0x7E, 0x55 } // mov.w r5, #0x3F800000
    };

    perform_conditional_patch(fast_reset_patch, fast_reset_enabled && self->getPracticeMode());

    auto illegitimate_run = false;
    if (auto extension_object = dynamic_cast<PlayLayerExt*>(self->getUserObject()); extension_object != nullptr) {
        illegitimate_run = extension_object->getIsIllegitimate();
    }

    ConditionalBytepatch no_save_patch = {
        0x1B5C80, // PlayLayer::destroyPlayer
        { 0x37, 0xE0 }, // b #0x37
        { 0x37, 0xD1 } // bne #0x37
    };

    perform_conditional_patch(no_save_patch, illegitimate_run);

    HookHandler::orig<&PlayLayer_destroyPlayer>(self, player);
}

// restart button
void PlayLayer_levelComplete(PlayLayer* self)
{
    spdlog::get("global")->info("PlayLayer::levelComplete called");

    if (auto extension_object = dynamic_cast<PlayLayerExt*>(self->getUserObject()); extension_object != nullptr) {
        if (extension_object->getIsIllegitimate()) {
            spdlog::get("global")->info("legitimacy tripped, exiting level");
            self->onQuit();
            return;
        }
    }

    HookHandler::orig<&PlayLayer_levelComplete>(self);
}

bool PlayLayer_init(PlayLayer* self, GJGameLevel* level)
{
    spdlog::get("global")->info("PlayLayer::init called");

    auto return_value = HookHandler::orig<&PlayLayer_init>(self, level);

    auto gm = GameManager::sharedState();
    bool secret_enabled = gm->getGameVariable(GameVariable::REPLAY_CONTROLS);

    auto extension_object = new PlayLayerExt();

    if (secret_enabled) {
        auto should_record = ptr_to_offset<bool>(self, 0x248);

        *should_record = true;
        spdlog::get("global")->info("recording feature force enabled");
    }

    // bug in pre 1.93, would allow for completion of deleted levels
    auto lvl_objs = get_from_offset<cocos2d::CCArray*>(self, 0x170);
    uint32_t obj_count = 0;
    for (uint32_t i = 0; i < lvl_objs->count(); i++) {
        obj_count += reinterpret_cast<cocos2d::CCArray*>(lvl_objs->objectAtIndex(i))->count();
        // don't count all the objects in the level if we know there's enough to get past the check
        if (obj_count > 1) {
            break;
        }
    }

    // end portal counts as an object, don't count it here obviously
    if (obj_count <= 1 && self->getLevel()->getStars() >= 1) {
        extension_object->setIsIllegitimate(true);

        auto warning_text = cocos2d::CCLabelBMFont::create("I see you :)", "bigFont.fnt");
        self->getGameLayer()->addChild(warning_text, 4);
        warning_text->setPosition(700.0f, 240.0f);
    }

    if (level->getAudioTrack() >= 18) {
        GameSoundManager::sharedManager()->enableMetering();
        auto metering_enabled = ptr_to_offset<bool>(self, 0x271);
        *metering_enabled = true;
    }

    auto director = cocos2d::CCDirector::sharedDirector();
    auto w_size = director->getWinSize();

    auto percentage_precision = GameManager::sharedState()->getGameVariable(GameVariable::ACCURATE_PERCENTAGE) ? 2 : 0;
    auto percentage_string = fmt::format("{:.{}f}%", 0.0f, percentage_precision);

    auto percentage_label = cocos2d::CCLabelBMFont::create(percentage_string.c_str(), "bigFont.fnt");
    self->addChild(percentage_label, 21, 662);

    if (gm->getShowProgressBar()) {
        percentage_label->setPositionX((w_size.width / 2) + 110.0f);
        percentage_label->setAnchorPoint(cocos2d::CCPoint(0.0f, 0.0f));
    } else {
        percentage_label->setPositionX(w_size.width / 2);
        percentage_label->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.0f));
    }

    percentage_label->setPositionY(director->getScreenTop() - 16.0f);
    percentage_label->setScale(0.5f);

    if (!gm->getGameVariable(GameVariable::SHOW_PERCENTAGE)) {
        percentage_label->setVisible(false);
    }

    bool noclip_enabled = gm->getGameVariable(GameVariable::IGNORE_DAMAGE);
    if (noclip_enabled) {
        spdlog::get("global")->info("noclip enabled");

        extension_object->setIsIllegitimate(true);
    }

    self->setUserObject(extension_object);

    if (extension_object->getIsIllegitimate())
        spdlog::get("global")->info("legitimacy has been tripped for run");

    return return_value;
}

void PlayLayer_setupReplay(PlayLayer* self, std::string replay)
{
    HookHandler::orig<&PlayLayer_setupReplay>(self, replay); // NOLINT(performance-unnecessary-value-param)

    if (self->getPlaybackMode()) {
        auto label = cocos2d::CCLabelBMFont::create("Playback", "bigFont.fnt");
        self->addChild(label, 10);

        auto director = cocos2d::CCDirector::sharedDirector();
        auto screen_left = director->getScreenLeft();
        auto screen_top = director->getScreenTop();

        label->setPositionX(screen_left + 43.0f);
        label->setPositionY(screen_top - 10.0f);

        label->setScale(0.5f);
        label->setOpacity(127);
    }
}

void PlayLayer_updateProgressbar(PlayLayer* self) {
    HookHandler::orig<&PlayLayer_updateProgressbar>(self);

    auto player = get_from_offset<PlayerObject*>(self, 0x28C);
    auto max_level_size = get_from_offset<float>(self, 0x1D0);

    auto percentage = std::min((player->getPositionX() / max_level_size) * 100.0f, 100.0f);

    auto label = reinterpret_cast<cocos2d::CCLabelBMFont*>(self->getChildByTag(662));
    if (label != nullptr) {
        // todo, this probably should be optimized since this reduces fps
        auto percentage_precision = GameManager::sharedState()->getGameVariable(GameVariable::ACCURATE_PERCENTAGE) ? 2 : 0;
        auto percentage_string = fmt::format("{:.{}f}%", percentage, percentage_precision);

        label->setString(percentage_string.c_str(), true);
    }
}

void EditLevelLayer_onShare(EditLevelLayer* self, cocos2d::CCObject* target) {
    HookHandler::orig<&EditLevelLayer_onShare>(self, target);

    auto gm = GameManager::sharedState();
    bool vfb_enabled = gm->getGameVariable(GameVariable::BYPASS_VERIFY);

    // maybe make this dynamic in the future? honestly idk
    ConditionalBytepatch fast_reset_patch = {
        0x1F5D4E, // EditLevelLayer::onShare
        { 0x06, 0xE0 }, // b
        { 0x30, 0xB9 } // cbnz
    };

    perform_conditional_patch(fast_reset_patch, vfb_enabled);
}

void FMODAudioEngine_playBackgroundMusic(FMODAudioEngine* self, std::string path, bool loop, bool paused) {
    HookHandler::orig<&FMODAudioEngine_playBackgroundMusic>(self, path, loop, paused);

    if (path != "menuLoop.mp3") {
        // ignore menu song
        if (GameManager::sharedState()->getGameVariable(GameVariable::KEEP_AWAKE)) {
            JNI::keep_screen_awake();
        }
    }
}

void FMODAudioEngine_pauseBackgroundMusic(FMODAudioEngine* self) {
    HookHandler::orig<&FMODAudioEngine_pauseBackgroundMusic>(self);

    if (GameManager::sharedState()->getGameVariable(GameVariable::KEEP_AWAKE)) {
        JNI::remove_screen_awake();
    }
}

void FMODAudioEngine_stopBackgroundMusic(FMODAudioEngine* self, bool p1) {
    HookHandler::orig<&FMODAudioEngine_stopBackgroundMusic>(self, p1);

    if (GameManager::sharedState()->getGameVariable(GameVariable::KEEP_AWAKE)) {
        JNI::remove_screen_awake();
    }
}

void GameManager_firstLoad(GameManager* self)
{
    HookHandler::orig<&GameManager_firstLoad>(self);
    self->setGameVariable(GameVariable::KEEP_AWAKE, true);
}

bool LevelInfoLayer_init(LevelInfoLayer* self, GJGameLevel* lvl)
{
    bool ch_enabled = GameManager::sharedState()->getGameVariable(GameVariable::COPY_HACK);
    ConditionalBytepatch copy_patch = {
            0x1F56BE, // LevelInfoLayer::init
            { 0x00, 0xBF }, // nop
            { 0x34, 0xD0 } // beq
    };

    ConditionalBytepatch texture_patch = {
            0x1F56CC, // LevelInfoLayer::init
            { 0x00, 0xBF }, // nop
            { 0x02, 0xD1 } // bne
    };

    perform_conditional_patch(copy_patch, ch_enabled);
    perform_conditional_patch(texture_patch, ch_enabled);

    return HookHandler::orig<&LevelInfoLayer_init>(self, lvl);
}

void LevelInfoLayer_tryCloneLevel(LevelInfoLayer* self, cocos2d::CCObject* target) {
    // technically this is the lazy way
    bool ch_enabled = GameManager::sharedState()->getGameVariable(GameVariable::COPY_HACK);
    ConditionalBytepatch copy_patch = {
            0x1F358E, // LevelInfoLayer::tryCloneLevel
            { 0x00, 0xBF }, // nop
            { 0x06, 0xD1 } // bne
    };

    perform_conditional_patch(copy_patch, ch_enabled);

    return HookHandler::orig<&LevelInfoLayer_tryCloneLevel>(self, target);
}

bool ShareLevelSettingsLayer_init(ShareLevelSettingsLayer* self, GJGameLevel* lvl) {
    bool ch_enabled = GameManager::sharedState()->getGameVariable(GameVariable::COPY_HACK);
    if (ch_enabled) {
        lvl->setPassword(1);
    }

    if (HookHandler::orig<&ShareLevelSettingsLayer_init>(self, lvl)) {
        if (ch_enabled) {
            auto password_check = get_from_offset<CCMenuItemToggler*>(self, 0x1C8);
            password_check->setDontToggle(true);
            password_check->setEnabled(false);
            password_check->setColor({ 0xAB, 0xAB, 0xAB });

            auto password_label = get_from_offset<cocos2d::CCLabelBMFont*>(self, 0x1CC);
            password_label->setColor({ 0xAB, 0xAB, 0xAB });

            auto copyable_check = reinterpret_cast<CCMenuItemToggler*>(self->_getMenu()->getChildren()->objectAtIndex(1));
            copyable_check->setDontToggle(true);
            copyable_check->setEnabled(false);
            copyable_check->setColor({ 0xAB, 0xAB, 0xAB });

            auto copyable_label = reinterpret_cast<cocos2d::CCLabelBMFont*>(self->getInternalLayer()->getChildren()->objectAtIndex(4));
            copyable_label->setColor({ 0xAB, 0xAB, 0xAB });

            auto sorry = cocos2d::CCLabelBMFont::create("Copy protection broke :(\nSorry...", "goldFont.fnt");
            self->getInternalLayer()->addChild(sorry);

            auto director = cocos2d::CCDirector::sharedDirector();

            sorry->setPositionX(director->getScreenRight() / 2);
            sorry->setPositionY(director->getScreenTop() / 2);
            sorry->setScale(0.8f);
        }

        return true;
    }

    return false;
}

bool ShareLevelLayer_init(ShareLevelLayer* self, GJGameLevel* lvl) {
    bool ch_enabled = GameManager::sharedState()->getGameVariable(GameVariable::COPY_HACK);
    if (ch_enabled) {
        lvl->setPassword(1);
    }

    return HookHandler::orig<&ShareLevelLayer_init>(self, lvl);
}

} // namespace

namespace Hacks {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook(&PauseLayer::customSetup, PauseLayer_customSetup)
        .add_hook("_ZN13GJGarageLayer4initEv", GJGarageLayer_init)
        .add_hook("_ZN9PlayLayer4initEP11GJGameLevel", PlayLayer_init)
        .add_hook(&PlayLayer::togglePracticeMode, PlayLayer_togglePracticeMode)
        .add_hook(&PlayLayer::destroyPlayer, PlayLayer_destroyPlayer)
        .add_hook(&PlayLayer::levelComplete, PlayLayer_levelComplete)
        .add_hook(&PlayLayer::setupReplay, PlayLayer_setupReplay)
        .add_hook(&PlayLayer::updateProgressbar, &PlayLayer_updateProgressbar)
        .add_hook(&PauseLayer::onProgressBar, &PauseLayer_onProgressBar)
        .add_hook(&EditLevelLayer::onShare, &EditLevelLayer_onShare)
        .add_hook(&FMODAudioEngine::playBackgroundMusic, FMODAudioEngine_playBackgroundMusic)
        .add_hook(&FMODAudioEngine::pauseBackgroundMusic, FMODAudioEngine_pauseBackgroundMusic)
        .add_hook(&FMODAudioEngine::stopBackgroundMusic, FMODAudioEngine_stopBackgroundMusic)
        .add_hook(&GameManager::firstLoad, GameManager_firstLoad)
        .add_hook("_ZN14LevelInfoLayer4initEP11GJGameLevel", LevelInfoLayer_init)
        .add_hook("_ZN15ShareLevelLayer4initEP11GJGameLevel", ShareLevelLayer_init)
        .add_hook("_ZN23ShareLevelSettingsLayer4initEP11GJGameLevel", ShareLevelSettingsLayer_init)
        .add_hook("_ZN14LevelInfoLayer13tryCloneLevelEPN7cocos2d8CCObjectE", LevelInfoLayer_tryCloneLevel);

    Hacks::Speedhack::Module().initialize();
}
} // namespace Hacks