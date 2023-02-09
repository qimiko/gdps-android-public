#pragma once

#ifndef CLASSES_EXTENSIONS_GJACCOUNTMANAGEREXT_HPP
#define CLASSES_EXTENSIONS_GJACCOUNTMANAGEREXT_HPP

#include <cocos2dcpp.h>
#include <ctime>

class GJAccountManagerExt : public cocos2d::CCNode
{
public:
    GJAccountManagerExt() : _createdSession(false) {}

    CC_SYNTHESIZE(std::string, _token, Token)
    CC_SYNTHESIZE(bool, _createdSession, CreatedSession)
    CC_SYNTHESIZE(std::time_t, _lastSessionTime, LastSessionTime)
    CC_SYNTHESIZE(bool, _isSendingReq, IsSendingReq)

    CREATE_FUNC(GJAccountManagerExt)
};

#endif //CLASSES_EXTENSIONS_GJACCOUNTMANAGEREXT_HPP
