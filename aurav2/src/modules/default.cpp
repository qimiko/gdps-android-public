#include "modules/default.hpp"

namespace {
constexpr auto OFFICIAL_GD_NAME = "com.robtopx.geometryjump";
bool IS_GD_INSTALLED = false;
int LAST_ERRNO_CODE = 0;

bool is_package_installed(const std::string& package_name)
{
    const std::string package_prefix = "/data/data/";
    std::string path_str = package_prefix + package_name + "/";

    // the cos8o method, access denied vs. not found error
    std::ifstream f;
    f.open(path_str);

    spdlog::get("global")->info("err on accessing {}: {} ({})", path_str, strerror(errno), errno);

    LAST_ERRNO_CODE = errno;
    switch (errno) {
    case 0: // success
    case EACCES: // access denied
        return true;
    default:
    case ENOENT: // no such file/directory
        return false;
    }
}

std::string get_log_path()
{
    return std::string(Config::DATA_PATH) + "logs/";
}

std::string get_last_crash_name()
{
    auto largest_mtime = 0u;

    auto last_filename = std::string();
    auto directory = opendir(get_log_path().c_str());

    if (directory != nullptr) {
        while (auto ep = readdir(directory)) {
            // strip .. and .

            if (ep->d_name[0] == '.') {
                continue;
            }

            struct stat file_stat = {};
            stat((get_log_path() + ep->d_name).c_str(), &file_stat); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

            if (file_stat.st_mtim.tv_sec > largest_mtime) {
                last_filename = ep->d_name; // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                largest_mtime = file_stat.st_mtim.tv_sec;
            }
        }

        closedir(directory);
    }

    spdlog::get("global")->info("last crash file: {}", last_filename);

    return last_filename;
}

class CrashHelperPopup : public FLAlertLayer {
protected:
    bool init() override
    {
        if (this->initWithColor({ 0, 0, 0, 75 })) {
            constexpr auto width = 360.0f;
            constexpr auto height = 225.0f;

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

            this->internalLayer_->addChild(bg);

            this->internalMenu_ = cocos2d::CCMenu::create();
            this->internalLayer_->addChild(this->internalMenu_);

            auto close_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
            auto close_btn = CCMenuItemSpriteExtra::create(
                close_sprite,
                nullptr,
                this,
                reinterpret_cast<cocos2d::SEL_MenuHandler>(&CrashHelperPopup::onClose));

            this->internalMenu_->addChild(close_btn);
            close_btn->setPosition(-((width / 2) - 5), (height / 2) - 5);

            auto delete_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
            auto delete_btn = CCMenuItemSpriteExtra::create(
                delete_sprite,
                nullptr,
                this,
                reinterpret_cast<cocos2d::SEL_MenuHandler>(&CrashHelperPopup::onDelete));

            this->internalMenu_->addChild(delete_btn);
            delete_btn->setPosition(-45.0f, -(height / 2));

            auto upload_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
            upload_sprite->setScale(0.60f);
            auto upload_btn = CCMenuItemSpriteExtra::create(
                upload_sprite,
                nullptr,
                this,
                reinterpret_cast<cocos2d::SEL_MenuHandler>(&CrashHelperPopup::onUpload));

            if (!PlatformToolbox::isNetworkAvailable() || !Config::CRASH_ENABLED) {
                upload_btn->setColor({ 0xA6, 0xA6, 0xA6 });
            }

            this->internalMenu_->addChild(upload_btn);
            upload_btn->setPosition(45.0f, -(height / 2));

            auto title = cocos2d::CCLabelBMFont::create("Crash Reporter", "bigFont.fnt");
            this->internalLayer_->addChild(title);

            title->setPosition(window_size.width / 2, (window_size.height / 2) + (height / 2) - 25.0f);
            title->setScale(0.75f);

            auto frown_face = cocos2d::CCLabelBMFont::create(":(", "bigFont.fnt");
            this->internalLayer_->addChild(frown_face);

            frown_face->setPosition(window_size.width / 2, (window_size.height / 2) + 40.0f);
            frown_face->setRotation(90.0f);
            frown_face->setScale(1.5f);

            auto info_text = cocos2d::CCLabelBMFont::create("Geometry Dash has crashed on a previous run.\nWould you like to delete or report the crash log?", "chatFont.fnt");
            this->internalLayer_->addChild(info_text);

            info_text->setScale(0.95f);
            info_text->setPosition(window_size.width / 2, (window_size.height / 2) - 5.0f);
            info_text->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);

            auto disclaimer_text = cocos2d::CCLabelBMFont::create("Reports will only be used for the purpose of bugfixing.\nNo identifying info will be stored on the servers.", "chatFont.fnt");
            this->internalLayer_->addChild(disclaimer_text);

            disclaimer_text->setScale(0.75f);
            disclaimer_text->setPosition(window_size.width / 2, (window_size.height / 2) - (height / 2) + 55.0f);
            disclaimer_text->setColor({ 0xDD, 0xDD, 0xDD });
            disclaimer_text->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);

