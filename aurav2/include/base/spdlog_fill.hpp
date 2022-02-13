#pragma once
#ifndef SPDLOG_FILL_HPP
#define SPDLOG_FILL_HPP

#include <memory>

namespace spdlog {

namespace {

struct logger_fill {
    template <typename... A>
    constexpr void info([[maybe_unused]] A...) const {}

    template <typename... A>
    constexpr void trace([[maybe_unused]] A...) const { }

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