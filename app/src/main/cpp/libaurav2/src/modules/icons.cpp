#include "modules/icons.hpp"

namespace {
    cocos2d::ccColor3B GameManager_colorForIdx(GameManager*, int idx)
    {
        auto color = OutfitManager::get_manager().get_color(idx);
        return color;
    }
}

namespace Icons {
    void Module::on_initialize()
    {
        HookHandler::get_handler()
            .add_hook("_ZN11GameManager11colorForIdxEi", &GameManager_colorForIdx);
    }
} // namespace Urls