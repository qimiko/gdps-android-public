#pragma once

#ifndef CLASSES_TOKENRESPONSEHANDLER_HPP
#define CLASSES_TOKENRESPONSEHANDLER_HPP

#include <cocos2dcpp.h>
#include <rapidjson/document.h>

#include "base/spdlog.hpp"

#include "classes/extensions/gjaccountmanagerext.hpp"

class TokenResponseHandler : public cocos2d::CCNode
{
public:
    void onGetAuthKeyReqFinish(cocos2d::extension::CCHttpClient *client, cocos2d::extension::CCHttpResponse *response);
    void onDeleteAuthKeyReqFinish(cocos2d::extension::CCHttpClient *client, cocos2d::extension::CCHttpResponse *response);
    void onCreateSessionReqFinish(cocos2d::extension::CCHttpClient *client, cocos2d::extension::CCHttpResponse *response);
    void createSession();
    void getAuthKey();
    void deleteAuthKey();

    CREATE_FUNC(TokenResponseHandler);

    CC_SYNTHESIZE(bool, _createSessionOnAuthKey, CreateSessionOnAuthKey)
    CC_SYNTHESIZE(bool, _hideAlert, HideAlert)
    CC_SYNTHESIZE(bool, _hideAlertOnSuccess, HideAlertOnSuccess)
};

#endif //CLASSES_TOKENRESPONSEHANDLER_HPP
