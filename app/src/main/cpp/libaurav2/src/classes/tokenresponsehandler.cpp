#include "classes/tokenresponsehandler.hpp"

void TokenResponseHandler::onGetAuthKeyReqFinish(
        cocos2d::extension::CCHttpClient* /* client */,
        cocos2d::extension::CCHttpResponse* response)
{
    auto am = GJAccountManager::sharedState();
    auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

    if (am_ext != nullptr) {
        am_ext->setIsSendingReq(false);
    }

    auto response_data = response->getResponseData();
    std::string response_string(response_data->begin(), response_data->end());

    spdlog::get("global")->info("authkey installed");

    rapidjson::Document resp_document;
    if (resp_document.Parse(response_string.c_str()).HasParseError()) {
        return;
    }

    auto success = resp_document["success"].GetBool();
    if (success) {
        if (am_ext != nullptr) {
            am_ext->setToken(resp_document["authkey"].GetString());

            if (this->_createSessionOnAuthKey) {
                this->createSession();
            }
        }
    } else {
        if (!this->_hideAlert) {
            FLAlertLayer::create(nullptr, "Authkey creation failure",
                                 resp_document["error"].GetString(), "OK", nullptr,
                                 350.0f)
                    ->show();
        }
    }
}

void TokenResponseHandler::onDeleteAuthKeyReqFinish(
        cocos2d::extension::CCHttpClient* /* client */,
        cocos2d::extension::CCHttpResponse* response)
{
    auto am = GJAccountManager::sharedState();
    auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

    if (am_ext != nullptr) {
        am_ext->setIsSendingReq(false);
    }

    auto response_data = response->getResponseData();
    std::string response_string(response_data->begin(), response_data->end());

    rapidjson::Document resp_document;
    if (resp_document.Parse(response_string.c_str()).HasParseError()) {
        return;
    }

    auto success = resp_document["success"].GetBool();
    if (success) {
        if (am_ext != nullptr) {
            am_ext->setToken(std::string());

            if (!this->_hideAlert && !this->_hideAlertOnSuccess) {
                FLAlertLayer::create(nullptr, "Success",
                                     "Authkey has been deleted. Open the creator menu "
                                     "to generate a new one.",
                                     "OK", nullptr, 350.0f)
                        ->show();
            }
        }
    } else {
        if (!this->_hideAlert) {
            FLAlertLayer::create(nullptr, "Authkey deletion failure",
                                 resp_document["error"].GetString(), "OK", nullptr,
                                 350.0f)
                    ->show();
        }
    }
}

void TokenResponseHandler::onCreateSessionReqFinish(
        cocos2d::extension::CCHttpClient* /* client */,
        cocos2d::extension::CCHttpResponse* response)
{
    auto am = GJAccountManager::sharedState();
    auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

    if (am_ext != nullptr) {
        am_ext->setIsSendingReq(false);
    }

    auto response_data = response->getResponseData();
    std::string response_string(response_data->begin(), response_data->end());

    spdlog::get("global")->info("session created: {}", response_string.c_str());

    rapidjson::Document resp_document;
    if (resp_document.Parse(response_string.c_str()).HasParseError()) {
        return;
    }

    auto success = resp_document["success"].GetBool();
    if (success) {
        if (am_ext != nullptr) {
            am_ext->setCreatedSession(true);
            am_ext->setLastSessionTime(std::time(nullptr));

            if (!this->_hideAlert && !this->_hideAlertOnSuccess) {
                auto an = AchievementNotifier::sharedState();

                auto session_notif = cocos2d::CCString::createWithFormat(
                        "Logged in as user %s", am->getAccountUserName().c_str());
                auto notif_string = session_notif->getCString();

                an->notifyAchievement("Session created", notif_string, nullptr);
            }
        }
    } else {
        if (!this->_hideAlert) {
            FLAlertLayer::create(nullptr, "Session creation failure",
                                 resp_document["error"].GetString(), "OK", nullptr,
                                 350.0f)
                    ->show();
        }
    }
}

void TokenResponseHandler::createSession()
{
    auto am = GJAccountManager::sharedState();
    auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

    if (am_ext == nullptr || am_ext->getToken().empty()) {
        return;
    }

    spdlog::get("global")->info("sending session creation req");

    auto http_client = cocos2d::extension::CCHttpClient::getInstance();
    auto http_request = new cocos2d::extension::CCHttpRequest();

    auto req_data = cocos2d::CCString::createWithFormat(
            "authkey=%s", am_ext->getToken().c_str());
    auto req_string = req_data->getCString();

    http_request->setUrl("https://absolllute.com/gdps/gdapi/api/session");
    http_request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    http_request->setResponseCallback(
            this, static_cast<cocos2d::extension::SEL_HttpResponse>(&TokenResponseHandler::onCreateSessionReqFinish));
    http_request->setRequestData(req_string, strlen(req_string));

    http_client->send(http_request);
    http_request->release();

    am_ext->setIsSendingReq(true);
}

void TokenResponseHandler::getAuthKey()
{
    auto am = GJAccountManager::sharedState();
    auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

    spdlog::get("global")->info("sending authkey creation req");

    auto http_client = cocos2d::extension::CCHttpClient::getInstance();
    auto http_request = new cocos2d::extension::CCHttpRequest();

    auto req_data = cocos2d::CCString::createWithFormat(
            "username=%s&password=%s", am->getAccountUserName().c_str(),
            am->getAccountPassword().c_str());
    auto req_string = req_data->getCString();

    http_request->setUrl("https://absolllute.com/gdps/gdapi/api/login");
    http_request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    http_request->setResponseCallback(
            this, static_cast<cocos2d::extension::SEL_HttpResponse>(&TokenResponseHandler::onGetAuthKeyReqFinish));
    http_request->setRequestData(req_string, strlen(req_string));

    http_client->send(http_request);
    http_request->release();

    if (am_ext != nullptr) {
        am_ext->setIsSendingReq(true);
    }
}

void TokenResponseHandler::deleteAuthKey()
{
    auto am = GJAccountManager::sharedState();
    auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

    if (am_ext == nullptr || am_ext->getToken().empty()) {
        return;
    }

    spdlog::get("global")->info("sending delete auth key req");

    auto http_client = cocos2d::extension::CCHttpClient::getInstance();
    auto http_request = new cocos2d::extension::CCHttpRequest();

    auto req_data = cocos2d::CCString::createWithFormat(
            "authkey=%s&mode=single", am_ext->getToken().c_str());
    auto req_string = req_data->getCString();

    http_request->setUrl("https://absolllute.com/gdps/gdapi/api/logout");
    http_request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
    http_request->setResponseCallback(
            this, static_cast<cocos2d::extension::SEL_HttpResponse>(&TokenResponseHandler::onDeleteAuthKeyReqFinish));
    http_request->setRequestData(req_string, strlen(req_string));

    http_client->send(http_request);
    http_request->release();

    am_ext->setIsSendingReq(true);
}
