#pragma once

#include "LogLevel.h"
#include "logix/misc/Common.h"

namespace logix
{

    /**
     * Represents source code information,
     * including the source file name, line number, and function name.
     */
    struct Source
    {
        Source();
        Source(const char* filename, int line, const char* func)
            : m_filename(filename), m_line(line), m_function(func) {}

        const char* m_filename { nullptr };
        int m_line { 0 };
        const char* m_function { nullptr };
    };
    
    /**
     * Represents a log message, encapsulating information such as
     * log level, timestamp, thread ID, module name, source code details, and the log message text.
     */
    struct LogMessage
    {
        LogMessage() = default;
        LogMessage(LogLevel level, const std::chrono::system_clock::time_point& timestamp, const std::string& module,
            const Source& src, const std::string& message);
        LogMessage(LogLevel level, const Source& src, const std::string& message);
        LogMessage(LogLevel level, const char* filename, int line, const char* func);
        LogMessage(LogLevel level, const std::string& message);
        LogMessage(LogLevel level);

        LogMessage(const LogMessage& other) = default;
        LogMessage& operator=(const LogMessage& other) = default;

        LogLevel m_level { LogLevel::None };
        std::chrono::system_clock::time_point m_timestamp;
        size_t m_threadId { 0 };
        std::string m_module;
        Source m_source;
        std::string m_message;
    };

} // namespace logix
