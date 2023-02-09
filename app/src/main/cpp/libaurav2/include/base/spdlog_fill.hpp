#pragma once
#ifndef SPDLOG_FILL_HPP
#define SPDLOG_FILL_HPP

#define LOG_FILL_CREATE(LEVEL) \
    template <typename... A> \
    constexpr void LEVEL([[maybe_unused]] A...) { }

#define LOG_FILL_CREATE_CLASS(LEVEL) \
    template <typename... A> \
    constexpr void LEVEL([[maybe_unused]] A...) const { }

namespace spdlog {

LOG_FILL_CREATE(debug)
LOG_FILL_CREATE(trace)
LOG_FILL_CREATE(info)
LOG_FILL_CREATE(warn)
LOG_FILL_CREATE(error)

namespace {

struct logger_fill {
    LOG_FILL_CREATE_CLASS(debug)
    LOG_FILL_CREATE_CLASS(trace)
    LOG_FILL_CREATE_CLASS(info)
    LOG_FILL_CREATE_CLASS(warn)
    LOG_FILL_CREATE_CLASS(error)

    constexpr logger_fill() { }
};

constexpr auto logger_fill_global = logger_fill();

struct spdlog_fill {
    constexpr auto operator->() const {
        return &logger_fill_global;
    }
};

}

constexpr spdlog_fill get(const char*)
{
    return spdlog_fill();
}
}

#endif