            return true;
        }

        return false;
    }

    void keyBackClicked() override
    {
        this->onClose(nullptr);
    };

    void onClose(cocos2d::CCObject*)
    {
        if (this->inUpload_) {
            return;
        }

        if (this->loadingCircle_ != nullptr) {
            this->loadingCircle_->removeFromParentAndCleanup(true);
            this->loadingCircle_ = nullptr;
        }

        if (this->crashBtn_ != nullptr) {
            if (get_last_crash_name().empty()) {
                this->crashBtn_->setVisible(false);
            }
        }

        this->setKeyboardEnabled(false);
        this->removeFromParentAndCleanup(true);
    }

    void onDelete(cocos2d::CCObject*)
    {
        if (this->inUpload_) {
            return;
        }

        auto last_crash_file = get_last_crash_name();
        if (!last_crash_file.empty()) {
            remove((get_log_path() + last_crash_file).c_str());
        }

        this->onClose(nullptr);
    }

    void onUpload(cocos2d::CCObject*)
    {
        if (this->inUpload_) {
            return;
        }
        this->inUpload_ = true;

        if constexpr (!Config::CRASH_ENABLED) {
            FLAlertLayer::create(
                nullptr,
                "Upload Disabled",
                "Your build of the game <cr>does not</c> have a <cl>crash upload endpoint</c> configured.",
                "OK",
                nullptr,
                300.0f)
                ->show();

            this->inUpload_ = false;
            return;
        }

        if (!PlatformToolbox::isNetworkAvailable()) {
            FLAlertLayer::create(
                nullptr,
                "Game Offline",
                "An <cl>internet connection</c> is <cr>required</c> to upload crash reports.",
                "OK",
                nullptr,
                325.0f)->show();

            this->inUpload_ = false;
            return;
        }

        spdlog::get("global")->info("uploading crash log");

        auto last_crash_file = get_last_crash_name();
        auto crash_path = (get_log_path() + last_crash_file).c_str();
        if (!last_crash_file.empty())
        {
            // init loading circle
            this->loadingCircle_ = LoadingCircle::create();
            this->loadingCircle_->setTargetScene(this->internalLayer_);
            this->loadingCircle_->setDarkOverlay(true);

            // read file into buffer
            auto log_file = std::ifstream(crash_path, std::ios::binary | std::ios::in);
            auto file_contents = std::stringstream();
            file_contents << log_file.rdbuf();

            // get file size since it's annoying with ifstreams
            struct stat sb = {};
            stat(crash_path, &sb);

            // create req
            auto http_client = cocos2d::extension::CCHttpClient::getInstance();
            auto http_request = new cocos2d::extension::CCHttpRequest();

            spdlog::get("global")->info("begin sending request");

            auto url_data = cocos2d::CCString::createWithFormat(
                Config::CRASH_UPLOAD_URL,
                CMakeConfiguration::BRANCH,
                CMakeConfiguration::HASH);
            auto url_string = url_data->getCString();

            http_request->setUrl(url_string);
            http_request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
            http_request->setResponseCallback(
                this, static_cast<cocos2d::extension::SEL_HttpResponse>(&CrashHelperPopup::onUploadCrashFinished));
            http_request->setRequestData(file_contents.str().c_str(), sb.st_size);
            http_request->setHeaders({"Content-Type: text/plain"});

            // send req and show circle
            http_client->send(http_request);
            http_request->release();

            this->loadingCircle_->retain();
            this->loadingCircle_->show();
        }
    }

    void onUploadCrashFinished(
        cocos2d::extension::CCHttpClient* /* client */,
        cocos2d::extension::CCHttpResponse* response)
    {
        if (this->loadingCircle_ != nullptr) {
            this->loadingCircle_->removeFromParentAndCleanup(true);
            this->loadingCircle_ = nullptr;
        }
        this->inUpload_ = false;

        spdlog::get("global")->info("request upload finished");

        if (!response->isSucceed()) {
            FLAlertLayer::create(nullptr, "Upload failed", "Uploading crash report <cr>failed</c>. Try again later.", "OK", nullptr)->show();
            return;
        }

        auto response_data = response->getResponseData();
        std::string response_string(response_data->begin(), response_data->end());

        rapidjson::Document resp_document;
        if (resp_document.Parse(response_string.c_str()).HasParseError()) {
            FLAlertLayer::create(nullptr, "Response failed", "Reading upload response <cr>failed</c>. This might mean upload was <co>not successful</c>.", "OK", nullptr)->show();
            return;
        }

        if (resp_document["success"].GetBool()) {
            FLAlertLayer::create(nullptr, "Upload success", resp_document["message"].GetString(), "OK", nullptr)->show();

            // delete crash report cause we don't need it anymore
            this->onDelete(nullptr);
        } else {
            FLAlertLayer::create(nullptr, "Upload failure", resp_document["message"].GetString(), "OK", nullptr)->show();
        }
    }

