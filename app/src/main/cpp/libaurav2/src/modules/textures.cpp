#include "modules/textures.hpp"

namespace {
constexpr unsigned int SUPER_VALUE = 184594917;
constexpr auto SUPER_TEXTURE = "GJ_superCoin_001.png";

void LevelCell_loadCustomLevelCell(LevelCell* self)
{
    // oh no
    HookHandler::orig<&LevelCell_loadCustomLevelCell>(self);

    auto level = get_from_offset<GJGameLevel*>(self, 0x15C);
    spdlog::trace("load custom level cell {}", level->getLevelID());

    auto featured = level->getFeatured();

    if (featured == SUPER_VALUE) {
        spdlog::trace("supered level {}", level->getLevelName().c_str());
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
    void onDeleteSong(cocos2d::CCObject* /* target */)
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

    void onFailureInfo(cocos2d::CCObject*) {
        FLAlertLayer::create(
                nullptr,
                "Song Issues",
                "Due to <cl>recent changes</c> to the <cy>Newgrounds</c> and <co>Soundcloud</c> APIs, "
                "some songs are now <cr>unavailable</c>. "
                "Please use the <cg>Song Reupload</c> tool to <cl>reupload new songs</c>.",
                "OK", nullptr, 350.0f)->show();
    }

    void onSongList(cocos2d::CCObject*) {
        cocos2d::CCApplication::sharedApplication()->openURL("https://absolllute.com/gdps/gdapi/tools/song-list/");
    }

    void onSongReupload(cocos2d::CCObject*) {
        cocos2d::CCApplication::sharedApplication()->openURL("https://absolllute.com/gdps/gdapi/tools/song-add/");
    }
};

void CustomSongWidget_onGetSongInfo(CustomSongWidget* self, cocos2d::CCObject* target) {
    HookHandler::orig<&CustomSongWidget_onGetSongInfo>(self, target);

    auto menu = get_from_offset<cocos2d::CCMenu*>(self, 0xF4);
    auto potential_failure_btn = menu->getChildByTag(28);
    if (potential_failure_btn != nullptr) {
        potential_failure_btn->setVisible(false);
    }
}

void CustomSongWidget_onDownload(CustomSongWidget* self, cocos2d::CCObject* target) {
    HookHandler::orig<&CustomSongWidget_onDownload>(self, target);

    auto menu = get_from_offset<cocos2d::CCMenu*>(self, 0xF4);
    auto potential_failure_btn = menu->getChildByTag(28);
    if (potential_failure_btn != nullptr) {
        potential_failure_btn->setVisible(false);
    }
}

void CustomSongWidget_updateError(CustomSongWidget* self, GJSongError error) {
    HookHandler::orig<&CustomSongWidget_updateError>(self, error);

    if (error == GJSongError::NotFound || error == GJSongError::Unknown) {
        auto menu = get_from_offset<cocos2d::CCMenu*>(self, 0xF4);
        auto potential_failure_btn = menu->getChildByTag(28);
        if (potential_failure_btn == nullptr) {
            auto failure_sprite = cocos2d::CCSprite::createWithSpriteFrameName(
                    "GJ_reportBtn_001.png");
            failure_sprite->setScale(0.7f);

            auto failure_btn = CCMenuItemSpriteExtra::create(
                    failure_sprite,
                    nullptr,
                    self,
                    static_cast<cocos2d::SEL_MenuHandler>(&CustomSongWidgetExt::onFailureInfo));

            menu->addChild(failure_btn);

            auto cancel_download_btn = get_from_offset<CCMenuItemSpriteExtra*>(self, 0x10C);

            failure_btn->setTag(28);
            failure_btn->setPositionX(cancel_download_btn->getPositionX());
            failure_btn->setPositionY(cancel_download_btn->getPositionY() + 40.0f);
            failure_btn->setSizeMult(1.1f);
        } else {
            potential_failure_btn->setVisible(true);
        }
    }
}

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

bool CustomSongLayer_init(CustomSongLayer* self, LevelSettingsObject* level) {
    if (HookHandler::orig<&CustomSongLayer_init>(self, level)) {
        auto menu = get_from_offset<cocos2d::CCMenu*>(self, 0x194);

        // hide large go to newgrounds button
        auto orig_button = static_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(4));
        orig_button->setVisible(false);

        auto newgrounds_sprite = ButtonSprite::create(
                "Newgrounds",
                220, 0, 0.4f, false,
                "bigFont.fnt", "GJ_button_01.png", 25.0f);

        auto newgrounds_btn = CCMenuItemSpriteExtra::create(
                newgrounds_sprite, nullptr, self,
                static_cast<cocos2d::SEL_MenuHandler>(&CustomSongLayer::onNewgrounds));

        menu->addChild(newgrounds_btn);
        newgrounds_btn->setPositionX(orig_button->getPositionX() - 98.0f);
        newgrounds_btn->setPositionY(orig_button->getPositionY());

        auto list_sprite = ButtonSprite::create(
                "Song List",
                220, 0, 0.4f, false,
                "bigFont.fnt", "GJ_button_02.png", 25.0f);

        auto list_btn = CCMenuItemSpriteExtra::create(
                list_sprite, nullptr, self,
                static_cast<cocos2d::SEL_MenuHandler>(&CustomSongWidgetExt::onSongList));

        menu->addChild(list_btn);
        list_btn->setPositionX(orig_button->getPositionX());
        list_btn->setPositionY(orig_button->getPositionY());

        auto reupload_sprite = ButtonSprite::create(
                "Reupload",
                220, 0, 0.4f, false,
                "bigFont.fnt", "GJ_button_05.png", 25.0f);

        auto reupload_btn = CCMenuItemSpriteExtra::create(
                reupload_sprite, nullptr, self,
                static_cast<cocos2d::SEL_MenuHandler>(&CustomSongWidgetExt::onSongReupload));

        menu->addChild(reupload_btn);
        reupload_btn->setPositionX(orig_button->getPositionX() + 90.0f);
        reupload_btn->setPositionY(orig_button->getPositionY());

        auto failure_sprite = cocos2d::CCSprite::createWithSpriteFrameName(
                "GJ_reportBtn_001.png");
        failure_sprite->setScale(0.7f);

        auto failure_btn = CCMenuItemSpriteExtra::create(
                failure_sprite,
                nullptr,
                self,
                static_cast<cocos2d::SEL_MenuHandler>(&CustomSongWidgetExt::onFailureInfo));

        menu->addChild(failure_btn);
        failure_btn->setPositionX(orig_button->getPositionX() - 175.0f);
        failure_btn->setPositionY(orig_button->getPositionY() + 50.0f);

        return true;
    }

    return false;
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

cocos2d::CCArray* GJMoreGamesLayer_getMoreGamesList(GJMoreGamesLayer* /* self */)
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
                textbox->setString("By sharing a level you give the rights to use the level (in any way) and your first born child to the one true deity, zmx.");
            } else {
                textbox->setString("By sharing a level you give the rights to use the level (in any way) to the creators of the 1.9 GDPS.");
            }
        }

        return true;
    }

    return false;
}

void CCDirector_updateContentScale(cocos2d::CCDirector* self, cocos2d::TextureQuality n_quality) {
    spdlog::info("updcontentscale: {}", static_cast<int>(n_quality));

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
        .add_hook(&LoadingLayer::loadAssets, LoadingLayer_loadAssets)
        .add_hook("_ZN15CustomSongLayer4initEP19LevelSettingsObject", &CustomSongLayer_init)
        .add_hook(&CustomSongWidget::onGetSongInfo, &CustomSongWidget_onGetSongInfo)
        .add_hook(&CustomSongWidget::onDownload, &CustomSongWidget_onDownload)
        .add_hook(&CustomSongWidget::updateError, &CustomSongWidget_updateError)
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
