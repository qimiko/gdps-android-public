#pragma once
#ifndef MODULES_TOKEN_HPP
#define MODULES_TOKEN_HPP

#include <vector>
#include <ctime>
#include <cocos2dcpp.h>

#include <rapidjson/document.h>

#include "base/module.hpp"
#include "base/hooks.hpp"
#include "base/hook_handler.hpp"

namespace Token
{
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

	class Module : public Base::Module
	{
	protected:
		void on_initialize();
	};
}

#endif