public:
    CrashHelperPopup() : crashBtn_(nullptr), loadingCircle_(nullptr), inUpload_(false) {}

    CC_SYNTHESIZE(cocos2d::CCNode *, crashBtn_, CrashBtn);
    CC_SYNTHESIZE(LoadingCircle*, loadingCircle_, LoadingCircle);
    CC_SYNTHESIZE(bool, inUpload_, InUpload);
    CREATE_FUNC(CrashHelperPopup); // NOLINT(modernize-use-auto)
};

class MenuLayer_H : public MenuLayer {
private:
    void googlePlaySignedIn() override {};
    void FLAlert_Clicked(FLAlertLayer*, bool) override {};

public:
    void onCrash(cocos2d::CCObject* btn)
    {
        auto last_file = get_last_crash_name();
        if (last_file.empty()) {
            reinterpret_cast<cocos2d::CCNode*>(btn)->setVisible(false);
            return;
        }

        auto popup = CrashHelperPopup::create();
        popup->setCrashBtn(reinterpret_cast<cocos2d::CCNode*>(btn));
        popup->show();
    }

    void onCreator(cocos2d::CCObject*)
    {
        spdlog::get("global")->info("MenuLayer::onCreator running");

        auto gm = GameManager::sharedState();
        /*
// messy logic, "caches" check
*/
        if (!IS_GD_INSTALLED && !gm->getGameVariable("3099")) {
            gm->setShowedPirate(true);

            auto protocol_handler = ptr_to_offset<FLAlertLayerProtocol>(this, 0x118);

            auto err_string = cocos2d::CCString::createWithFormat(
                "<cy>Online features</c> have been <cr>disabled</c>.\n\
To enable them, you must <cl>purchase and install</c> the 2.1 version of <cg>Geometry Dash</c> onto your device. \
This allows the GDPS to stay up and also supports the development of the main game.\n\
<co>Thanks</c>!\n(Error code: %i)",
                LAST_ERRNO_CODE);

            auto msg_box = FLAlertLayer::create(
                protocol_handler, "Geometry Dash 2.1 Needed!", err_string->getCString(),
                "OK :(", "Get 2.1", 400.0);

            msg_box->setTag(2);

            auto app_delegate = AppDelegate::get();
            auto scene_pointer = app_delegate->getScenePointer();

            msg_box->setTargetScene(scene_pointer);
            msg_box->show();
            return;
        }

        if (!gm->getClickedEditor()) {
            gm->setClickedEditor(true);
        }

        // i'm too lazy to figure out how to correctly trampoline, gg
        this->willClose();

        auto director = cocos2d::CCDirector::sharedDirector();

        auto clScene = CreatorLayer::scene();
        auto fadeScene = cocos2d::CCTransitionFade::create(0.5, clScene);

        director->replaceScene(fadeScene);
    }
};

