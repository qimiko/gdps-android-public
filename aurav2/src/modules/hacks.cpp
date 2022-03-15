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

float get_interval_for_value(float value)
{
    float val = 1.0 + (value * 0.25);
    return std::min(std::max(val, 0.1f), 3.0f);
}

int get_save_value_for_interval(int old_val, int change)
{
    if (get_interval_for_value(old_val) == get_interval_for_value(old_val + change)) {
        return old_val;
    } else {
        return old_val + change;
    }
}

float get_speedhack_interval()
{
    auto gm = GameManager::sharedState();
    auto speedhack_interval = gm->getIntGameVariable("5099");

    return get_interval_for_value(speedhack_interval);
}

void set_speedhack_value(float interval)
{
    if (auto pl = GameManager::sharedState()->getPlayLayer(); pl != nullptr) {
        if (auto pl_ext = dynamic_cast<Hacks::PlayLayer_ext*>(pl->getUserObject()); pl_ext != nullptr) {
            if (interval != 1.0f) {
                pl_ext->setIsIllegitimate(true);
            }
        }
    }

    auto audio_engine = FMODAudioEngine::sharedEngine();
    auto control = get_from_offset<FMOD::ChannelControl*>(audio_engine, 0x11C);

    control->setPitch(interval);
}

class PauseLayerCallbackHelper : public cocos2d::CCNode {
private:
    void updateSpeedhackLabel()
    {
        auto label = reinterpret_cast<cocos2d::CCLabelBMFont*>(this->getChildByTag(41));
        if (label != nullptr) {
            auto speedhack_interval = get_speedhack_interval();
            auto speedhack_string = cocos2d::CCString::createWithFormat("%.2fx", speedhack_interval);

            set_speedhack_value(speedhack_interval);
            label->setString(speedhack_string->getCString(), true);

            if (auto pl = GameManager::sharedState()->getPlayLayer(); pl != nullptr) {
                if (auto speedhack_pl_label = dynamic_cast<cocos2d::CCLabelBMFont*>(pl->getChildByTag(661)); speedhack_pl_label != nullptr) {
                    speedhack_pl_label->setString(speedhack_string->getCString(), true);
                }
            }
        }
    }

public:
    void onBtnDown(cocos2d::CCObject* /* target */)
    {
        auto gm = GameManager::sharedState();
        auto speedhack_interval = gm->getIntGameVariable("5099");

        gm->setIntGameVariable("5099",
            get_save_value_for_interval(speedhack_interval, -1));

        this->updateSpeedhackLabel();
    }

    void onBtnUp(cocos2d::CCObject* /* target */)
    {
        auto gm = GameManager::sharedState();
        auto speedhack_interval = gm->getIntGameVariable("5099");

        gm->setIntGameVariable("5099",
            get_save_value_for_interval(speedhack_interval, 1));

        this->updateSpeedhackLabel();
    }

    void onPercentage(cocos2d::CCObject* /* target */) {
        auto gm = GameManager::sharedState();
        gm->toggleGameVariable("3098");

        if (auto pl = gm->getPlayLayer(); pl != nullptr) {
            if (auto percent_label = dynamic_cast<cocos2d::CCLabelBMFont*>(pl->getChildByTag(662)); percent_label != nullptr) {
                percent_label->setVisible(gm->getGameVariable("3098"));
            }
        }
    }
};

void CCScheduler_update(cocos2d::CCScheduler* self, float dt)
{
    auto game_manager = GameManager::sharedState();
    if (game_manager->getPlayLayer() != nullptr && game_manager->getGameVariable("4093")) {
        auto speedhack_interval = get_speedhack_interval();

        set_speedhack_value(speedhack_interval);
        return HookHandler::orig<&CCScheduler_update>(self, dt * speedhack_interval);
    } else {
        set_speedhack_value(1.0f);
        return HookHandler::orig<&CCScheduler_update>(self, dt);
    }
}

