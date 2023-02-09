#include "modules/token.hpp"

namespace {
void GJAccountManager_dataLoaded(GJAccountManager* self,
    DS_Dictionary* dictionary)
{
    HookHandler::orig<&GJAccountManager_dataLoaded>(self, dictionary);

    auto extension_data = GJAccountManagerExt::create();
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

    auto extension_data = dynamic_cast<GJAccountManagerExt*>(self->getUserObject());

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
    auto create_session = gm->getGameVariable(GameVariable::AUTOMATIC_SESSION);
    auto am = GJAccountManager::sharedState();

    if (PlatformToolbox::isNetworkAvailable() && create_session && am->getAccountID() != 0) {
        auto am_ext = dynamic_cast<GJAccountManagerExt*>(am->getUserObject());

        if (am_ext != nullptr) {
            if (!am_ext->getIsSendingReq()) {
                if (am_ext->getToken().empty()) {

                    auto response_handler = TokenResponseHandler::create();
                    response_handler->setHideAlertOnSuccess(true);
                    response_handler->setCreateSessionOnAuthKey(true);

                    response_handler->getAuthKey();
                } else {
                    if (!am_ext->getCreatedSession()) {
                        auto response_handler = TokenResponseHandler::create();
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
    auto response_handler = TokenResponseHandler::create();
    response_handler->setHideAlert(true);

    response_handler->deleteAuthKey();

    HookHandler::orig<&GJAccountManager_unlinkFromAccount>(self);
}

void GJAccountManager_onLoginAccountCompleted(GJAccountManager* self,
    std::string resp,
    std::string err)
{
    auto response_handler = TokenResponseHandler::create();
    response_handler->setHideAlert(true);

    response_handler->deleteAuthKey();

    HookHandler::orig<&GJAccountManager_onLoginAccountCompleted>(self, resp, err); // NOLINT(performance-unnecessary-value-param)
}

void GameManager_firstLoad(GameManager* self)
{
    HookHandler::orig<&GameManager_firstLoad>(self);
    self->setGameVariable(GameVariable::AUTOMATIC_SESSION, true);
}
} // namespace

namespace Token {
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