bool MenuLayer_init(MenuLayer* self)
{
    spdlog::get("global")->info("MenuLayer::init called");

    if (HookHandler::orig<&MenuLayer_init>(self)) {
        auto gm = GameManager::sharedState();

        if (!IS_GD_INSTALLED) {
            if (!is_package_installed(OFFICIAL_GD_NAME) && !gm->getGameVariable("3099")) {
                auto menu_buttons = reinterpret_cast<cocos2d::CCMenu*>(
                    self->getChildren()->objectAtIndex(2));

                auto create_button = reinterpret_cast<CCMenuItemSpriteExtra*>(
                    menu_buttons->getChildren()->lastObject());

                create_button->setColor({ 0xA6, 0xA6, 0xA6 });
            } else {
                IS_GD_INSTALLED = true;
            }
        }

        if (!get_last_crash_name().empty()) {
            auto crash_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_reportBtn_001.png");
            crash_sprite->setScale(0.75f);

            auto crash_btn = CCMenuItemSpriteExtra::create(
                crash_sprite, nullptr, self,
                static_cast<cocos2d::SEL_MenuHandler>(&MenuLayer_H::onCrash));

            auto menu = cocos2d::CCMenu::createWithItem(crash_btn);
            self->addChild(menu);

            auto director = cocos2d::CCDirector::sharedDirector();

            auto pos_x = director->getScreenLeft() + 23.0f;
            auto pos_y = director->getScreenTop() - 23.0f;

            menu->setPosition(pos_x, pos_y);

            crash_btn->setSizeMult(1.1f);
        }

        return true;
    }

    return false;
}

const char*
GetLoadingString(LoadingLayer* /* self */)
{
    auto t = time(nullptr);
    auto tmp = localtime(&t);

    if (tmp == nullptr || tmp->tm_wday == 1) {
        // thank you stev
        return "Save your game often!";
    } else {
        std::array<const char*, 7> messages {
            { "Did you make a session?", "Be sure to support the original game!",
                "Tip: Blending colors goes beneath other colors.",
                "Sponsored by beans.com", "Its not over till its over...",
                "Check the options menu for special settings!",
                "Tip: Ensure your 3D lines are consistent" }
        };

        auto chosen_index = random() % messages.size();
        return messages.at(chosen_index);
    }
}

void LevelPage_onInfo(LevelPage* self, cocos2d::CCObject* target)
{
    auto gamelevel = get_from_offset<GJGameLevel*>(self, 0x11C);
    if (gamelevel != nullptr) {
        // game uses page -1 for last page
        if (gamelevel->getLevelID() == -1) {
            std::array<const char*, 4> messages {
                { "1.9 GDPS says:\n\
<cl>====================</c>\n\
<cp>====================</c>\n\
====================\n\
<cp>====================</c>\n\
<cl>====================</c>",
                    "<cr>Watch your step</c>, the <co>lasaga</c> approaches...",
                    "This <cp>secret message</c> was sponsored by <cy>beans.com</c>.",
                    "Special thanks to <cl>jamie;</c> for being <cg>the</c>." }
            };

            auto chosen_index = random() % messages.size();

            spdlog::get("global")->info("{} ~ chloe <3", messages.at(chosen_index));

            auto alert = FLAlertLayer::create(nullptr, "You found me..!",
                messages.at(chosen_index), "<3", nullptr, 350.0);
            alert->show();

            return;
        }
    }

    HookHandler::orig<&LevelPage_onInfo>(self, target);
}

class SupportLayerBtnHelper : public cocos2d::CCNode {
public:
    void onBypassBtn(cocos2d::CCObject* /* target */)
    {
        auto gm = GameManager::sharedState();
        gm->toggleGameVariable("3099");
    }

    void onResetAuthKey(cocos2d::CCObject* /* target */)
    {
        if (GameToolbox::doWeHaveInternet()) {
            auto trh = Token::TokenResponseHandler::create();
            trh->deleteAuthKey();
        }
    }
};

