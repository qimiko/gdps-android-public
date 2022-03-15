#include "modules/token.hpp"

namespace {
void GJAccountManager_dataLoaded(GJAccountManager* self,
    DS_Dictionary* dictionary)
{
    HookHandler::orig<&GJAccountManager_dataLoaded>(self, dictionary);

    auto extension_data = Token::GJAccountManagerExt::create();
    self->setUserObject(extension_data);

    // cocos string is used to prevent segfaults
    auto string_obj = reinterpret_cast<cocos2d::CCString*>(
        dictionary->getObjectForKey("GJAExt_003"));
    if (string_obj != nullptr) {
        extension_data->setToken(string_obj->getCString());
    }

    auto time_string_obj = reinterpret_cast<cocos2d::CCString*>(
        dictionary->getObjectForKey("GJAExt_004"));
    if (time_string_obj != nullptr) {
        auto lst = std::strtoul(time_string_obj->getCString(), nullptr, 0);
        extension_data->setLastSessionTime(lst);
    }
}

void GJAccountManager_encodeDataTo(GJAccountManager* self,
    DS_Dictionary* dictionary)
{
    HookHandler::orig<&GJAccountManager_encodeDataTo>(self, dictionary);

    auto extension_data = dynamic_cast<Token::GJAccountManagerExt*>(self->getUserObject());

    if (extension_data != nullptr && !extension_data->getToken().empty()) {
        auto authkey_obj = cocos2d::CCString::createWithFormat(
            "%s", extension_data->getToken().c_str());

        dictionary->setObjectForKey("GJAExt_003", authkey_obj);

        auto session_time_obj = cocos2d::CCString::createWithFormat(
            "%lu", extension_data->getLastSessionTime());

        dictionary->setObjectForKey("GJAExt_004", session_time_obj);
    }
}

bool CreatorLayer_init(CreatorLayer* self)
{
    spdlog::get("global")->info("CreatorLayer::init called");

    auto gm = GameManager::sharedState();
    auto create_session = gm->getIntGameVariable("4090");
    auto am = GJAccountManager::sharedState();

    if (PlatformToolbox::isNetworkAvailable() && create_session && am->getAccountID() != 0) {
        auto am_ext = dynamic_cast<Token::GJAccountManagerExt*>(am->getUserObject());

        if (am_ext != nullptr) {
            if (!am_ext->getIsSendingReq()) {
                if (am_ext->getToken().empty()) {

                    auto response_handler = Token::TokenResponseHandler::create();
                    response_handler->setHideAlertOnSuccess(true);
                    response_handler->setCreateSessionOnAuthKey(true);

                    response_handler->getAuthKey();
                } else {
                    if (!am_ext->getCreatedSession()) {
                        auto response_handler = Token::TokenResponseHandler::create();
                        response_handler->setHideAlertOnSuccess(true);

                        response_handler->createSession();
                    }
                }
            }
        }
    }

    return HookHandler::orig<&CreatorLayer_init>(self);
}

void GJAccountManager_unlinkFromAccount(GJAccountManager* self)
{
    auto response_handler = Token::TokenResponseHandler::create();
    response_handler->setHideAlert(true);

    response_handler->deleteAuthKey();

    HookHandler::orig<&GJAccountManager_unlinkFromAccount>(self);
}

void GJAccountManager_onLoginAccountCompleted(GJAccountManager* self,
    std::string resp,
    std::string err)
{
    auto response_handler = Token::TokenResponseHandler::create();
    response_handler->setHideAlert(true);

    response_handler->deleteAuthKey();

    HookHandler::orig<&GJAccountManager_onLoginAccountCompleted>(self, resp, err); // NOLINT(performance-unnecessary-value-param)
}

void GameManager_firstLoad(GameManager* self)
{
    HookHandler::orig<&GameManager_firstLoad>(self);
    self->setGameVariable("4090", true);
}
} // namespace

namespace Token {

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

void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook(&GJAccountManager::dataLoaded, GJAccountManager_dataLoaded)
        .add_hook(&GJAccountManager::encodeDataTo, GJAccountManager_encodeDataTo)
        .add_hook("_ZN12CreatorLayer4initEv", CreatorLayer_init)
        .add_hook(&GJAccountManager::unlinkFromAccount, GJAccountManager_unlinkFromAccount)
        .add_hook(&GJAccountManager::onLoginAccountCompleted, GJAccountManager_onLoginAccountCompleted)
        .add_hook(&GameManager::firstLoad, GameManager_firstLoad);
}
} // namespace Token