#pragma once

#define CYBRION_INFO(...) cybrion::Log::sysLogger->info(__VA_ARGS__)
#define CYBRION_WARN(...) cybrion::Log::sysLogger->warn(__VA_ARGS__)
#define CYBRION_ERROR(...) cybrion::Log::sysLogger->error(__VA_ARGS__)
#define CYBRION_TRACE(...) cybrion::Log::sysLogger->trace(__VA_ARGS__)

#define CYBRION_CLIENT_INFO(...) cybrion::Log::clientLogger->info(__VA_ARGS__)
#define CYBRION_CLIENT_WARN(...) cybrion::Log::clientLogger->warn(__VA_ARGS__)
#define CYBRION_CLIENT_ERROR(...) cybrion::Log::clientLogger->error(__VA_ARGS__)
#define CYBRION_CLIENT_TRACE(...) cybrion::Log::clientLogger->trace(__VA_ARGS__)

#define CYBRION_GAME_INFO(...) cybrion::Log::gameLogger->info(__VA_ARGS__)
#define CYBRION_GAME_WARN(...) cybrion::Log::gameLogger->warn(__VA_ARGS__)
#define CYBRION_GAME_ERROR(...) cybrion::Log::gameLogger->error(__VA_ARGS__)
#define CYBRION_GAME_TRACE(...) cybrion::Log::gameLogger->trace(__VA_ARGS__)

namespace cybrion
{
    class Log
    {
        using logger = std::shared_ptr<spdlog::logger>;
    public:

        static void Init();

        static logger clientLogger;
        static logger sysLogger;
        static logger gameLogger;
    };
}