void SupportLayer_customSetup(SupportLayer* self)
{
    spdlog::get("global")->info("SupportLayer::customSetup called");

    HookHandler::orig<&SupportLayer_customSetup>(self);

    auto internal_layer = self->getInternalLayer();

    auto director = cocos2d::CCDirector::sharedDirector();
    auto win_size = director->getWinSize();

    auto gm = GameManager::sharedState();
    auto current_status = gm->getGameVariable("3099");

    auto point = cocos2d::CCPoint(
        director->getScreenLeft() + (win_size.width / 2.0f) - 150.0f,
        (2 * director->getScreenTop()) - 77.0f);

    auto legal_menu = reinterpret_cast<cocos2d::CCMenu*>(
        internal_layer->getChildren()->objectAtIndex(5));

    // bypass button
    self->createToggleButton("",
        static_cast<cocos2d::SEL_MenuHandler>(
            &SupportLayerBtnHelper::onBypassBtn),
        !current_status, legal_menu, point);

    auto reset_authkey_sprite = ButtonSprite::create(
        "Reset", 220, 0, 0.4f, false, "bigFont.fnt", "GJ_button_03.png", 25.0f);

    auto reset_authkey_button = CCMenuItemSpriteExtra::create(
        reset_authkey_sprite, nullptr, self,
        static_cast<cocos2d::SEL_MenuHandler>(
            &SupportLayerBtnHelper::onResetAuthKey));

    legal_menu->addChild(reset_authkey_button);

    auto reset_authkey_position = cocos2d::CCPoint(142.0f, -120.0f);
    reset_authkey_button->setPosition(reset_authkey_position);

    return;
}

cocos2d::enumKeyCodes translateAndroidKeyCodeToWindows(int keyCode)
{
    std::unordered_map<int, cocos2d::enumKeyCodes> codes = {
        { AKEYCODE_ESCAPE, cocos2d::kCCEnumKeyCodesEscape },
        { AKEYCODE_SPACE, cocos2d::kCCEnumKeyCodesSpace },
        { AKEYCODE_DPAD_LEFT, cocos2d::kCCEnumKeyCodesLeft },
        { AKEYCODE_DPAD_RIGHT, cocos2d::kCCEnumKeyCodesRight },
        { AKEYCODE_DPAD_UP, cocos2d::kCCEnumKeyCodesUp },
        { AKEYCODE_DPAD_DOWN, cocos2d::kCCEnumKeyCodesDown },
        { AKEYCODE_1, cocos2d::kCCEnumKeyCodesOne },
        { AKEYCODE_2, cocos2d::kCCEnumKeyCodesTwo },
        { AKEYCODE_3, cocos2d::kCCEnumKeyCodesThree },
        { AKEYCODE_Q, cocos2d::kCCEnumKeyCodesQ },
        { AKEYCODE_W, cocos2d::kCCEnumKeyCodesW },
        { AKEYCODE_E, cocos2d::kCCEnumKeyCodesE },
        { AKEYCODE_R, cocos2d::kCCEnumKeyCodesR },
        { AKEYCODE_T, cocos2d::kCCEnumKeyCodesT },
        { AKEYCODE_Y, cocos2d::kCCEnumKeyCodesY },
        { AKEYCODE_A, cocos2d::kCCEnumKeyCodesA },
        { AKEYCODE_S, cocos2d::kCCEnumKeyCodesS },
        { AKEYCODE_D, cocos2d::kCCEnumKeyCodesD },
        { AKEYCODE_F, cocos2d::kCCEnumKeyCodesF },
        { AKEYCODE_G, cocos2d::kCCEnumKeyCodesG },
        { AKEYCODE_Z, cocos2d::kCCEnumKeyCodesZ },
        { AKEYCODE_X, cocos2d::kCCEnumKeyCodesX },
        { AKEYCODE_C, cocos2d::kCCEnumKeyCodesC },
        { AKEYCODE_V, cocos2d::kCCEnumKeyCodesV },
        { AKEYCODE_F1, cocos2d::kCCEnumKeyCodesF1 },
        { AKEYCODE_F2, cocos2d::kCCEnumKeyCodesF2 },
        { AKEYCODE_ENTER, cocos2d::kCCEnumKeyCodesEnter },
        { AKEYCODE_FORWARD_DEL, cocos2d::kCCEnumKeyCodesDelete },
        { AKEYCODE_DEL, cocos2d::kCCEnumKeyCodesBackspace },
        { AKEYCODE_CTRL_LEFT, cocos2d::kCCEnumKeyCodesControl },
        { AKEYCODE_CTRL_RIGHT, cocos2d::kCCEnumKeyCodesControl },
        { AKEYCODE_SHIFT_LEFT, cocos2d::kCCEnumKeyCodesShift },
        { AKEYCODE_SHIFT_RIGHT, cocos2d::kCCEnumKeyCodesShift },
        { AKEYCODE_ALT_LEFT, cocos2d::kCCEnumKeyCodesAlt },
        { AKEYCODE_ALT_RIGHT, cocos2d::kCCEnumKeyCodesAlt },
    };

    if (auto it = codes.find(keyCode); it != codes.end()) {
        return it->second;
    } else {
        spdlog::get("global")->info("received unknown key code: {}", keyCode);
        return cocos2d::kCCEnumKeyCodesNone;
    }
}

