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
        reinterpret_cast<uint8_t*>(const_cast<char*>(s)), // NOLINT
         // preserve null byte
        reinterpret_cast<uint8_t*>(const_cast<char*>(s + strlen(s) + 1))); // NOLINT
}

template <const int MAX_LEN, std::size_t N>
const std::vector<uint8_t> validate_string_as_bytes(char const (&s)[N])
{
    return string_to_bytes(validate_str_length<MAX_LEN>(s)); // NOLINT
}

void CCHttpClient_send(cocos2d::extension::CCHttpClient* self, cocos2d::extension::CCHttpRequest* x)
{
    auto url = std::string(x->getUrl());

    const std::string url_boomlings = "http://www.boomlings.com/database";
    const std::string url_replacement = "https://absolllute.com/gdps/gdapi";

    if (auto host_pos = url.find(url_boomlings); host_pos != std::string::npos) {
        url.replace(host_pos, url_boomlings.length(), url_replacement);
        x->setUrl(url.c_str());
    }

    HookHandler::orig<&CCHttpClient_send>(self, x);
}

void GameManager_reloadAllStep3(GameManager* self)
{
    auto file = cocos2d::CCFileUtils::sharedFileUtils();
    file->setAndroidPath(JNI::get_save_directory());

    HookHandler::orig<&GameManager_reloadAllStep3>(self);
}

void AppDelegate_applicationDidFinishLaunching(AppDelegate* self) {
    if (JNI::is_launcher_build()) {
        spdlog::warn("Launcher mode enabled, things may break!");
    }

    HookHandler::orig<&AppDelegate_applicationDidFinishLaunching>(self);

    auto file = cocos2d::CCFileUtils::sharedFileUtils();
    file->setAndroidPath(JNI::get_save_directory());
}

std::string FileOperation_getFilePath() {
    return JNI::get_save_directory();
}

std::string ZipUtils_decompressString2Wrap(unsigned char* data, bool use_decrypt, int size) {
    // this wrapper exists to remove all uses of strlen
    // strlen would cause a crash if placed at page end without null termination
    if (data == nullptr || size == 0) {
        spdlog::get("global")->error("ds2_wrap:: nulld fail");
        return "";
    }

    // check if first byte invalid
    if (*data == 0x00) {
        spdlog::get("global")->error("ds2_wrap:: dfb fail");
        return "";
    }

    if (use_decrypt) {
        // in-place xor. why not
        for (auto i = 0u; i < size; i++) {
            data[i] ^= 0x0B;
        }

        // this is the behavior of the original func
        // auto ds = std::string(reinterpret_cast<const char*>(data), size);
        // auto decrypted = cocos2d::ZipUtils::encryptDecrypt(ds, 0x0B);
        // decoded_length = cocos2d::base64Decode(data, size, &b64decoded);
    }

    unsigned char* b64decoded = nullptr;
    auto decoded_length = cocos2d::base64Decode(data, size, &b64decoded, true);

    if (decoded_length < 1) {
        delete[] b64decoded;

        spdlog::get("global")->error("ds2_wrap:: dcl fail, dl {}", decoded_length);
        return "";
    } else {
        unsigned char* inflated = nullptr;

        auto inflated_length = cocos2d::ZipUtils::ccInflateMemory(b64decoded, decoded_length, &inflated);
        if (inflated_length < 1) {
            delete[] b64decoded;
            delete[] inflated;

            spdlog::get("global")->error("ds2_wrap:: infll fail, il {}", inflated_length);
            return "";
        }

        auto output_str = std::string(reinterpret_cast<char*>(inflated), inflated_length);

        delete[] b64decoded;
        delete[] inflated;

        return output_str;
    }
}

bool DS_Dictionary_loadRootSubDictFromCompressedFile(DS_Dictionary* self, const char* filename) {
    if (LoadingFailedHijackLayer::hasDestroyedState()) {
        // ignore load if state already broken
        return false;
    }

    auto fpath = FileOperation::getFilePath() + filename;

    spdlog::info("loading save file {}", fpath);

    auto ifs = std::ifstream(fpath);
    if (!ifs) {
        return false;
    }

    auto data = std::vector<char>(
            std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
/*
    spdlog::get("global")->info(
            "lrsdfcf({}):: ifstream size {}, fb {:#04x} lb {:#04x}",
            filename, data.size(), data[0x00], data[data.size() - 1]);
*/

    // handle unencoded data
    // this is dangerous if strlen is called like ever
    if (self->loadRootSubDictFromString(std::string(data.begin(), data.end()))) {
        return true;
    }

    auto dfile = ZipUtils_decompressString2Wrap(
            reinterpret_cast<unsigned char*>(data.data()), true, data.size());

//    spdlog::get("global")->info("lrsdfcf:: ds2 return: {}", dfile.substr(0,10));

    if (self->loadRootSubDictFromString(dfile)) {
        return true;
    }

    // this means our save data is not functional in some way
    // destroy state and show corruption layer
    LoadingFailedHijackLayer::destroyGameState();

    return false;
}

cocos2d::CCScene* LoadingLayer_scene(LoadingLayer* self, bool reload) {
    if (!LoadingFailedHijackLayer::hasDestroyedState()) {
        return HookHandler::orig<&LoadingLayer_scene>(self, reload);
    }

    return LoadingFailedHijackLayer::scene();
}

}

namespace Urls {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .install_patch(0x470FED, validate_string_as_bytes<27>("http://absolllute.com/tool"))
        .install_patch(0x472692, validate_string_as_bytes<38>("https://absolllute.com/api/1.9?join\0\0"))
        .install_patch(0x4726B8, validate_string_as_bytes<32>("https://absolllute.com/api/1.9\0"))
        .install_patch(0x4726D8, validate_string_as_bytes<41>("https://absolllute.com/api/1.9?youtube\0\0"))
        .install_patch(0x48B4B3, validate_string_as_bytes<129>("Your <cy>command</c> has been successfully run! For questions contact <cy>Absolute#1507</c> with ID: <cy>%i</c>\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"))
        .install_patch(0x48B534, validate_string_as_bytes<7>("Win!\0\0"))
        .add_hook(&cocos2d::extension::CCHttpClient::send, CCHttpClient_send)
        .add_hook(&DS_Dictionary::loadRootSubDictFromCompressedFile, &DS_Dictionary_loadRootSubDictFromCompressedFile)
        .add_hook(&LoadingLayer::scene, &LoadingLayer_scene)
        .add_hook(&GameManager::reloadAllStep3, GameManager_reloadAllStep3)
        .add_hook(&FileOperation::getFilePath, &FileOperation_getFilePath)
        .add_hook("_ZN11AppDelegate29applicationDidFinishLaunchingEv", &AppDelegate_applicationDidFinishLaunching);
}
} // namespace Urls