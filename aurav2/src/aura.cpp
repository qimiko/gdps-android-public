#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <cocos2dcpp.h>

#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "modules/modules.hpp"

#include <client/linux/handler/exception_handler.h>
#include <client/linux/handler/minidump_descriptor.h>

#if GDMOD_ENABLE_SPDLOG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#endif

#include <memory>
#include <sys/stat.h>
#include <vector>

namespace {

bool dump_callback(const google_breakpad::MinidumpDescriptor& descriptor,
    void* /* context */,
    bool succeeded)
{
    spdlog::get("global")->info("crash log saved to {}", descriptor.path());

    return succeeded;
}

auto GLOBAL_EXCEPTION_HANDLER = std::unique_ptr<google_breakpad::ExceptionHandler>(nullptr);
}

[[gnu::constructor]] void aqua()
{
#if GDMOD_ENABLE_SPDLOG
    constexpr auto RINGBUFFER_SIZE = 1024;

    auto ring_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(RINGBUFFER_SIZE);
    ring_sink->set_pattern("[%H:%M:%S.%e] [%l] %v");

    std::vector<spdlog::sink_ptr> sinks { ring_sink };

    if constexpr (Config::USE_LOGGING) {
        auto android_sink = std::make_shared<spdlog::sinks::android_sink_mt>("gdmod");
        android_sink->set_level(spdlog::level::trace);

        sinks.push_back(android_sink);
    }

    auto logger = std::make_shared<spdlog::logger>("global", std::begin(sinks), std::end(sinks));
    spdlog::register_logger(logger);
    logger->info("1.9 GDPS u{}", CMakeConfiguration::VERSION);

    logger->info("initializing error logger");
#endif

    if constexpr (Config::ENABLE_CRASH_DUMPS) {
        auto log_path = std::string(Config::DATA_PATH) + "logs/";
        mkdir(log_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

        google_breakpad::MinidumpDescriptor descriptor(log_path);

        GLOBAL_EXCEPTION_HANDLER = std::make_unique<google_breakpad::ExceptionHandler>(descriptor, nullptr, dump_callback, nullptr, true, -1);
    }

#if GDMOD_ENABLE_SPDLOG
    logger->info("loading handle");
#endif

    setup_handle();

    std::vector<std::shared_ptr<Base::Module>> modules {
        std::make_shared<Default::Module>(Default::Module()),
        std::make_shared<Token::Module>(Token::Module()),
        std::make_shared<Levels::Module>(Levels::Module()),
        std::make_shared<Hacks::Module>(Hacks::Module()),
        std::make_shared<Textures::Module>(Textures::Module()),
        std::make_shared<Limits::Module>(Limits::Module()),
        std::make_shared<Search::Module>(Search::Module()),
        std::make_shared<Settings::Module>(Settings::Module()),
        std::make_shared<Urls::Module>(Urls::Module()),
        std::make_shared<Badges::Module>(Badges::Module())
    };

#if GDMOD_ENABLE_LOGGING
    logger->info("development logging enabled, enabling overlay");
    modules.push_back(std::make_shared<Overlay::Module>(Overlay::Module()));
#endif

#if GDMOD_ENABLE_SPDLOG
    logger->info("running module init");
#endif
    for (const auto& module : modules) {
        module->initialize();
    }

#if GDMOD_ENABLE_SPDLOG
    logger->info("installing all hooks");
#endif
    HookHandler::get_handler().install_hooks();
#if GDMOD_ENABLE_SPDLOG
    logger->info("hook install finished :)");
#endif
}
