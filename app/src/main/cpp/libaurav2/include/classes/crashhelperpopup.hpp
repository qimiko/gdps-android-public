#pragma once

#ifndef CLASSES_CRASHHELPERPOPUP_HPP
#define CLASSES_CRASHHELPERPOPUP_HPP

#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

#include <cocos2dcpp.h>
#include <rapidjson/document.h>

#include "base/spdlog.hpp"
#include "base/cmake_config.hpp"
#include "base/config.hpp"
#include "base/jni_funcs.hpp"

class CrashHelperPopup : public FLAlertLayer {
public:
    static std::string getLogPath();
    static std::string getLastCrashName();

protected:
    bool init() override;

    void keyBackClicked() override;

    void onClose(cocos2d::CCObject*);
    void onDelete(cocos2d::CCObject*);
    void onExport(cocos2d::CCObject*);
    void onUpload(cocos2d::CCObject*);

    void onUploadCrashFinished(
            cocos2d::extension::CCHttpClient* /* client */,
            cocos2d::extension::CCHttpResponse* response);

public:
    CrashHelperPopup() : crashBtn_(nullptr), loadingCircle_(nullptr), inUpload_(false) {}

    CC_SYNTHESIZE(cocos2d::CCNode *, crashBtn_, CrashBtn);
    CC_SYNTHESIZE(LoadingCircle*, loadingCircle_, LoadingCircle);
    CC_SYNTHESIZE(bool, inUpload_, InUpload);
    CREATE_FUNC(CrashHelperPopup); // NOLINT(modernize-use-auto)
};

#endif //CLASSES_CRASHHELPERPOPUP_HPP
