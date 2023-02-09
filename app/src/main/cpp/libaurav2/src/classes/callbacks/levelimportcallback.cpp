#include "classes/callbacks/levelimportcallback.hpp"

namespace {
    // stole this from https://stackoverflow.com/a/60629336/10498578
    bool check_utf_bytes(const char* bytes) {
        while (*bytes != '\0') {
            auto utf8 = reinterpret_cast<const uint8_t*>(bytes++);
            // Switch on the high four bits.
            switch (*utf8 >> 4) {
                case 0x00:
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                    // Bit pattern 0xxx. No need for any extra bytes.
                    break;
                case 0x08:
                case 0x09:
                case 0x0a:
                case 0x0b:
                    // Bit patterns 10xx, which are illegal start bytes.
                    return false;
                case 0x0f:
                    // Bit pattern 1111, which might be the start of a 4 byte sequence.
                    if ((*utf8 & 0x08) == 0) {
                        // Bit pattern 1111 0xxx, which is the start of a 4 byte sequence.
                        // We consume one continuation byte here, and fall through to consume two more.
                        utf8 = reinterpret_cast<const uint8_t*>(bytes++);
                        if ((*utf8 & 0xc0) != 0x80) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                    // Fall through to the cases below to consume two more continuation bytes.
                case 0x0e:
                    // Bit pattern 1110, so there are two additional bytes.
                    utf8 = reinterpret_cast<const uint8_t*>(bytes++);
                    if ((*utf8 & 0xc0) != 0x80) {
                        return false;
                    }
                    // Fall through to consume one more continuation byte.
                case 0x0c:
                case 0x0d:
                    // Bit pattern 110x, so there is one additional byte.
                    utf8 = reinterpret_cast<const uint8_t*>(bytes++);
                    if ((*utf8 & 0xc0) != 0x80) {
                        return false;
                    }
                    break;
            }
        }
        return true;
    }

    std::string base64_decode(const std::string& str) {
        if (str.empty()) {
            return str;
        }

        unsigned char* out = nullptr;
        const auto size = cocos2d::base64Decode(
                reinterpret_cast<unsigned char *>(const_cast<char*>(str.data())),
                str.size(), &out);
        std::string outs(reinterpret_cast<char*>(out), size);
        free(out);

        if (!check_utf_bytes(outs.c_str())) {
            return str;
        }

        return outs;
    }
}

void LevelImportCallback::goToScene() {
    if (toLevel_ == nullptr) {
        return;
    }

    auto editor_scene = EditLevelLayer::scene(reinterpret_cast<GJGameLevel*>(toLevel_));
    auto fade_scene = cocos2d::CCTransitionFade::create(0.5f, editor_scene);

    if (this->isNewLevel_) {
        auto warning_popup = FLAlertLayer::create(
                nullptr, "Info",
                "This level was created on a <cy>newer version</c> of the game.\n<cr>Some issues may occur during gameplay.</c>",
                "OK", nullptr);
        warning_popup->setTargetScene(editor_scene);
        warning_popup->show();
    }

    cocos2d::CCDirector::sharedDirector()->replaceScene(fade_scene);
}

void LevelImportCallback::onLevelImported(cocos2d::CCNode*) {
    goToScene();
}

void LevelImportCallback::FLAlert_Clicked(FLAlertLayer*, bool) {
    goToScene();
}

extern "C"
JNIEXPORT void JNICALL Java_com_kyurime_geometryjump_ModGlue_onLevelImported(
        JNIEnv *env,
        jclass clazz,
        jstring path) {
    auto path_str = cocos2d::JniHelper::jstring2string(path);
    spdlog::get("global")->info("path `{}`", path_str);

    auto doc = tinyxml2::XMLDocument();
    if (doc.LoadFile(path_str.c_str()) != tinyxml2::XML_SUCCESS) {
        FLAlertLayer::create(
                nullptr,
                "Import Failed",
                "A valid file was not provided for import.",
                "OK", nullptr)->show();
        return;
    }

    // plist parsing, tinyxml style
    // there's no validation on this at all. enjoy

    auto level = GameLevelManager::sharedState()->createNewLevel();
    GameLevelManager::sharedState()->setCreatedNewLevel(true);

    auto is_new_level = false;

    auto document = doc.FirstChildElement("d");
    for (auto elem = document->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
        if (strcmp(elem->Value(), "k") == 0) {
            auto key = elem->GetText();

            elem = elem->NextSiblingElement();
            if (elem == nullptr) {
                break;
            }

            auto value = elem->GetText();
            if (value == nullptr) {
                continue;
            }

            // what nice parsing we have
            if (strcmp(key, "kCEK") == 0) {
                auto obj_type = atoi(value);
                if (obj_type != 4) {
                    FLAlertLayer::create(
                            nullptr,
                            "Import Failed",
                            "A valid level was not provided for import.",
                            "OK", nullptr)->show();
                    return;
                }
            } else if (strcmp(key, "k2") == 0) {
                level->setLevelName(value);
            } else if (strcmp(key, "k3") == 0) {
                // undo the double encode
                auto desc = base64_decode(base64_decode(value));
                level->setLevelDesc(desc);
            } else if (strcmp(key, "k4") == 0) {
                level->setLevelString(value);
            } else if (strcmp(key, "k8") == 0) {
                level->setAudioTrack(atoi(value));
            } else if (strcmp(key, "k45") == 0) {
                level->setSongID(atoi(value));
            } else if (strcmp(key, "k50") == 0) {
                if (atoi(value) > 24) {
                    is_new_level = true;
                }
            }
        }
    }

    level->setLevelType(GJLevelType::Editor);

    auto callback = LevelImportCallback::create();

    callback->setIsNewLevel(is_new_level);
    callback->setToLevel(level);

    spdlog::get("global")->info("beginning import callback");

    auto cc_func = cocos2d::CCCallFuncN::create(callback,
                                                static_cast<cocos2d::SEL_CallFuncN>(&LevelImportCallback::onLevelImported)
    );

    // this avoids a very yummy crash
    auto cc_sequence = cocos2d::CCSequence::createWithTwoActions(
            cocos2d::CCDelayTime::create(0.0f),
            cc_func);

    auto action_manager = cocos2d::CCDirector::sharedDirector()->getActionManager();
    action_manager->addAction(cc_sequence, callback, false);

    std::remove(path_str.c_str());
}
