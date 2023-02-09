#include "modules/share.hpp"

namespace
{
    // thanks mat
    std::string base64_encode(const std::string& str) {
        char* out = nullptr;
        cocos2d::base64Encode(
                reinterpret_cast<unsigned char *>(const_cast<char*>(str.data())),
                str.size(), &out, false);
        std::string outs(out);
        free(out);
        return outs;
    }

    class EditLevelLayer_ext : public EditLevelLayer
    {
    public:
        void onExport(cocos2d::CCObject *)
        {
            auto level = get_from_offset<GJGameLevel *>(this, 0x124);

            auto level_string = ptr_to_offset<std::string>(level, 0xF8);

            if (!level_string->empty())
            {
                auto export_string = this->createLevelExportString();

                auto output_path = JNI::get_save_directory() + "temp.gmd";
                auto out = std::ofstream(output_path, std::ofstream::out);

                out << export_string;

                out.close();

                JNI::export_level(level->getLevelName());
            } else {
                FLAlertLayer::create(nullptr, "Empty", "You can't export an empty level.", "OK", nullptr, 300.0f)->show();
            }
        }

    private:
        std::string createLevelExportString()
        {
            auto level = get_from_offset<GJGameLevel *>(this, 0x124);
            spdlog::get("global")->info("exporting level `{}`", level->getLevelName());

            // port of gdshare::exportLevel function at HJfod/GDShare-mod@38f00df3d1af115fb2ddca30b02d6acd12f89661/src/utils/gdshare.cpp#L150
            auto song = std::string();

            if (level->getSongID() != 0)
            {
                std::stringstream song_stream;
                song_stream << "<k>k45</k><i>" << level->getSongID() << "</i>";
                song = song_stream.str();
            }
            else
            {
                std::stringstream song_stream;
                song_stream << "<k>k8</k><i>" << level->getAudioTrack() << "</i>";
                song = song_stream.str();
            }

            // i love stringstreams
            // and also strings in general

            // gdshare double encodes description strings, and we have to replicate this
            auto desc = base64_encode(
                    base64_encode(
                            ptr_to_offset<std::string>(level, 0xF4)->c_str()));
            spdlog::get("global")->info("desc `{}`", desc);

//            auto level_string = ptr_to_offset<std::string>(level, 0xF8)->c_str();
//            spdlog::get("global")->info("level_string `{}`", level_string);

            std::stringstream data_stream;
            data_stream << "<d>";
            data_stream << "<k>kCEK</k>";
            data_stream << "<i>4</i>";
            data_stream << "<k>k2</k>";
            data_stream << "<s>" << level->getLevelName() << "</s>";
            data_stream << "<k>k3</k>";
            data_stream << "<s>" << desc << "</s>";
            data_stream << "<k>k4</k>";
            data_stream << "<s>" <<  level->getLevelString() << "</s>";
            data_stream << song;
            data_stream << "<k>k13</k>";
            data_stream << "<t/>";
            data_stream << "<k>k21</k>";
            data_stream << "<i>2</i>";
            data_stream << "<k>k50</k>";
            data_stream << "<i>24</i>";
            data_stream << "</d>";

            return data_stream.str();
        }
    };

    bool EditLevelLayer_init(EditLevelLayer *self, GJGameLevel *lvl)
    {
        if (HookHandler::orig<&EditLevelLayer_init>(self, lvl))
        {
            auto share_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");

            auto share_btn = CCMenuItemSpriteExtra::create(
                    share_sprite, nullptr, self,
                    static_cast<cocos2d::SEL_MenuHandler>(&EditLevelLayer_ext::onExport));

            auto menu = cocos2d::CCMenu::createWithItem(share_btn);
            self->addChild(menu, 1);

            auto director = cocos2d::CCDirector::sharedDirector();

            auto pos_x = director->getScreenRight() - 23.0f;
            auto pos_y = director->getScreenBottom() + 23.0f;

            menu->setPosition(pos_x, pos_y);

            return true;
        }

        return false;
    }

    class LevelBrowserLayer_ext : public LevelBrowserLayer {
    public:
        void onImport(cocos2d::CCObject*) {
            JNI::import_level();
        }
    };

    bool LevelBrowserLayer_init(LevelBrowserLayer* self, GJSearchObject* search) {
        if (HookHandler::orig<&LevelBrowserLayer_init>(self, search)) {
            if (search->getType() == SearchType::Created) {
                auto import_sprite = cocos2d::CCSprite::createWithSpriteFrameName(
                        "GJ_downloadBtn_001.png");

                auto import_button = CCMenuItemSpriteExtra::create(
                        import_sprite,
                        nullptr,
                        self,
                        static_cast<cocos2d::SEL_MenuHandler>(&LevelBrowserLayer_ext::onImport));

                auto button_menu = reinterpret_cast<cocos2d::CCNode*>(
                        self->getChildren()->objectAtIndex(9));

                if (dynamic_cast<TextArea*>(button_menu) != nullptr) {
                    // tap new offsets the amount of children by 1
                    button_menu = reinterpret_cast<cocos2d::CCNode*>(
                            self->getChildren()->objectAtIndex(10));
                }

                button_menu->addChild(import_button);
                import_button->setPositionY(50.0f);
            }

            return true;
        }

        return false;
    }
}

namespace Share
{
    void Module::on_initialize()
    {
        HookHandler::get_handler()
                .add_hook("_ZN14EditLevelLayer4initEP11GJGameLevel", &EditLevelLayer_init)
                .add_hook(&LevelBrowserLayer::init, &LevelBrowserLayer_init);
    }
} // namespace Share