#include "classes/managers/outfitmanager.hpp"

OutfitManager& OutfitManager::get_manager() {
    static OutfitManager _manager_instance;
    return _manager_instance;
}

cocos2d::ccColor3B OutfitManager::get_color(int idx) {
    if (_outfits == nullptr) {
        return { 0xFF, 0xFF, 0xFF };
    }

    auto colors = reinterpret_cast<cocos2d::CCArray*>(_outfits->objectForKey("aPlayerColors"));
    auto colorStr = reinterpret_cast<cocos2d::CCString*>(colors->objectAtIndex(idx));

    if (colorStr == nullptr) {
        // indicate the error type
        return { 0x00, 0x00, 0x00 };
    }

    auto colorInt = colorStr->intValue();

    // this should always fit in byte size regardless
    return {
            static_cast<GLubyte>(((colorInt >> 16) & 0xFF)),
            static_cast<GLubyte>(((colorInt >> 8) & 0xFF)),
            static_cast<GLubyte>((colorInt & 0xFF))
    };
}

void OutfitManager::init() {
    _outfits = cocos2d::CCDictionary::createWithContentsOfFile("playerOutfits.plist");
    if (_outfits != nullptr) {
        auto colors = reinterpret_cast<cocos2d::CCArray*>(_outfits->objectForKey("aPlayerColors"));
        spdlog::get("global")->info("colors loaded: {}", colors->count());
        _outfits->retain();

        install_patches();
    }
}

void OutfitManager::install_patches() {
    std::array<std::pair<const char* /* key */, std::vector<uintptr_t> /* patches */>, 4> count_patches {
            {
                    { "iPlayerMaxId", { 0x209732, 0x20A176, 0x20A17C, 0x1CCA60, 0x1CCA66, 0x1CA6AA, 0x1CA6A4, 0x20A7AC } }, // player count
                    { "iBallMaxId", { 0x209772, 0x20A7B4, 0x1CA9A4, 0x1CA9AA } }, // ball
                    { "iShipMaxId", { 0x1CA78C, 0x1CA792, 0x209752, 0x20A7B0 } }, // ship
                    { "iSpecialMaxId", { 0x2097AE, 0x1CB372, 0x1CB374 } } // special
            }
    };

    for (const auto& patch : count_patches) {
        for (const auto& address : patch.second) {
            auto maxStr = _outfits->valueForKey(patch.first);
            if (maxStr != nullptr) {
                auto maxVal = std::max(maxStr->intValue(), 1);
                HookHandler::get_handler().install_patch(address,{ static_cast<unsigned char>(maxVal) });
            }
        }
    }

    auto maxColorStr = _outfits->valueForKey("iColorMaxDispId");
    if (maxColorStr != nullptr) {
        auto maxVal = maxColorStr->intValue() + 1;

        // will have to find where these go
        auto maxLastPage = (maxVal + 1) % 12;

        HookHandler::get_handler()
                .install_patch(0x208B82,{ 0xB9, 0xF1, static_cast<unsigned char>(maxVal), 0x0F });
    }
}

OutfitManager::~OutfitManager() {
    _outfits->release();
}

OutfitManager::OutfitManager() {
    init();
};