#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <cocos2dcpp.h>

#include "base/config.hpp"
#include "base/hook_handler.hpp"
#include "modules/modules.hpp"

#if GDMOD_ENABLE_CRASH_DUMPS
#include <client/linux/handler/exception_handler.h>
#include <client/linux/handler/minidump_descriptor.h>
#endif

#if GDMOD_ENABLE_SPDLOG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#endif

#include <memory>
#include <sys/stat.h>
#include <vector>

#if GDMOD_ENABLE_CRASH_DUMPS
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
#endif

[[gnu::constructor]] void aqua()
{
#if GDMOD_ENABLE_SPDLOG
    constexpr auto RINGBUFFER_SIZE = 1024;

    auto ring_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(RINGBUFFER_SIZE);
    ring_sink->set_pattern("[%H:%M:%S.%e] [%l] %v");
    ring_sink->set_level(spdlog::level::trace);

    std::vector<spdlog::sink_ptr> sinks { ring_sink };

    if constexpr (Config::USE_LOGGING) {
        auto android_sink = std::make_shared<spdlog::sinks::android_sink_mt>("gdmod");
        android_sink->set_level(spdlog::level::trace);

        sinks.push_back(android_sink);
    }

    auto logger = std::make_shared<spdlog::logger>("global", std::begin(sinks), std::end(sinks));
    logger->set_level(spdlog::level::trace);

    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);

    spdlog::info("1.9 GDPS u{}", CMakeConfiguration::VERSION);
#endif

#if GDMOD_ENABLE_CRASH_DUMPS
        spdlog::info("initializing crash handler");

        // hardcoded log path... ew
        // people on unofficial builds shouldn't be submitting logs anyways
        // (may eventually be worth finding a good public directory or redirecting output)
        auto log_path = "/data/data/com.kyurime.geometryjump/logs/";
        mkdir(log_path, S_IRWXU | S_IRWXG | S_IRWXO);

        google_breakpad::MinidumpDescriptor descriptor(log_path);

        GLOBAL_EXCEPTION_HANDLER = std::make_unique<google_breakpad::ExceptionHandler>(descriptor, nullptr, dump_callback, nullptr, true, -1);
#endif

    spdlog::info("loading handle");

    setup_handle();

    std::array<std::unique_ptr<Base::Module>, 14> modules {
        std::make_unique<Default::Module>(),
        std::make_unique<Token::Module>(),
        std::make_unique<Levels::Module>(),
        std::make_unique<Hacks::Module>(),
        std::make_unique<Textures::Module>(),
        std::make_unique<Limits::Module>(),
        std::make_unique<Search::Module>(),
        std::make_unique<Settings::Module>(),
        std::make_unique<Urls::Module>(),
        std::make_unique<Share::Module>(),
        std::make_unique<Icons::Module>(),
        std::make_unique<Input::Module>(),
        std::make_unique<Editor::Module>(),
        std::make_unique<Badges::Module>()
    };

#if GDMOD_ENABLE_LOGGING
    spdlog::info("development logging enabled, enabling overlay");
    // manually initialize overlay code
    Overlay::Module().initialize();
#endif

    spdlog::info("running module init");
    for (const auto& module : modules) {
        module->initialize();
    }

    spdlog::info("installing all hooks");

    HookHandler::get_handler().install_hooks();

    spdlog::info("hook install finished :)");
}
