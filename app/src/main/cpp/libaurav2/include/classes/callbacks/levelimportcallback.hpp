#pragma once

#ifndef CLASSES_CALLBACKS_LEVELIMPORTCALLBACK_HPP
#define CLASSES_CALLBACKS_LEVELIMPORTCALLBACK_HPP

#include <cocos2dcpp.h>
#include <support/base64.h>
#include <jni/JniHelper.h>

#include "base/spdlog.hpp"

class LevelImportCallback : public cocos2d::CCNode, public FLAlertLayerProtocol {
public:
    void onLevelImported(cocos2d::CCNode*);

    void goToScene();

    void FLAlert_Clicked(FLAlertLayer*, bool) override;

    LevelImportCallback() : isNewLevel_(false), toLevel_(nullptr) {}

    CC_SYNTHESIZE(bool, isNewLevel_, IsNewLevel);
    CC_SYNTHESIZE(GJGameLevel*, toLevel_, ToLevel);

    CREATE_FUNC(LevelImportCallback)
};

#endif //CLASSES_CALLBACKS_LEVELIMPORTCALLBACK_HPP
