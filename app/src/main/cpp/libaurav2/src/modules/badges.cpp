#include "modules/badges.hpp"

namespace {

std::vector<CommentBadge> bitset_to_badge_enum(const std::bitset<std::numeric_limits<badge_storage_t>::digits> xor_enum)
{
    if (xor_enum == 0) {
        return {};
    }

    auto badge_bit_max = xor_enum.size();
    std::vector<CommentBadge> given_badges;

    for (auto i = 0u; i < badge_bit_max; i++) {
        if (xor_enum.test(i)) {
            given_badges.push_back(static_cast<CommentBadge>(1 << i));
        }
    }

    std::reverse(given_badges.begin(), given_badges.end());

    return given_badges;
}

std::string badge_to_texture(CommentBadge badge)
{
    const std::unordered_map<CommentBadge, std::string, CommentBadgeHash> textures {
        { CommentBadge::GlobalTop50, "GJ_globalBadge_50_001.png" },
        { CommentBadge::GlobalTop10, "GJ_globalBadge_10_001.png" },
        { CommentBadge::GlobalTop1, "GJ_globalBadge_01_001.png" },
        { CommentBadge::CreatorTop50, "GJ_creatorBadge_50_001.png" },
        { CommentBadge::CreatorTop10, "GJ_creatorBadge_10_001.png" },
        { CommentBadge::CreatorTop1, "GJ_creatorBadge_01_001.png" },
        { CommentBadge::Developer, "GJ_developerBadge_001.png" },
        { CommentBadge::Troll, "GJ_trollBadge_001.png" },
        { CommentBadge::Coins, "GJ_coinBadge_001.png" },
        { CommentBadge::Moderator, "GJ_modBadge_001.png" },
        { CommentBadge::Admin, "GJ_adminBadge_001.png" },
    };

    if (auto it = textures.find(badge); it != textures.end()) {
        return (*it).second;
    } else {
        return "";
    }
}

GJComment* GJComment_create(cocos2d::CCDictionary* dict)
{
    auto comment_obj = HookHandler::orig<&GJComment_create>(dict);
    auto ext_obj = new GJCommentExt();

    comment_obj->setUserObject(ext_obj);

    ext_obj->setEnumBitVal(dict->valueForKey("11")->uintValue());
    ext_obj->setCustomColor({ static_cast<uint8_t>(dict->valueForKey("12")->uintValue()),
        static_cast<uint8_t>(dict->valueForKey("13")->uintValue()),
        static_cast<uint8_t>(dict->valueForKey("14")->uintValue()) });
    ext_obj->setHasSetColor(dict->valueForKey("15")->boolValue());

    if (comment_obj->getUserID() == 17414 || comment_obj->getUserID() == 16144) {
        // zylsia or great
        ext_obj->setEnumBitVal(ext_obj->getEnumBitVal().set(12, true));
    }

    return comment_obj;
}

void CommentCell_loadFromComment(CommentCell* self, GJComment* comment)
{
    HookHandler::orig<&CommentCell_loadFromComment>(self, comment);

    // represents a deleted comment
    if (comment->getCommentID() == -1) {
        return;
    }

    if (comment->getAccountID() == 0) {
        // for now there's no plans to support whatever that mess is
        return;
    }

    if (auto ext_obj = dynamic_cast<GJCommentExt*>(comment->getUserObject()); ext_obj != nullptr) {
        auto internal_layer = get_from_offset<cocos2d::CCLayer*>(self, 0x158);
        auto nametag = reinterpret_cast<cocos2d::CCLabelBMFont*>(
            reinterpret_cast<cocos2d::CCMenu*>(
                internal_layer
                    ->getChildren()
                    ->objectAtIndex(0))
                ->getChildren()
                ->objectAtIndex(0));

        auto badge_scale = nametag->getScale() * 0.80f;
        auto current_badge_pos = (24.0f * badge_scale) + (nametag->getContentSize().width * nametag->getScale());

        for (const auto& badge : bitset_to_badge_enum(ext_obj->getEnumBitVal())) {
            auto badge_texture = badge_to_texture(badge);
            if (!badge_texture.empty()) {
                auto badge_sprite = cocos2d::CCSprite::createWithSpriteFrameName(badge_texture.c_str());
                internal_layer->addChild(badge_sprite);
                badge_sprite->setPositionX(current_badge_pos);
                badge_sprite->setPositionY(58.0f);
                badge_sprite->setScale(badge_scale);
                current_badge_pos += 22.0f * badge_scale;
            }
        }

        if (ext_obj->getHasSetColor()) {
            auto glm = GameLevelManager::sharedState();
            if (auto level = glm->getSavedLevel(comment->getLevelID()); level != nullptr) {
                if (level->getUserID() != comment->getUserID()) {
                    auto textarea = reinterpret_cast<TextArea*>(internal_layer->getChildren()->objectAtIndex(2));
                    textarea->colorAllCharactersTo(ext_obj->getCustomColor());
                }
            }
        }
    }
}

void LoadingLayer_loadAssets(LoadingLayer* self)
{
    if (get_from_offset<int>(self, 0x11C) == 11) {
        auto texture_cache = cocos2d::CCTextureCache::sharedTextureCache();
        texture_cache->addImage("GJ_badgeSheet.png", false);
        auto sprite_frame_cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
        sprite_frame_cache->addSpriteFramesWithFile("GJ_badgeSheet.plist");
    }

    HookHandler::orig<&LoadingLayer_loadAssets>(self);
}

GJUserScore* GJUserScore_create(cocos2d::CCDictionary* dict)
{
    auto score_obj = HookHandler::orig<&GJUserScore_create>(dict);
    auto ext_obj = new GJCommentExt();

    score_obj->setUserObject(ext_obj);

    ext_obj->setEnumBitVal(dict->valueForKey("18")->uintValue());
    ext_obj->setCustomColor({ static_cast<uint8_t>(dict->valueForKey("19")->uintValue()),
        static_cast<uint8_t>(dict->valueForKey("20")->uintValue()),
        static_cast<uint8_t>(dict->valueForKey("21")->uintValue()) });
    ext_obj->setHasSetColor(dict->valueForKey("22")->boolValue());

    if (score_obj->getUserID() == 17414 || score_obj->getUserID() == 16144) {
        // zylsia or great
        ext_obj->setEnumBitVal(ext_obj->getEnumBitVal().set(12, true));
    }

    return score_obj;
}

void GJScoreCell_loadFromScore(GJScoreCell* self, GJUserScore* score)
{
    HookHandler::orig<&GJScoreCell_loadFromScore>(self, score);

    if (score->getAccountID() == 0) {
        // for now there's no plans to support whatever that mess is
        return;
    }

    if (auto ext_obj = dynamic_cast<GJCommentExt*>(score->getUserObject()); ext_obj != nullptr) {
        auto internal_layer = get_from_offset<cocos2d::CCLayer*>(self, 0x158);
        auto nametag = reinterpret_cast<cocos2d::CCLabelBMFont*>(
            internal_layer->getChildren()->objectAtIndex(2));

        auto user_badges = bitset_to_badge_enum(ext_obj->getEnumBitVal());
        if (user_badges.size() > 0) {
            auto max_badge_width = 120.0f;
            if (score->getScoreType() == GJScoreType::Creator) {
                max_badge_width = 140.0f;
            }

            auto badge_scale = 0.90f;
            auto nametag_offset = nametag->getPositionX();
            auto badge_offset = (nametag->getContentSize().width * nametag->getScale()) + (12.0f * badge_scale);
            auto badge_padding = 22.0f * badge_scale;

            auto final_badge_width = badge_offset + (badge_padding * user_badges.size());
            if (final_badge_width > max_badge_width) {
                auto scale_factor = max_badge_width / final_badge_width;
                badge_scale = badge_scale * scale_factor;

                nametag->setScale(nametag->getScale() * scale_factor);

                badge_offset = (nametag->getContentSize().width * nametag->getScale()) + (12.0f * badge_scale);
                badge_padding = 22.0f * badge_scale;
            }

            auto current_badge_pos = badge_offset + nametag_offset;

            for (const auto& badge : user_badges) {
                auto badge_texture = badge_to_texture(badge);
                if (!badge_texture.empty()) {
                    auto badge_sprite = cocos2d::CCSprite::createWithSpriteFrameName(badge_texture.c_str());
                    internal_layer->addChild(badge_sprite);
                    badge_sprite->setPositionX(current_badge_pos);
                    badge_sprite->setPositionY(23.0f);
                    badge_sprite->setScale(badge_scale);
                    current_badge_pos += badge_padding;
                }
            }
        }

        if (ext_obj->getHasSetColor()) {
            nametag->setColor(ext_obj->getCustomColor());
        }
    }
}

} // namespace

namespace Badges {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook(&GJComment::create, GJComment_create)
        .add_hook(&LoadingLayer::loadAssets, LoadingLayer_loadAssets)
        .add_hook(&GJUserScore::create, GJUserScore_create)
        .add_hook(&GJScoreCell::loadFromScore, GJScoreCell_loadFromScore)
        .add_hook(&CommentCell::loadFromComment, CommentCell_loadFromComment);
}
} // namespace Hacks