// restart button
void PauseLayer_customSetup(PauseLayer* self)
{
    auto gm = GameManager::sharedState();
    bool restart_button_enabled = gm->getGameVariable("4096");

    spdlog::get("global")->info("PauseLayer::customSetup called ({})", restart_button_enabled);

    ConditionalBytepatch patch = {
        0x1ECE40, // PauseLayer::customSetup
        { 0x00, 0xBF }, // nop
        { 0x12, 0xD1 } // ble
    };

    perform_conditional_patch(patch, restart_button_enabled);

    auto director = cocos2d::CCDirector::sharedDirector();
    auto screen_right = director->getScreenRight();
    auto screen_top = director->getScreenTop();
    auto screen_bottom = director->getScreenBottom();

    bool speedhack_controls_enabled = gm->getGameVariable("4093");
    if (speedhack_controls_enabled) {

        auto time_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
        self->addChild(time_sprite);
        time_sprite->setPosition(
            cocos2d::CCPoint(screen_right - 100.0f, screen_top - 70.0f));

        auto items_menu = cocos2d::CCMenu::create();
        self->addChild(items_menu);
        items_menu->setPosition(screen_right - 75.0f, screen_top - 70.0f);

        // arrow begins pointing up
        auto speedhack_down = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        speedhack_down->setRotation(90.0f);
        speedhack_down->setScale(0.75f);

        auto speedhack_up = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        speedhack_up->setFlipX(true);
        speedhack_up->setRotation(90.0f);
        speedhack_up->setScale(0.75f);

        auto speedhack_down_btn = CCMenuItemSpriteExtra::create(
            speedhack_down, nullptr, self,
            static_cast<cocos2d::SEL_MenuHandler>(
                &PauseLayerCallbackHelper::onBtnDown));
        items_menu->addChild(speedhack_down_btn);
        speedhack_down_btn->setPositionY(-30.0f);

        auto speedhack_up_btn = CCMenuItemSpriteExtra::create(
            speedhack_up, nullptr, self,
            static_cast<cocos2d::SEL_MenuHandler>(
                &PauseLayerCallbackHelper::onBtnUp));
        items_menu->addChild(speedhack_up_btn);
        speedhack_up_btn->setPositionY(30.0f);

        auto speedhack_interval = get_speedhack_interval();
        auto speedhack_string = cocos2d::CCString::createWithFormat("%.2fx", speedhack_interval);

        auto speedhack_label = cocos2d::CCLabelBMFont::create(
            speedhack_string->getCString(), "bigFont.fnt");
        self->addChild(speedhack_label, 0, 41);

        speedhack_label->setPosition(screen_right - 60.0f, screen_top - 70.0f);
        speedhack_label->setScale(0.5f);
    }

    HookHandler::orig<&PauseLayer_customSetup>(self);

    auto child_count = self->getChildren()->count();
    auto checkbox_menu = reinterpret_cast<cocos2d::CCMenu*>(self->getChildren()->objectAtIndex(child_count - 5));

    auto bar_text = reinterpret_cast<cocos2d::CCLabelBMFont*>(self->getChildren()->objectAtIndex(child_count - 6));
    bar_text->setString("Bar");
    bar_text->setScale(0.42f);

    self->createToggleButton(
        "%",
        static_cast<cocos2d::SEL_MenuHandler>(
            &PauseLayerCallbackHelper::onPercentage),
        !(gm->getGameVariable("3098")),
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
    bool practice_music_enabled = gm->getGameVariable("4098");

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
    bool unlock_all_enabled = gm->getGameVariable("4097");

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
    bool noclip_enabled = gm->getGameVariable("4099");
    bool fast_reset_enabled = gm->getGameVariable("4095");

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
    if (auto extension_object = dynamic_cast<Hacks::PlayLayer_ext*>(self->getUserObject()); extension_object != nullptr) {
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

    if (auto extension_object = dynamic_cast<Hacks::PlayLayer_ext*>(self->getUserObject()); extension_object != nullptr) {
        if (extension_object->getIsIllegitimate()) {
            spdlog::get("global")->info("legitimacy tripped, exiting level");
            self->onQuit();
            return;
        }
    }

    HookHandler::orig<&PlayLayer_levelComplete>(self);
}

const std::string& (*string_assign)(const std::string&, const std::string&);

// persistent clipboard
void EditorUI_destructor(EditorUI* self)
{
    spdlog::get("global")->info("EditorUI::~EditorUI called");

    auto string_ptr = get_from_offset<std::string>(self, 0x264);

    auto manager = GameManager::sharedState();
    bool clipboard_enabled = manager->getGameVariable("4092");
    if (clipboard_enabled) {
        manager->setEditorCopyString(string_ptr);
    }

    return HookHandler::orig<&EditorUI_destructor>(self);
}

bool EditorUI_init(EditorUI* self, LevelEditorLayer* editor)
{
    spdlog::get("global")->info("EditorUI::init called");

    auto result = HookHandler::orig<&EditorUI_init>(self, editor);

    auto manager = GameManager::sharedState();

    auto copy_string = ptr_to_offset<std::string>(manager, 0x114);

    spdlog::get("global")->trace("copy string: `%s`", copy_string->c_str());

    if (!copy_string->empty()) {
        auto string_ptr = ptr_to_offset<std::string>(self, 0x264);

        // we must go through gd if we wish to assign strings manually
        string_assign = reinterpret_cast<decltype(string_assign)>(
            reinterpret_cast<uintptr_t>(GLOBAL_BASE) + 0x416E94);
        string_assign(*string_ptr, *copy_string);

        spdlog::get("global")->trace("copy setting from `%s`", string_ptr->c_str());

        self->updateButtons();

        //		*string_ptr = copy_string;
    }
    return result;
}

bool PlayLayer_init(PlayLayer* self, GJGameLevel* level)
{
    spdlog::get("global")->info("PlayLayer::init called");

    auto return_value = HookHandler::orig<&PlayLayer_init>(self, level);

    auto gm = GameManager::sharedState();
    bool secret_enabled = gm->getGameVariable("4094");

    auto extension_object = new Hacks::PlayLayer_ext();

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

    auto percentage_precision = GameManager::sharedState()->getGameVariable("4089") ? 2 : 0;
    auto percentage_string = cocos2d::CCString::createWithFormat("%.*f%%", percentage_precision, 0.0f);

    auto percentage_label = cocos2d::CCLabelBMFont::create(percentage_string->getCString(), "bigFont.fnt");
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

    if (!gm->getGameVariable("3098")) {
        percentage_label->setVisible(false);
    }

    bool speedhack_controls_enabled = gm->getGameVariable("4093");
    if (speedhack_controls_enabled) {
        auto time_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
        self->addChild(time_sprite, 21);

        auto screen_side = gm->getGameVariable("0015") ? director->getScreenRight() : director->getScreenLeft();
        auto screen_positioning_scale = gm->getGameVariable("0015") ? -1 : 1;
        auto screen_top = director->getScreenTop();

        time_sprite->setPositionX(screen_side + screen_positioning_scale * 15.0f);
        time_sprite->setPositionY(screen_top - 15.0f);

        time_sprite->setOpacity(127);

        auto speedhack_interval = get_speedhack_interval();
        auto speedhack_string = cocos2d::CCString::createWithFormat("%.2fx", speedhack_interval);
        auto speedhack_label = cocos2d::CCLabelBMFont::create(
            speedhack_string->getCString(), "bigFont.fnt");

        self->addChild(speedhack_label, 21, 661);
        speedhack_label->setPosition(screen_side + screen_positioning_scale * 55.0f, screen_top - 15.0f);
        speedhack_label->setScale(0.5f);
        speedhack_label->setOpacity(127);
        if (gm->getGameVariable("0015")) {
            speedhack_label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentRight);
        }

        set_speedhack_value(speedhack_interval);

        spdlog::get("global")->info("speedhack controls enabled with interval {}", speedhack_interval);
    }

    bool noclip_enabled = gm->getGameVariable("4099");
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
        auto percentage_precision = GameManager::sharedState()->getGameVariable("4089") ? 2 : 0;
        auto percentage_string = cocos2d::CCString::createWithFormat("%.*f%%", percentage_precision, percentage);

        label->setString(percentage_string->getCString(), true);
    }
}

void EditLevelLayer_onShare(EditLevelLayer* self, cocos2d::CCObject* target) {
    HookHandler::orig<&EditLevelLayer_onShare>(self, target);

    auto gm = GameManager::sharedState();
    bool vfb_enabled = gm->getGameVariable("3001");

    // maybe make this dynamic in the future? honestly idk
    ConditionalBytepatch fast_reset_patch = {
        0x1F5D4E, // EditLevelLayer::onShare
        { 0x06, 0xE0 }, // b
        { 0x30, 0xB9 } // cbnz
    };

    perform_conditional_patch(fast_reset_patch, vfb_enabled);
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
        .add_hook("_ZN8EditorUID0Ev", EditorUI_destructor)
        .add_hook("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUI_init)
        .add_hook("_ZN7cocos2d11CCScheduler6updateEf", CCScheduler_update);
}
} // namespace Hacks