#include "log.hpp"

namespace cybrion
{
    Log::logger Log::clientLogger;
    Log::logger Log::sysLogger;
    Log::logger Log::gameLogger;

    void Log::Init()
    {
        clientLogger = spdlog::stdout_color_st("client");
        sysLogger = spdlog::stdout_color_st("system");
        gameLogger = spdlog::stdout_color_st("game");

        sysLogger    ->set_pattern("[%Y-%m-%d %T] [sys]: %^%v%$");
        clientLogger ->set_pattern("[%Y-%m-%d %T] [client]: %^%v%$");
        gameLogger   ->set_pattern("[%Y-%m-%d %T] [game]: %^%v%$");

        sysLogger->set_level(spdlog::level::trace);
        clientLogger->set_level(spdlog::level::trace);
        gameLogger->set_level(spdlog::level::trace);
    }
}