#include "modules/settings.hpp"

class SettingsPage : public cocos2d::CCLayer {
public:
    virtual void createPage() = 0;

    bool init() {
//        this->_internal_menu = cocos2d::CCMenu::create();
//        this->addChild(this->_internal_menu);
//        this->_internal_menu->setPosition(0.0f, 0.0f);

        return true;
    }

    void setDimensions(const cocos2d::CCSize dimensions) {
        this->_dimensions = dimensions;
    }

    void setWindowDimensions(const cocos2d::CCSize dimensions) {
        this->_window_dimensions = dimensions;
    }

    void setInternalMenu(cocos2d::CCMenu* menu) {
        this->_internal_menu = menu;
    }

    void onToggleVisibility(bool visible) {
        for (const auto& object : this->_menu_objects) {
            object->setVisible(visible);
        }
    }

protected:
    std::vector<cocos2d::CCNode*> _menu_objects;
    cocos2d::CCMenu* _internal_menu;
    cocos2d::CCSize _dimensions;
    cocos2d::CCSize _window_dimensions;
};

class ToggleSettingsPage : public SettingsPage {
protected:
    void addToggle(const char* name, bool default_on, cocos2d::SEL_MenuHandler callback = nullptr, const char* desc = nullptr)
    {
        auto pos = cocos2d::CCPoint(20.0f, this->getNextToggleY());

        auto toggle = GameToolbox::createToggleButton(name, callback, default_on, this->_internal_menu, pos, this, nullptr, 0.8, 0.5, 180.0f, cocos2d::CCPoint(8.0f, 0.0f), "bigFont.fnt", false);

        // the toggle is placed very strangely relative to objects, thanks rob?
        toggle->setPosition(-(_window_dimensions.width / 2) + 35.0f, this->getNextToggleMenuY());
        this->_menu_objects.push_back(toggle);
        this->_toggle_count++;

        if (desc)
        {
            auto info_offset = -17.0f;

            auto info_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            info_sprite->setScale(0.5f);
            auto info_btn = CCMenuItemSpriteExtra::create(info_sprite, nullptr, this, static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onInfoText));

            auto toggle_pos = toggle->getPosition();

            this->_internal_menu->addChild(info_btn);
            this->_menu_objects.push_back(info_btn);

            info_btn->setPosition(toggle_pos.x + info_offset, toggle_pos.y + 17.0f);

            auto desc_str = std::string(desc);

            auto desc_identifier = static_cast<uint32_t>(std::hash<std::string>{}(desc_str));
            this->_info_text.insert({ desc_identifier, desc_str });

            info_btn->setTag(desc_identifier);
            info_btn->setSizeMult(2.0f);
        }
    }

    float getNextToggleY() {
        return _dimensions.height - (this->_toggle_count * 40.0f) - 45.0f;
    }

    float getNextToggleMenuY() {
        return (_window_dimensions.height / 2) - ((this->_toggle_count + 1) * 40.0f) - 23.0f;
    }

    std::unordered_map<uint32_t, std::string> _info_text;
    uint32_t _toggle_count;

public:
    template <uint32_t V>
    void onToggleVariable(cocos2d::CCObject* taBarrySilbertet)
    {
        auto var_string = cocos2d::CCString::createWithFormat("%04d", V);

        GameManager::sharedState()->toggleGameVariable(var_string->getCString());
    }

    void onInfoText(cocos2d::CCObject* target) {
        auto n_target = dynamic_cast<cocos2d::CCNode*>(target);
        if (n_target == nullptr) {
            return;
        }

        auto identifier = n_target->getTag();
        auto desc = _info_text[identifier];

        FLAlertLayer::create(nullptr, "Info", desc.c_str(), "OK", nullptr, 300.0f)->show();
    }
};

struct SettingsItem {
    const char* title;
    const char* icon;
    float size;
    SettingsPage* page;
};

