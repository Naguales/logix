#include "logix/LogLevel.h"
#include "logix/misc/Common.h"
#include "fmt/format.h"

#include <array>
#include <unordered_map>
#include <algorithm>

namespace logix
{

    std::string logLevelToString(LogLevel logLevel)
    {
        static const std::array<std::string, 8> logLevels = 
            { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL", "BACKTRACE", "NONE" };

        auto const ucLogLevel = static_cast<std::underlying_type<LogLevel>::type>(logLevel);

        if (ucLogLevel > (logLevels.size() - 1))
            throw LogixException { fmt::format("Unsupported log level value \"{}\"", ucLogLevel) };

        return logLevels[ucLogLevel];
    }

    LogLevel logLevelFromString(std::string logLevel)
    {
        static std::unordered_map<std::string, LogLevel> const logLevelsMap = {
            { "TRACE", LogLevel::Trace },   
            { "DEBUG", LogLevel::Debug },
            { "INFO", LogLevel::Info },
            { "WARNING", LogLevel::Warning },
            { "ERROR", LogLevel::Error },
            { "CRITICAL", LogLevel::Critical },
            { "BACKTRACE", LogLevel::Backtrace },
            { "NONE", LogLevel::None }
        };

        std::transform(std::begin(logLevel), std::end(logLevel), std::begin(logLevel),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        auto const logLevelIt = logLevelsMap.find(logLevel);

        if (logLevelIt == std::cend(logLevelsMap))
            throw LogixException { fmt::format("The provided string does not correspond to an existing LogLevel enum value \"{}\"", logLevel) };

        return logLevelIt->second;
    }

} // namespace logix
