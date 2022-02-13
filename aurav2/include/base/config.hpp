#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace Config {

#ifdef GDMOD_ENABLE_LOGGING
	constexpr bool USE_LOGGING = true;
#else
	constexpr bool USE_LOGGING = false;
#endif

	constexpr bool ENABLE_CRASH_DUMPS = true;

	constexpr auto DATA_PATH = "/data/data/com.kyurime.geometryjump/";
}

#endif