class AboutSettingsPage : public SettingsPage {
public:
    void createPage()
    {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto internal_layer = cocos2d::CCLayer::create();
        this->addChild(internal_layer);

        auto title_string = cocos2d::CCString::createWithFormat("1.9 GDPS u%i.%i.%i",
            CMakeConfiguration::VER_MAJOR, CMakeConfiguration::VER_MINOR, CMakeConfiguration::VER_PATCH);

        auto title = cocos2d::CCLabelBMFont::create(title_string->getCString(), "bigFont.fnt");
        internal_layer->addChild(title);
        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto version_string = cocos2d::CCString::createWithFormat(
            "%s-%s\n%s\nL%i C%i",
            CMakeConfiguration::BRANCH, CMakeConfiguration::HASH,
            CMakeConfiguration::COMPILE_TIME,
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

    void onSource(cocos2d::CCObject*)
    {
        cocos2d::CCApplication::sharedApplication()->openURL(Config::SOURCE_URL);
    }

    void onLicense(cocos2d::CCObject* target)
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

    CREATE_FUNC(AboutSettingsPage);
};

class GameplaySettingsPage : public ToggleSettingsPage {
public:
    void createPage() {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto title = cocos2d::CCLabelBMFont::create("Gameplay", "bigFont.fnt");
        this->addChild(title);

        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto gm = GameManager::sharedState();

        this->addToggle(
            "Auto-Retry",
            gm->getGameVariable("0026"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<26>));

        this->addToggle(
            "Auto-Checkpoints",
            gm->getGameVariable("0027"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<27>),
            "Automatically place checkpoints while in practice mode.");

        this->addToggle(
            "Safe Noclip",
            gm->getGameVariable("4099"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4099>),
            "Prevents the player from dying. Disables level completions.");

        this->addToggle(
            "Speedhack Controls",
            gm->getGameVariable("4093"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4093>),
            "Enables controlling the speed of levels. Disables level completions if used during a session.");

        this->addToggle(
            "Fast Practice Reset",
            gm->getGameVariable("4095"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4095>),
            "Shortens reset time in practice mode from 1 second to 0.5 seconds.");
    }

    CREATE_FUNC(GameplaySettingsPage);
};

class InterfaceSettingsPage : public ToggleSettingsPage {
public:
    void createPage() {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto title = cocos2d::CCLabelBMFont::create("Interface", "bigFont.fnt");
        this->addChild(title);

        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto gm = GameManager::sharedState();

        this->addToggle(
            "Flip 2-Player Controls",
            gm->getGameVariable("0010"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<10>),
            "Flip which side controls which player during 2-player dual mode.");

        this->addToggle(
            "Always Limit Controls",
            gm->getGameVariable("0011"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<11>),
            "Player 1 controls are limited to one side even when dual mode is inactive.");

        this->addToggle(
            "Flip pause button",
            gm->getGameVariable("0015"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<15>));

        this->addToggle(
            "Restart Button",
            gm->getGameVariable("4096"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4096>),
            "Enables a persistent restart button in the pause menu.");

        this->addToggle(
            "Accurate Percentage",
            gm->getGameVariable("4089"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4089>),
            "Shows an extra two digits of precision on the ingame progress bar. Does not apply anywhere else.");
    }

    CREATE_FUNC(InterfaceSettingsPage);
};

class TweaksSettingsPage : public ToggleSettingsPage {
public:
    void createPage() {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto title = cocos2d::CCLabelBMFont::create("Tweaks", "bigFont.fnt");
        this->addChild(title);

        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto gm = GameManager::sharedState();

        this->addToggle(
            "Smooth Fix",
            gm->getGameVariable("0023"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<23>),
            "Makes some optimizations that can reduce lag. Disable if game speed becomes inconsistent.");

        this->addToggle(
            "Disable explosion shake",
            gm->getGameVariable("0014"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<14>));

        this->addToggle(
            "Unlock All",
            gm->getGameVariable("4097"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4097>));

        this->addToggle(
            "Create Session",
            gm->getGameVariable("4090"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4090>));
    }

    CREATE_FUNC(TweaksSettingsPage);
};

class EditorSettingsPage : public ToggleSettingsPage {
public:
    void createPage()
    {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto title = cocos2d::CCLabelBMFont::create("Editor", "bigFont.fnt");
        this->addChild(title);

        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto gm = GameManager::sharedState();

        this->addToggle(
            "Increased Max Undo/Redo",
            gm->getGameVariable("0013"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<13>),
            "Increases undo limit in the level editor from 200 to 1000.");

        this->addToggle(
            "Small editor step",
            gm->getGameVariable("0035"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<35>),
            "Smallest editor move command uses 1 instead of 2");

        this->addToggle(
            "Persistent Clipboard",
            gm->getGameVariable("4092"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4092>),
            "Makes object clipboard transfer between editor sessions.");

        this->addToggle(
            "Show Unused Objects",
            gm->getGameVariable("4091"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4091>),
            "Adds various unused objects to the editor for use.");
    }

    CREATE_FUNC(EditorSettingsPage);
};

class SongsSettingsPage : public ToggleSettingsPage {
public:
    void createPage()
    {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto title = cocos2d::CCLabelBMFont::create("Songs", "bigFont.fnt");
        this->addChild(title);

        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto gm = GameManager::sharedState();

        this->addToggle(
            "Load songs to memory",
            gm->getGameVariable("0019"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<19>),
            "Songs are loaded into memory before playing. Increases load time but can improve performance.");

        this->addToggle(
            "No song limit",
            gm->getGameVariable("0018"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<18>),
            "Custom songs are no longer auto-removed to save space.");

        this->addToggle(
            "Higher audio quality\n(req. restart)",
            gm->getGameVariable("0022"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<22>),
            "Switches samplerate from 24000 to 44100");

        this->addToggle(
            "Practice Music",
            gm->getGameVariable("4098"),
            static_cast<cocos2d::SEL_MenuHandler>(&ToggleSettingsPage::onToggleVariable<4098>),
            "Uses the song of the current level in practice mode.");

        auto song_browser_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_savedSongsBtn_001.png");
        auto song_browser_button = CCMenuItemSpriteExtra::create(song_browser_sprite, nullptr, this, static_cast<cocos2d::SEL_MenuHandler>(&SongsSettingsPage::onSongBrowser));

        this->_internal_menu->addChild(song_browser_button);
        this->_menu_objects.push_back(song_browser_button);

        song_browser_button->setPosition(-((_window_dimensions.width / 2) - 5), -(_window_dimensions.height / 2) + 5);
    }

    void onSongBrowser(cocos2d::CCObject* target) {
      auto song_browser = GJSongBrowser::create(nullptr);

      auto director = cocos2d::CCDirector::sharedDirector();
      director->getRunningScene()->addChild(song_browser, 106);

      song_browser->showLayer(false);
    }

    CREATE_FUNC(SongsSettingsPage);
};

class GraphicsSettingsPage : public SettingsPage {
public:
    void createPage()
    {
        float width = this->_dimensions.width;
        float height = this->_dimensions.height;

        auto title = cocos2d::CCLabelBMFont::create("Graphics", "bigFont.fnt");
        this->addChild(title);

        title->setPosition(width / 2, height - 10.0f);
        title->setScale(0.75f);

        auto ccd = cocos2d::CCDirector::sharedDirector();
        this->_selected_quality = ccd->getLoadedTextureQuality();
        if (static_cast<int>(this->_selected_quality) > 3) {
            this->_selected_quality = cocos2d::TextureQuality::kCCTextureQualityHigh;
        }

        auto apply_sprite = ButtonSprite::create("Apply", 80, 0, 1.0f, false);

        this->_apply_button = CCMenuItemSpriteExtra::create(apply_sprite, nullptr, this, static_cast<cocos2d::SEL_MenuHandler>(&GraphicsSettingsPage::onApply));

        this->_internal_menu->addChild(this->_apply_button);
        this->_menu_objects.push_back(this->_apply_button);
        _apply_button->setPosition((-_window_dimensions.width / 2) + (width / 2) + 15.0f, -(_window_dimensions.height / 2) + 35.0f);

        this->_quality_label = cocos2d::CCLabelBMFont::create(labelForQuality(this->_selected_quality), "bigFont.fnt");
        this->addChild(this->_quality_label);
        this->_quality_label->setPosition(width / 2, (height / 2) + 4.0f);
        this->_quality_label->setScale(1.0f);

        auto sprite_center = cocos2d::CCPoint((-_window_dimensions.width / 2) + (width / 2), 0.0f);

        auto next_sprite = cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
        next_sprite->setScale(2.0f);
        auto next_btn = CCMenuItemSpriteExtra::create(next_sprite, nullptr, this, static_cast<cocos2d::SEL_MenuHandler>(&GraphicsSettingsPage::onNext));
        this->_internal_menu->addChild(next_btn);
        this->_menu_objects.push_back(next_btn);

        next_btn->setPosition(sprite_center.x + 85.0f + 13.0f, sprite_center.y);

        auto prev_sprite = cocos2d::CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
        prev_sprite->setScale(2.0f);
        auto prev_btn = CCMenuItemSpriteExtra::create(prev_sprite, nullptr, this, static_cast<cocos2d::SEL_MenuHandler>(&GraphicsSettingsPage::onPrev));
        this->_internal_menu->addChild(prev_btn);
        this->_menu_objects.push_back(prev_btn);

        prev_btn->setPosition(sprite_center.x - 85.0f + 13.0f, sprite_center.y);
    }

    void onPrev(cocos2d::CCObject*)
    {
        auto btn = static_cast<int32_t>(this->_selected_quality);
        btn--;
        this->updateToggles(btn);
    }

    void onNext(cocos2d::CCObject*)
    {
        auto btn = static_cast<int32_t>(this->_selected_quality);
        btn++;
        this->updateToggles(btn);
    }

    void onApply(cocos2d::CCObject*)
    {
        auto gm = GameManager::sharedState();
        gm->setTexQuality(this->_selected_quality);

        auto ccd = cocos2d::CCDirector::sharedDirector();

        auto c_quality = ccd->getLoadedTextureQuality();

        ccd->updateContentScale(this->_selected_quality);

        if (ccd->getLoadedTextureQuality() != c_quality) {
            gm->reloadAll(false, false, true);
        } else {
            FLAlertLayer::create(nullptr, "Info", "No changes were made to graphics quality.", "OK", nullptr)->show();
        }
    }

    CREATE_FUNC(GraphicsSettingsPage);

protected:
    const char* labelForQuality(cocos2d::TextureQuality quality)
    {
        switch (quality) {
            case cocos2d::kCCTextureQualityLow:
                return "Low";
            case cocos2d::kCCTextureQualityMedium:
                return "Medium";
            case cocos2d::kCCTextureQualityHigh:
            default:
                return "High";
        }
    }

    void updateToggles(uint32_t btn)
    {
        auto ccd = cocos2d::CCDirector::sharedDirector();

        if (btn > 3) {
            btn = 1;
        }

        if (btn < 1) {
            btn = 3;
        }

        this->_selected_quality = static_cast<cocos2d::TextureQuality>(btn);

        auto c_quality = ccd->getLoadedTextureQuality();

        if (this->_selected_quality != c_quality) {
            _apply_button->setColor({ 0xFF, 0xFF, 0xFF });
        } else {
            _apply_button->setColor({ 0xA6, 0xA6, 0xA6 });
        }

        this->_quality_label->setString(labelForQuality(this->_selected_quality));
    }

    cocos2d::CCLabelBMFont* _quality_label;
    CCMenuItemSpriteExtra* _apply_button;
    cocos2d::TextureQuality _selected_quality;
};

namespace {

void OptionsLayer_onOptions(OptionsLayer* self, cocos2d::CCObject* target)
{
    Settings::SettingsPopup::create()->show();
}

class LevelInfoLayerExt : public cocos2d::CCLayer {
public:
    void onOptions(cocos2d::CCObject * target)
    {
        Settings::SettingsPopup::create()->show();
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
        bool secret_enabled = gm->getGameVariable("4094");

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
        
        auto sp = Settings::SettingsPopup::create(5);
        self->addChild(sp, 100);
        return;
    } else {
        HookHandler::orig<&MenuLayer_openOptions>(self, openVideoSettings);
    }
}
}

namespace Settings
{

bool SettingsPopup::init(int page)
{
    if (this->initWithColor({ 0, 0, 0, 75 })) {
        constexpr auto width = 380.0f;
        constexpr auto height = 250.0f;

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

        this->internalLayer_->addChild(bg, -2);

        this->internalMenu_ = cocos2d::CCMenu::create();
        this->internalLayer_->addChild(this->internalMenu_, 3);

        auto close_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto close_btn = CCMenuItemSpriteExtra::create(
            close_sprite,
            nullptr,
            this,
            reinterpret_cast<cocos2d::SEL_MenuHandler>(&SettingsPopup::onClose));

        this->internalMenu_->addChild(close_btn);
        close_btn->setPosition(-((width / 2) - 5), (height / 2) - 5);

        auto tabs_bg = cocos2d::extension::CCScale9Sprite::create("square02_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        tabs_bg->setOpacity(75);
        tabs_bg->setContentSize(cocos2d::CCSize(100.0f, height - 20.0f));
        tabs_bg->setPosition(cocos2d::CCPoint(((window_size.width + width) / 2.0f) - 60.0f, window_size.height / 2));

        this->internalLayer_->addChild(tabs_bg, -1);

        std::array<SettingsItem, 7> items { {
            { "Gameplay", "GJ_gameplayBadge_001.png", 0.37f, GameplaySettingsPage::create() },
            { "Interface", "GJ_interfaceBadge_001.png", 0.36f, InterfaceSettingsPage::create() },
            { "Editor", "GJ_editorBadge_001.png", 0.5f, EditorSettingsPage::create() },
            { "Songs", "GJ_songsBadge_001.png", 0.5f, SongsSettingsPage::create() },
            { "Tweaks", "GJ_tweaksBadge_001.png", 0.47f, TweaksSettingsPage::create() },
            { "Graphics", "GJ_graphicsBadge_001.png", 0.36f, GraphicsSettingsPage::create() },
            { "About", "GJ_aboutBadge_001.png", 0.5f, AboutSettingsPage::create() },
        } };

        auto i = 0u;
        this->buttons_ = cocos2d::CCArray::create();
        this->layers_ = cocos2d::CCArray::create();

        this->buttons_->retain();
        this->layers_->retain();

        for (const auto& item : items) {
            auto toggle_sprite = SearchButton::create("GJ_longBtn01_001.png", item.title, item.size, item.icon);
            auto toggle_on_sprite = SearchButton::create("GJ_longBtn02_001.png", item.title, item.size, item.icon);

            auto toggle = CCMenuItemToggler::create(toggle_sprite, toggle_on_sprite, this, static_cast<cocos2d::SEL_MenuHandler>(&SettingsPopup::onToggleLayer));

            this->internalMenu_->addChild(toggle);
            toggle->setPosition((width / 2) - 60.0f, (height / 2) - 32.0f - (i * 31.0f));
            toggle->setTag(i);

            item.page->setDimensions(cocos2d::CCSize(width - 135.0f, height - 35.0f));
            item.page->setWindowDimensions(cocos2d::CCSize(width, height));
            item.page->setInternalMenu(this->internalMenu_);
            item.page->createPage();

            this->internalLayer_->addChild(item.page, 2);
            item.page->setPosition(cocos2d::CCPoint(((window_size.width - width) / 2.0f) + 13.0f, 52.0f));
            item.page->setVisible(false);

            this->buttons_->addObject(toggle);
            this->layers_->addObject(item.page);

            i++;
        }

        if (page == -1) {
            this->toggleLayer(i - 1);
        } else {
            this->toggleLayer(page);
        }

        return true;
    }

    return false;
}

void SettingsPopup::keyBackClicked()
{
    this->onClose(nullptr);
};

void SettingsPopup::onClose(cocos2d::CCObject*)
{
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);

    this->buttons_->release();
    this->layers_->release();
}

void SettingsPopup::onToggleLayer(cocos2d::CCObject* target)
{
    auto n_target = dynamic_cast<cocos2d::CCNode*>(target);
    if (n_target == nullptr) {
        return;
    }

    this->toggleLayer(n_target->getTag());
}

void SettingsPopup::toggleLayer(int layer)
{
    if (this->layers_ == nullptr) {
        return;
    }

    if (this->buttons_ == nullptr) {
        return;
    }

    for (auto i = 0u; i < this->buttons_->count(); i++) {
        auto current_button = dynamic_cast<CCMenuItemToggler*>(this->buttons_->objectAtIndex(i));
        if (current_button == nullptr) {
            continue;
        }

        if (i != layer) {
            current_button->toggle(false);
            current_button->setDontToggle(false);
        } else {
            if (!current_button->getIsActive()) {
                current_button->toggle(true);
                current_button->setDontToggle(true);
            }
        }
    }

    for (auto i = 0u; i < this->layers_->count(); i++) {
        auto selected_layer = dynamic_cast<SettingsPage*>(this->layers_->objectAtIndex(i));
        if (selected_layer == nullptr) {
            continue;
        }

        auto visibility = i == layer;
        selected_layer->setVisible(visibility);
        selected_layer->onToggleVisibility(visibility);
    }
}

void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook("_ZN12OptionsLayer9onOptionsEPN7cocos2d8CCObjectE", OptionsLayer_onOptions)
        .add_hook("_ZN14LevelInfoLayer4initEP11GJGameLevel", LevelInfoLayer_init)
        .add_hook("_ZN14EditLevelLayer4initEP11GJGameLevel", EditLevelLayer_init)
        .add_hook(&MenuLayer::openOptions, &MenuLayer_openOptions);
}

} // namespace Search