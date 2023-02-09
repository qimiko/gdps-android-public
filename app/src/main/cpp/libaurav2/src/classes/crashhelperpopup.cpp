#include <classes/crashhelperpopup.hpp>

std::string CrashHelperPopup::getLogPath()
{
    return JNI::get_save_directory() + "logs/";
}

std::string CrashHelperPopup::getLastCrashName() {
    auto largest_mtime = 0u;

    auto last_filename = std::string();
    auto directory = opendir(getLogPath().c_str());

    if (directory != nullptr) {
        while (auto ep = readdir(directory)) {
            // strip .. and .

            if (ep->d_name[0] == '.') {
                continue;
            }

            struct stat file_stat = {};
            stat((getLogPath() + ep->d_name).c_str(), &file_stat); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

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

bool CrashHelperPopup::init() {
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

        auto export_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
        auto export_btn = CCMenuItemSpriteExtra::create(
                export_sprite,
                nullptr,
                this,
                reinterpret_cast<cocos2d::SEL_MenuHandler>(&CrashHelperPopup::onExport));

        this->internalMenu_->addChild(export_btn);
        export_btn->setPosition(width / 2, -(height / 2));

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

void CrashHelperPopup::onUploadCrashFinished(
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

void CrashHelperPopup::onUpload(cocos2d::CCObject*)
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

    auto last_crash_file = getLastCrashName();
    auto crash_path = (getLogPath() + last_crash_file).c_str();
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

void CrashHelperPopup::keyBackClicked()
{
    this->onClose(nullptr);
};

void CrashHelperPopup::onClose(cocos2d::CCObject*)
{
    if (this->inUpload_) {
        return;
    }

    if (this->loadingCircle_ != nullptr) {
        this->loadingCircle_->removeFromParentAndCleanup(true);
        this->loadingCircle_ = nullptr;
    }

    if (this->crashBtn_ != nullptr) {
        if (getLastCrashName().empty()) {
            this->crashBtn_->setVisible(false);
        }
    }

    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void CrashHelperPopup::onDelete(cocos2d::CCObject*)
{
    if (this->inUpload_) {
        return;
    }

    auto last_crash_file = getLastCrashName();
    if (!last_crash_file.empty()) {
        remove((getLogPath() + last_crash_file).c_str());
    }

    this->onClose(nullptr);
}

void CrashHelperPopup::onExport(cocos2d::CCObject*) {
    auto last_crash_file = getLastCrashName();
    auto crash_path = (getLogPath() + last_crash_file).c_str();
    if (!last_crash_file.empty())
    {
        JNI::export_crash_dump(crash_path);
    }
}
