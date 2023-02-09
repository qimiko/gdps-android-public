#include "modules/search.hpp"

namespace {

class MoreSearchLayerExt : public cocos2d::CCLayer {
public:
    void onNoReupload(cocos2d::CCObject* /* target */)
    {
        auto glm = GameLevelManager::sharedState();

        auto no_reupload = glm->getBoolForKey("noreupload_filter");
        glm->setBoolForKey(!no_reupload, "noreupload_filter");
    }

    void onSuper(cocos2d::CCObject* /* target */)
    {
        auto glm = GameLevelManager::sharedState();

        auto super_filter = glm->getBoolForKey("super_filter");
        glm->setBoolForKey(!super_filter, "super_filter");
    }
};

bool MoreSearchLayer_init(MoreSearchLayer* self)
{
    auto result = HookHandler::orig<&MoreSearchLayer_init>(self);
    if (result) {
        auto toggle_menu = get_from_offset<cocos2d::CCMenu*>(self, 0x194);

        auto glm = GameLevelManager::sharedState();
        auto filter_noreupload_toggled = glm->getBoolForKey("noreupload_filter");
        auto filter_supered_toggled = glm->getBoolForKey("super_filter");
        /*
self->createToggleButton(
            "No Reupload",
            static_cast<cocos2d::SEL_MenuHandler>(&MoreSearchLayerExt::onNoReupload),
            !filter_noreupload_toggled,
            toggle_menu,
            cocos2d::CCPoint(125.0f, 80.0f));
*/

        auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
        float x = winSize.width * 0.5f - 140.0f;
        float y = winSize.height * 0.5f + 70.0f;

        self->createToggleButton(
            "Super",
            static_cast<cocos2d::SEL_MenuHandler>(&MoreSearchLayerExt::onSuper),
            !filter_supered_toggled, toggle_menu, cocos2d::CCPoint(x, y - (50.0f * 3)));
    }

    std::vector<int> song_objects_offsets = { 0x1DC, 0x1E0, 0x1D8 };

    for (const auto& song_objects_offset : song_objects_offsets) {
        auto song_objects = get_from_offset<cocos2d::CCArray*>(self, song_objects_offset);
        for (int i = 0; i < song_objects->count(); i++) {
            auto song_object = reinterpret_cast<cocos2d::CCNode*>(song_objects->objectAtIndex(i));

            auto obj_position = song_object->getPosition();
            obj_position.x += 75.0f;

            song_object->setPosition(obj_position);
        }
    }

    return result;
}

void LevelSearchLayer_clearFilters(LevelSearchLayer* self)
{
    HookHandler::orig<&LevelSearchLayer_clearFilters>(self);

    auto glm = GameLevelManager::sharedState();
    glm->setBoolForKey(false, "noreupload_filter");
    glm->setBoolForKey(false, "super_filter");

    return;
}

const char* GJSearchObject_getKey(GJSearchObject* self)
{
    auto search_key = HookHandler::orig<&GJSearchObject_getKey>(self);

    auto ext_obj = dynamic_cast<GJSearchObjectExt*>(self->getUserObject());
    if (ext_obj != nullptr) {
        if (ext_obj->getSuper()) {
            std::string search_key_str(search_key);
            search_key_str += "_1";

            search_key = search_key_str.c_str();
        }
    }
    return search_key;
}

const char* GJSearchObject_getNextPageKey(GJSearchObject* self)
{
    auto search_key = HookHandler::orig<&GJSearchObject_getNextPageKey>(self);

    auto ext_obj = dynamic_cast<GJSearchObjectExt*>(self->getUserObject());
    if (ext_obj != nullptr) {
        if (ext_obj->getSuper()) {
            std::string search_key_str(search_key);
            search_key_str += "_1";

            search_key = search_key_str.c_str();
        }
    }
    return search_key;
}

GJSearchObject* GJSearchObject_getNextPageObject(GJSearchObject* self)
{
    auto next_page_obj = HookHandler::orig<&GJSearchObject_getNextPageObject>(self);
    auto ext_obj = dynamic_cast<GJSearchObjectExt*>(self->getUserObject());
    if (ext_obj != nullptr) {
        next_page_obj->setUserObject(ext_obj);
    }

    return next_page_obj;
}

GJSearchObject* GJSearchObject_getPrevPageObject(GJSearchObject* self)
{
    auto prev_page_obj = HookHandler::orig<&GJSearchObject_getPrevPageObject>(self);
    auto ext_obj = dynamic_cast<GJSearchObjectExt*>(self->getUserObject());
    if (ext_obj != nullptr) {
        prev_page_obj->setUserObject(ext_obj);
    }

    return prev_page_obj;
}

GJSearchObject* LevelSearchLayer_getSearchObject(LevelSearchLayer* self,
    SearchType type,
    std::string query)
{
    auto search_object = HookHandler::orig<&LevelSearchLayer_getSearchObject>(self, type, query); // NOLINT(performance-unnecessary-value-param)

    auto ext_object = GJSearchObjectExt::create();

    auto glm = GameLevelManager::sharedState();
    auto filter_supered_toggled = glm->getBoolForKey("super_filter");
    ext_object->setSuper(filter_supered_toggled);

    search_object->setUserObject(ext_object);

    return search_object;
}

void GameLevelManager_getOnlineLevels(GameLevelManager* self,
    GJSearchObject* search_obj)
{
    auto ext_obj = dynamic_cast<GJSearchObjectExt*>(search_obj->getUserObject());
    if (ext_obj != nullptr) {
        self->setUserObject(ext_obj);
    }

    HookHandler::orig<&GameLevelManager_getOnlineLevels>(self, search_obj);
}

void GameLevelManager_ProcessHttpRequest(GameLevelManager* self,
    std::string url, std::string data,
    std::string key,
    GJHttpType http_type)
{
    auto ext_obj = dynamic_cast<GJSearchObjectExt*>(self->getUserObject());
    if (ext_obj != nullptr) {
        // object is here (searching levels)
        if (ext_obj->getSuper()) {
            data += "&epic=1";
        }

        // clear the object so it isn't left lingering between searches
        self->setUserObject(nullptr);
    }

    HookHandler::orig<&GameLevelManager_ProcessHttpRequest>(self, url, data, key, http_type); // NOLINT(performance-unnecessary-value-param)
}

class LevelBrowserLayer_ext : public LevelBrowserLayer {
public:
    void onHallOfFame(cocos2d::CCObject*) {
        // all this mess is to get it to return to the previous search object
        // (featured)
        GameManager::sharedState()->setLastScene2(LastGameScene::PreviousSearch);

        auto current_search = get_from_offset<GJSearchObject*>(this, 0x134);

        GameLevelManager::sharedState()->setLastSearchKey2(current_search->getKey());

        auto hof_search = GJSearchObject::create(SearchType::HallOfFame);
        auto browser_scene = LevelBrowserLayer::scene(hof_search);

        auto fade_scene = cocos2d::CCTransitionFade::create(0.5f, browser_scene);
        cocos2d::CCDirector::sharedDirector()->replaceScene(fade_scene);
    }
};

bool LevelBrowserLayer_init(LevelBrowserLayer* self, GJSearchObject* search) {
    if (HookHandler::orig<&LevelBrowserLayer_init>(self, search)) {
        if (search->getType() == SearchType::Featured) {
            auto hof_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png");

            auto hof_button = CCMenuItemSpriteExtra::create(
                    hof_sprite,
                    nullptr,
                    self,
                    static_cast<cocos2d::SEL_MenuHandler>(&LevelBrowserLayer_ext::onHallOfFame));

            auto hof_menu = cocos2d::CCMenu::createWithItem(hof_button);
            self->addChild(hof_menu, 1);

            auto director = cocos2d::CCDirector::sharedDirector();

            auto pos_x = director->getScreenRight() - 30.0f;
            auto pos_y = director->getScreenBottom() + 30.0f;

            hof_menu->setPosition(pos_x, pos_y);
        }

        return true;
    }

    return false;
}

} // namespace

namespace Search {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook("_ZN15MoreSearchLayer4initEv", MoreSearchLayer_init)
        .add_hook(&LevelSearchLayer::clearFilters, LevelSearchLayer_clearFilters)
        .add_hook(&LevelSearchLayer::getSearchObject, LevelSearchLayer_getSearchObject)
        .add_hook(&GameLevelManager::ProcessHttpRequest, GameLevelManager_ProcessHttpRequest)
        .add_hook(&GameLevelManager::getOnlineLevels, GameLevelManager_getOnlineLevels)
        .add_hook(&GJSearchObject::getKey, GJSearchObject_getKey)
        .add_hook(&GJSearchObject::getNextPageKey, GJSearchObject_getNextPageKey)
        .add_hook(&GJSearchObject::getNextPageObject, GJSearchObject_getNextPageObject)
        .add_hook(&GJSearchObject::getPrevPageObject, GJSearchObject_getPrevPageObject)
        .add_hook(&LevelBrowserLayer::init, &LevelBrowserLayer_init);
}
} // namespace Search