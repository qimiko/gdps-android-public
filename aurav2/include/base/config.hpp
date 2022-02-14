#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef GDMOD_ENABLE_LOGGING
#define GDMOD_ENABLE_LOGGING false
#endif

#ifndef GDMOD_ENABLE_CRASH_DUMPS
#define GDMOD_ENABLE_CRASH_DUMPS false
#endif

#ifndef GDMOD_DATA_PATH
#define GDMOD_DATA_PATH "/data/data/com.kyurime.geometryjump/"
#endif

#ifndef GDMOD_CRASH_UPLOAD_URL
#define GDMOD_CRASH_UPLOAD_URL ""
#endif

namespace {
// c++17 has a constexpr function for this but we don't!
constexpr std::size_t length_of(const char* x) {
    return *x ? 1 + length_of(x + 1) : 0;
}
}

namespace Config {
constexpr bool USE_LOGGING = GDMOD_ENABLE_LOGGING;
constexpr bool ENABLE_CRASH_DUMPS = GDMOD_ENABLE_CRASH_DUMPS;
constexpr auto DATA_PATH = GDMOD_DATA_PATH;
constexpr auto CRASH_UPLOAD_URL = GDMOD_CRASH_UPLOAD_URL;
constexpr auto CRASH_ENABLED = length_of(CRASH_UPLOAD_URL) > 0;
}

#endif