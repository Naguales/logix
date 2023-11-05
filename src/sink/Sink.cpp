#include "logix/sinks/Sink.h"

namespace logix
{

    void Sink::log(const LogMessage& msg)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        logImpl(msg);
    }
    
    void Sink::flush()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        flushImpl();
    }

    LogLevel Sink::level() const
    {
        return static_cast<LogLevel>(m_logLevel.load(std::memory_order_relaxed));
    }

    void Sink::setLevel(LogLevel logLevel)
    {
        m_logLevel.store(static_cast<std::underlying_type<LogLevel>::type>(logLevel), std::memory_order_relaxed);
    }

    bool Sink::shouldLog(LogLevel msgLogLevel) const
    {
        return msgLogLevel >= static_cast<LogLevel>(m_logLevel.load(std::memory_order_relaxed));
    }

} // namespace logix
