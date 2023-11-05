#include "logix/LogMessage.h"
#include "logix/misc/Common.h"

#include <thread>

namespace logix
{

    Source::Source()
        : m_filename(__FILENAME__), m_line(__LINE__), m_function(__FUNCTION__) {}

    LogMessage::LogMessage(LogLevel level, const std::chrono::system_clock::time_point& timestamp, const std::string& module,
        const Source& src, const std::string& message)
        : m_level(level), m_timestamp(timestamp),
        m_threadId(static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()))),
        m_module(module), m_source(src), m_message(message) {}
    LogMessage::LogMessage(LogLevel level, const Source& src, const std::string& message)
        : LogMessage(level, std::chrono::system_clock::now(), "", src, message) {}
    LogMessage::LogMessage(LogLevel level, const char* filename, int line, const char* func)
        : LogMessage(level, std::chrono::system_clock::now(), "", Source(filename, line, func), "") {}
    LogMessage::LogMessage(LogLevel level, const std::string& message)
        : LogMessage(level, std::chrono::system_clock::now(), "", Source{}, message) {}
    LogMessage::LogMessage(LogLevel level)
        : LogMessage(level, "") {}

} // namespace logix