bool Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown(void* env, int thiz, int keyCode)
{
    if (keyCode != 0x4 && keyCode != 0x52) {
        auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
        auto translated_code = translateAndroidKeyCodeToWindows(keyCode);

        if (translated_code == cocos2d::kCCEnumKeyCodesControl) {
            keyboard_dispatcher->updateModifierKeys(
                keyboard_dispatcher->getShiftKeyPressed(),
                true,
                keyboard_dispatcher->getAltKeyPressed(),
                false);
        }

        if (translated_code == cocos2d::kCCEnumKeyCodesShift) {
            keyboard_dispatcher->updateModifierKeys(
                true,
                keyboard_dispatcher->getControlKeyPressed(),
                keyboard_dispatcher->getAltKeyPressed(),
                false);
        }

        if (translated_code == cocos2d::kCCEnumKeyCodesAlt) {
            keyboard_dispatcher->updateModifierKeys(
                keyboard_dispatcher->getShiftKeyPressed(),
                keyboard_dispatcher->getControlKeyPressed(),
                true,
                false);
        }

        return keyboard_dispatcher->dispatchKeyboardMSG(translated_code, true);
    } else {
        return HookHandler::orig<&Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown>(env, thiz, keyCode);
    }
}

extern "C" {
    [[gnu::visibility("default")]] bool Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyUp(void* /* env */, int /* thiz */, int keyCode)
    {
        if (keyCode != 0x4 && keyCode != 0x52) {
            auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
            auto translated_code = translateAndroidKeyCodeToWindows(keyCode);

            if (translated_code == cocos2d::kCCEnumKeyCodesControl) {
                keyboard_dispatcher->updateModifierKeys(
                    keyboard_dispatcher->getShiftKeyPressed(),
                    false,
                    keyboard_dispatcher->getAltKeyPressed(),
                    false);
            }

            if (translated_code == cocos2d::kCCEnumKeyCodesShift) {
                keyboard_dispatcher->updateModifierKeys(
                    false,
                    keyboard_dispatcher->getControlKeyPressed(),
                    keyboard_dispatcher->getAltKeyPressed(),
                    false);
            }

            if (translated_code == cocos2d::kCCEnumKeyCodesAlt) {
                keyboard_dispatcher->updateModifierKeys(
                    keyboard_dispatcher->getShiftKeyPressed(),
                    keyboard_dispatcher->getControlKeyPressed(),
                    false,
                    false);
            }

            return keyboard_dispatcher->dispatchKeyboardMSG(translated_code, false);
        }

        return true;
    }

    [[gnu::visibility("default")]] bool Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeActionScroll(void* /* env */, int /* thiz */, float scrollX, float scrollY)
    {
        spdlog::get("global")->trace("scroll params: {} {}", scrollX, scrollY);

        // android scroll values: [-1, 1], -1 is down on the scroll
        // windows scroll values: [-120, 120], 120 is down on the scroll
        // but rob uses glfw thanks robtop, no idea what changed with the values but they different

        constexpr auto SCROLL_FACTOR = -15.0f;

        return cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()->dispatchScrollMSG(
            scrollX * SCROLL_FACTOR, scrollY * SCROLL_FACTOR);
    }
}

} // namespace

namespace Default {

void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook(&LoadingLayer::getLoadingString, GetLoadingString)
        .add_hook(&MenuLayer::onCreator, &MenuLayer_H::onCreator)
        .add_hook("_ZN9MenuLayer4initEv", &MenuLayer_init)
        .add_hook(&SupportLayer::customSetup, SupportLayer_customSetup)
        .add_hook(&LevelPage::onInfo, LevelPage_onInfo)
        .add_hook("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown", &Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown)
        .install_patch(0x1AE47E, { 0x21, 0xE0, 0x00, 0x00 }) // skips adding google play button to menu
        .install_patch(0x1AC13C, { 0x00 }) // sets default pixel format to RGBA8888
        .install_patch(0x1AC142, { 0x00 }); // turns off premultiplied alpha
}
} // namespace Default