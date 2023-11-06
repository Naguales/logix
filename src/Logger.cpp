#pragma once

#include "logix/Logger.h"
#include "logix/misc/Utils.h"

#include <iostream>

namespace logix
{

    const SinkList& Logger::sinks() const
    {
        return m_sinks;
    }

    SinkList& Logger::sinks()
    {
        return m_sinks;
    }

    void Logger::log(const LogMessage& msg)
    {
        logImpl(msg);
    }

    void Logger::doSink(const LogMessage& msg)
    {
        if (shouldLog(msg.m_level)) {
            for (auto& sink : m_sinks) {
                if (sink->shouldLog(msg.m_level)) {
                    try {
                        sink->log(msg);
                    } LOGIX_EXCEPTION_HANDLER
                }
            }

            // In a practical implementation, a separate public flush method should be presented.
            flush();
        }
    }
    
    void Logger::flush()
    {
        for (auto& sink : m_sinks) {
            try {
                sink->flush();
            } LOGIX_EXCEPTION_HANDLER
        }
    }

    LogLevel Logger::level() const
    {
        return static_cast<LogLevel>(m_logLevel.load(std::memory_order_relaxed));
    }

    void Logger::setLevel(LogLevel logLevel)
    {
        m_logLevel.store(static_cast<std::underlying_type<LogLevel>::type>(logLevel), std::memory_order_relaxed);
    }

    bool Logger::shouldLog(LogLevel msgLogLevel) const
    {
        return msgLogLevel >= static_cast<LogLevel>(m_logLevel.load(std::memory_order_relaxed));
    }

    void Logger::logImpl(const LogMessage& msg)
    {
        doSink(msg);
    }

    void Logger::exceptionHandler(const std::string& msg)
    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration)
            - std::chrono::duration_cast<std::chrono::milliseconds>(secs);

        std::cerr << fmt::format("[!!!LOG EXCEPTION] {}.{:^3}\t{}\n", timestampToString(now),
            static_cast<uint32_t>(milliseconds.count()), msg) << std::endl;
    }

} // namespace logix
