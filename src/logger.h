#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
class Logger
{
public:
    Logger()=default;
    static void init();

    inline static std::shared_ptr<spdlog::logger> logger;
};

#endif // LOGGER_H
