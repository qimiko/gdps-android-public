#include "modules/urls.hpp"

namespace {

template <const int MAX_LEN, std::size_t N>
constexpr auto& validate_str_length(char const (&s)[N])
{
    static_assert(N == MAX_LEN, "String exceeds size limit!");
    return s;
}

const std::vector<uint8_t> string_to_bytes(const char* s)
{
    // this is a mess but if it works it's fine
    return std::vector<uint8_t>(
        reinterpret_cast<uint8_t*>(const_cast<char*>(s)),
        reinterpret_cast<uint8_t*>(const_cast<char*>(s + strlen(s) + 1))); // preserve null byte
}

template <const int MAX_LEN, std::size_t N>
const std::vector<uint8_t> validate_string_as_bytes(char const (&s)[N])
{
    return string_to_bytes(validate_str_length<MAX_LEN>(s));
}

void CCHttpClient_send(cocos2d::extension::CCHttpClient* self, cocos2d::extension::CCHttpRequest* x)
{
    auto url = std::string(x->getUrl());

    spdlog::get("global")->trace("request being made to endpoint {}", url);

    constexpr auto HOSTNAME_LENGTH = 18u;

    const std::string url_boomlings = validate_str_length<HOSTNAME_LENGTH>("www.boomlings.com");
    const std::string url_replacement = validate_str_length<HOSTNAME_LENGTH>("gdps.nettik.co.uk");

    if (auto host_pos = url.find(url_boomlings); host_pos != std::string::npos) {
        url.replace(host_pos, url_boomlings.length(), url_replacement);
        x->setUrl(url.c_str());
    }

    HookHandler::orig<&CCHttpClient_send>(self, x);
}

void GameManager_reloadAllStep3(GameManager* self)
{
    auto file = cocos2d::CCFileUtils::sharedFileUtils();

    file->setAndroidPath(Config::DATA_PATH);

    HookHandler::orig<&GameManager_reloadAllStep3>(self);
}

}

namespace Urls {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .install_patch(0x46FB30, validate_string_as_bytes<37>("/data/data/com.kyurime.geometryjump/"))
        .install_patch(0x470FED, validate_string_as_bytes<27>("http://absolllute.com/tool"))
        .install_patch(0x472692, validate_string_as_bytes<38>("https://absolllute.com/api/1.9?join\0\0"))
        .install_patch(0x4726B8, validate_string_as_bytes<32>("https://absolllute.com/api/1.9\0"))
        .install_patch(0x4726D8, validate_string_as_bytes<41>("https://absolllute.com/api/1.9?youtube\0\0"))
        .install_patch(0x48B4B3, validate_string_as_bytes<129>("Your <cy>command</c> has been successfully run! For questions contact <cy>Absolute#1507</c> with ID: <cy>%i</c>\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"))
        .install_patch(0x48B534, validate_string_as_bytes<7>("Win!\0\0"))
        .add_hook(&cocos2d::extension::CCHttpClient::send, CCHttpClient_send)
        .add_hook(&GameManager::reloadAllStep3, GameManager_reloadAllStep3);
}
} // namespace Urls