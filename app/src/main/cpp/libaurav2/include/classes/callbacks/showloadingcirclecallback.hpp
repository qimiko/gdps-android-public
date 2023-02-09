#pragma once

#ifndef CLASSES_CALLBACKS_SHOWLOADINGCIRCLECALLBACK_HPP
#define CLASSES_CALLBACKS_SHOWLOADINGCIRCLECALLBACK_HPP

#include <cocos2dcpp.h>
#include <jni/JniHelper.h>

class ShowLoadingCircleCallback : public cocos2d::CCNode {
private:
    static LoadingCircle* loadingCircle_;

public:
    void onCircle(cocos2d::CCNode*);
    void onRemoveCircle(cocos2d::CCNode*);

    CREATE_FUNC(ShowLoadingCircleCallback)
};


#endif //CLASSES_CALLBACKS_SHOWLOADINGCIRCLECALLBACK_HPP
