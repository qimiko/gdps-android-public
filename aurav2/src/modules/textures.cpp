#include "modules/textures.hpp"

namespace {
constexpr unsigned int SUPER_VALUE = 184594917;
constexpr auto SUPER_TEXTURE = "GJ_superCoin_001.png";

void LevelCell_loadCustomLevelCell(LevelCell* self)
{
    // oh no
    HookHandler::orig<&LevelCell_loadCustomLevelCell>(self);

    auto level = get_from_offset<GJGameLevel*>(self, 0x15C);
    spdlog::get("global")->trace("load custom level cell %i", level->getLevelID());

    auto featured = level->getFeatured();

    if (featured == SUPER_VALUE) {
        spdlog::get("global")->trace("supered level %s", level->getLevelName().c_str());
        auto menu = get_from_offset<cocos2d::CCMenu*>(self, 0x158);

        auto featured_coin = reinterpret_cast<cocos2d::CCSprite*>(
            menu->getChildren()->objectAtIndex(2));

        auto sprite_frame_cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
        auto special_frame = sprite_frame_cache->spriteFrameByName(SUPER_TEXTURE);

        featured_coin->setDisplayFrame(special_frame);
    }

    return;
}

void LevelInfoLayer_setupLevelInfo(LevelInfoLayer* self)
{
    HookHandler::orig<&LevelInfoLayer_setupLevelInfo>(self);

    auto level = get_from_offset<GJGameLevel*>(self, 0x13C);

    auto featured = level->getFeatured();

    if (featured == SUPER_VALUE) {
        auto featured_coin = get_from_offset<cocos2d::CCSprite*>(self, 0x17C);

        auto sprite_frame_cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
        auto special_frame = sprite_frame_cache->spriteFrameByName(SUPER_TEXTURE);

        featured_coin->setDisplayFrame(special_frame);
    }

    return;
}

class CustomSongWidgetExt : public CustomSongWidget {
public:
    void onDeleteSong(cocos2d::CCObject* target)
    {
        auto menu = get_from_offset<cocos2d::CCMenu*>(this, 0xF4);
        auto download_btn = get_from_offset<CCMenuItemSpriteExtra*>(this, 0x108);

        auto song_object = this->getSongObject();
        auto song_id = song_object->getSongID();

        auto music_download_manager = MusicDownloadManager::sharedState();
        music_download_manager->deleteSong(song_id);

        auto delete_button = menu->getChildByTag(27);
        delete_button->setVisible(false);

        this->songStateChanged();
    };
};

void CustomSongWidget_updateSongInfo(CustomSongWidget* self)
{
    HookHandler::orig<&CustomSongWidget_updateSongInfo>(self);

    auto show_playback_button = get_from_offset<bool>(self, 0x12D);
    if (show_playback_button) {
        return;
    }

    auto menu = get_from_offset<cocos2d::CCMenu*>(self, 0xF4);

    auto song_object = self->getSongObject();
    auto song_id = song_object->getSongID();

    auto music_download_manager = MusicDownloadManager::sharedState();

    auto potential_delete_btn = menu->getChildByTag(27);

    if (music_download_manager->isSongDownloaded(song_id)) {
        if (potential_delete_btn != nullptr) {
            potential_delete_btn->setVisible(true);
            return;
        }

        auto delete_sprite = cocos2d::CCSprite::createWithSpriteFrameName(
            "GJ_deleteSongBtn_001.png");
        auto delete_btn = CCMenuItemSpriteExtra::create(delete_sprite, nullptr, self,
            static_cast<cocos2d::SEL_MenuHandler>(
                &CustomSongWidgetExt::onDeleteSong));

        menu->addChild(delete_btn);

        auto cancel_download_btn = get_from_offset<CCMenuItemSpriteExtra*>(self, 0x10C);

        delete_btn->setTag(27);
        delete_btn->setPosition(cancel_download_btn->getPosition());
        delete_btn->setSizeMult(1.1f);
    } else {
        if (potential_delete_btn != nullptr) {
            potential_delete_btn->setVisible(false);
            return;
        }
    }
}

void GJSongBrowser_customSetup(GJSongBrowser* self)
{
    HookHandler::orig<&GJSongBrowser_customSetup>(self);

    auto next_page_button = get_from_offset<CCMenuItemSpriteExtra*>(self, 0x1C0);
    auto prev_page_button = get_from_offset<CCMenuItemSpriteExtra*>(self, 0x1C4);

    auto next_page_position = next_page_button->getPosition();
    auto prev_page_position = prev_page_button->getPosition();

    auto director = cocos2d::CCDirector::sharedDirector();

    auto window_size = director->getWinSize();

    auto fixed_next_point = cocos2d::CCPoint(-18.0f, window_size.height / 2.0f);
    next_page_button->setPosition(fixed_next_point);

    auto fixed_prev_point = cocos2d::CCPoint(-window_size.width + (18.0f * 3),
        window_size.height / 2.0f);
    prev_page_button->setPosition(fixed_prev_point);

    return;
}

cocos2d::CCArray* GJMoreGamesLayer_getMoreGamesList(GJMoreGamesLayer* self)
{
    auto game_array = cocos2d::CCArray::create();

    std::vector<cocos2d::CCString*> strings {
        cocos2d::CCString::create("robtop"),
        cocos2d::CCString::create("absolute"),
        cocos2d::CCString::create("aqua"),
        cocos2d::CCString::create("morphi"),
        cocos2d::CCString::create("squared"),
        cocos2d::CCString::create("zyl"),
    };

    for (const auto& string : strings) {
        game_array->addObject(string);
    }

    return game_array;
}

cocos2d::CCMenu* get_menu_bar_for_index(EditorUI* self, uint32_t ind, uint32_t page)
{
    // CCArray of EditButtonBars
    auto edit_categories = get_from_offset<cocos2d::CCArray*>(self, 0x1E0);

    auto button_bar = dynamic_cast<EditButtonBar*>(edit_categories->objectAtIndex(ind));

    // CCArray of `ButtonPage`s
    auto pages = get_from_offset<cocos2d::CCArray*>(button_bar, 0xEC);

    auto scroll_layer = reinterpret_cast<BoomScrollLayer*>(button_bar->getChildren()->objectAtIndex(0));
    auto extended_layer = reinterpret_cast<cocos2d::CCLayer*>(scroll_layer->getChildren()->objectAtIndex(0));

    auto scroll_layer_dot_node = reinterpret_cast<cocos2d::CCLayer*>(scroll_layer->getChildren()->objectAtIndex(1));
    auto scroll_layer_dot_container = get_from_offset<cocos2d::CCArray*>(scroll_layer, 0x118);

    while (pages->count() <= page) {
        // page creation stuffs
        auto last_page = dynamic_cast<ButtonPage*>(pages->lastObject());
        auto penultimate_page = dynamic_cast<ButtonPage*>(pages->objectAtIndex(pages->count() - 2));

        auto last_pos = last_page->getPosition();

        auto blank_page = ButtonPage::create(
            cocos2d::CCArray::create(),
            // todo, find the origin of this constant (likely changes per device?)
            cocos2d::CCPoint(last_page->getPositionX() - penultimate_page->getPositionX(), last_pos.y)
        );

//        pages->addObject(blank_page);
        extended_layer->addChild(blank_page);

        scroll_layer->addPage(blank_page, page);

//        blank_page->setVisible(false);

        auto page_dot = cocos2d::CCSprite::create("smallDot.png");
        scroll_layer_dot_container->addObject(page_dot);
        scroll_layer_dot_node->addChild(page_dot);
    }

    auto last_page = dynamic_cast<ButtonPage*>(pages->objectAtIndex(page));

    // CCMenu of `CreateMenuItem`
    return reinterpret_cast<cocos2d::CCMenu*>(
        last_page->getChildren()->objectAtIndex(0));
}

void EditorUI_setupCreateMenu(EditorUI* self)
{
    HookHandler::orig<&EditorUI_setupCreateMenu>(self);

    auto gm = GameManager::sharedState();
    auto custom_obj_enabled = gm->getGameVariable("4091");
    if (!custom_obj_enabled) {
        return;
    }

    auto chain_button_menu = get_menu_bar_for_index(self, 8, 5);

    auto circle_create_btn = self->getCreateBtn(725, 4);
    auto grass_lg_create_btn = self->getCreateBtn(406, 4);
    auto grass_md_create_btn = self->getCreateBtn(407, 4);
    auto grass_sm_create_btn = self->getCreateBtn(408, 4);

    chain_button_menu->addChild(circle_create_btn);
    chain_button_menu->addChild(grass_lg_create_btn);
    chain_button_menu->addChild(grass_md_create_btn);
    chain_button_menu->addChild(grass_sm_create_btn);

    // button size is 40.0f
    // initial position is -105, -92
    auto initial_position = cocos2d::CCPoint(-105.0f, -92.0f);
    auto btn_size = 40.0f;

    circle_create_btn->setPosition(initial_position.x + (btn_size * 5),
        initial_position.y);
    grass_lg_create_btn->setPosition(initial_position.x,
        initial_position.y - btn_size);
    grass_md_create_btn->setPosition(initial_position.x + 40.0f,
        initial_position.y - btn_size);
    grass_sm_create_btn->setPosition(initial_position.x + (btn_size * 2),
        initial_position.y - btn_size);

    auto spike_button_menu = get_menu_bar_for_index(self, 4, 2);

    auto spiky_ground_create_btn = self->getCreateBtn(421, 4);
    auto spiky_ground_edge_create_btn = self->getCreateBtn(422, 4);

    spike_button_menu->addChild(spiky_ground_create_btn);
    spike_button_menu->addChild(spiky_ground_edge_create_btn);

    spiky_ground_create_btn->setPosition(initial_position.x + (btn_size * 4),
        initial_position.y - btn_size);
    spiky_ground_edge_create_btn->setPosition(initial_position.x + (btn_size * 5),
        initial_position.y - btn_size);

    auto slope_button_menu = get_menu_bar_for_index(self, 3, 7);

    auto short_slope_btn = self->getCreateBtn(371, 4);
    auto long_slope_btn = self->getCreateBtn(372, 4);
    auto short_slope_corner_btn = self->getCreateBtn(373, 4);
    auto long_slope_corner_btn = self->getCreateBtn(374, 4);

    slope_button_menu->addChild(short_slope_btn);
    slope_button_menu->addChild(long_slope_btn);
    slope_button_menu->addChild(short_slope_corner_btn);
    slope_button_menu->addChild(long_slope_corner_btn);

    short_slope_btn->setPosition(initial_position.x + (btn_size * 0),
        initial_position.y);
    long_slope_btn->setPosition(initial_position.x + (btn_size * 1),
        initial_position.y);
    short_slope_corner_btn->setPosition(initial_position.x + (btn_size * 2),
        initial_position.y);
    long_slope_corner_btn->setPosition(initial_position.x + (btn_size * 3),
        initial_position.y);

    auto ground_menu_first = get_menu_bar_for_index(self, 7, 1);

    auto spiky_deco_btn = self->getCreateBtn(419, 4);
    ground_menu_first->addChild(spiky_deco_btn);

    spiky_deco_btn->setPosition(initial_position.x + (btn_size * 5),
        initial_position.y - btn_size);

    auto ground_menu_second = get_menu_bar_for_index(self, 7, 2);

    auto spiky_deco_edge_btn = self->getCreateBtn(420, 4);
    ground_menu_second->addChild(spiky_deco_edge_btn);

    spiky_deco_edge_btn->setPosition(initial_position.x + (btn_size * 0),
        initial_position.y);

    return;
}

void LoadingLayer_loadAssets(LoadingLayer* self)
{
    auto texture_idx = get_from_offset<int>(self, 0x11C);
    if (texture_idx == 1)
    {
        auto texture_cache = cocos2d::CCTextureCache::sharedTextureCache();
        texture_cache->addImage("GJ_modificationsSheet.png", false);
        auto sprite_frame_cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
        sprite_frame_cache->addSpriteFramesWithFile("GJ_modificationsSheet.plist");
    }

    HookHandler::orig<&LoadingLayer_loadAssets>(self);
}

bool ShareLevelLayer_init(ShareLevelLayer* self, GJGameLevel* lvl) {
    if (HookHandler::orig<&ShareLevelLayer_init>(self, lvl)) {

        auto textbox = dynamic_cast<TextArea*>(
            self->getInternalLayer()->getChildren()->objectAtIndex(4));

        if (textbox != nullptr) {

            auto dev = std::random_device();
            auto rng = std::mt19937(dev());
            auto distribution = std::uniform_int_distribution<std::mt19937::result_type>(1, 100);

            // easter egg
            if (distribution(rng) == 1) {
                textbox->setString("By sharing a level you give the rights to use the level (in any way) and your first born child to the one true goddess, zmx.");
            } else {
                textbox->setString("By sharing a level you give the rights to use the level (in any way) to the creators of the 1.9 GDPS.");
            }
        }

        return true;
    }

    return false;
}

void CCDirector_updateContentScale(cocos2d::CCDirector* self, cocos2d::TextureQuality n_quality) {
    spdlog::get("global")->info("updcontentscale: {}", n_quality);

    if (static_cast<int>(n_quality) == 0) {
        // this value is 0 when the game starts
        auto gm = GameManager::sharedState();
        n_quality = gm->getTexQuality();

        if (static_cast<int>(n_quality) == 0) {
            n_quality = cocos2d::TextureQuality::kCCTextureQualityMedium;
        }
    }

    if (n_quality == cocos2d::TextureQuality::kCCTextureQualityHigh) {
        n_quality = static_cast<cocos2d::TextureQuality>(4);
    }

    auto tex_quality = ptr_to_offset<cocos2d::TextureQuality>(self, 0xF0);

    // todo, recreate float to int algo that rob uses
    self->setContentScaleFactor(static_cast<float>(n_quality));

    auto csf = self->getContentScaleFactor();

    *tex_quality = static_cast<cocos2d::TextureQuality>(n_quality);
    return;
}

} // namespace

