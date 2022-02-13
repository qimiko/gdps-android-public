#include "modules/search.hpp"

namespace {
class GJSearchObjectExt : public cocos2d::CCNode {
private:
    bool super_;

public:
    bool getSuper() const { return this->super_; };
    void setSuper(bool n_super) { this->super_ = n_super; }

    CREATE_FUNC(GJSearchObjectExt)
};

class MoreSearchLayerExt : public cocos2d::CCLayer {
public:
    void onNoReupload(cocos2d::CCObject* target)
    {
        auto glm = GameLevelManager::sharedState();

        auto no_reupload = glm->getBoolForKey("noreupload_filter");
        glm->setBoolForKey(!no_reupload, "noreupload_filter");
    }

    void onSuper(cocos2d::CCObject* target)
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
        self->createToggleButton(
            "Super",
            static_cast<cocos2d::SEL_MenuHandler>(&MoreSearchLayerExt::onSuper),
            !filter_supered_toggled, toggle_menu, cocos2d::CCPoint(145.0f, 83.0f));
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
    auto search_object = HookHandler::orig<&LevelSearchLayer_getSearchObject>(self, type, query);

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

    HookHandler::orig<&GameLevelManager_ProcessHttpRequest>(self, url, data, key, http_type);
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
        .add_hook(&GJSearchObject::getPrevPageObject, GJSearchObject_getPrevPageObject);
}
} // namespace Search