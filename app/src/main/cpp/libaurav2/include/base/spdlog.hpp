#ifndef BASE_SPDLOG_HPP
#define BASE_SPDLOG_HPP

#if GDMOD_ENABLE_SPDLOG
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#else
#include "spdlog_fill.hpp"
#endif

#endif //BASE_SPDLOG_HPP