namespace Textures {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook(&LevelCell::loadCustomLevelCell, LevelCell_loadCustomLevelCell)
        .add_hook(&LevelInfoLayer::setupLevelInfo, LevelInfoLayer_setupLevelInfo)
        .add_hook(&CustomSongWidget::updateSongInfo, CustomSongWidget_updateSongInfo)
        .add_hook(&GJSongBrowser::customSetup, GJSongBrowser_customSetup)
        .add_hook(&GJMoreGamesLayer::getMoreGamesList, GJMoreGamesLayer_getMoreGamesList)
        .add_hook(&EditorUI::setupCreateMenu, EditorUI_setupCreateMenu)
        .add_hook(&LoadingLayer::loadAssets, LoadingLayer_loadAssets)
        .add_hook("_ZN15ShareLevelLayer4initEP11GJGameLevel", ShareLevelLayer_init)
        .add_hook("_ZN7cocos2d10CCDirector18updateContentScaleENS_14TextureQualityE", CCDirector_updateContentScale)
        .install_patch(0x46FDD4, {
                                     0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, // https://
                                     0x78, 0x79, 0x7a, 0x65, // xyze
                                     0x2e, 0x64, 0x65, 0x76, // .dev
                                     0x2f, 0x67, 0x64, // /gd
                                     0x2f, 0x72, 0x65, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, // /redirect
                                     0x2f, 0x25, 0x73, // /%s
                                     0x3f, 0x74, 0x72, 0x61, 0x63, 0x6b, 0x3d, 0x25, 0x69, // ?track=%i
                                     0x00 // terminator
                                 })
        .install_patch(0x48D28B, { 0x43, 0x72, 0x65, 0x64, 0x69, 0x74, 0x73, 0x00 }); // credits
}
} // namespace Textures
