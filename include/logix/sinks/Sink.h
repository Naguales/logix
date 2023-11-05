#pragma once

#include "logix/LogMessage.h"

#include <atomic>
#include <mutex>

namespace logix
{

    /**
     * 'Sink' is a base class that serves as the core representation for sinks,
     * which are utilized as message receivers or outputs for logging message distribution.
     */
	class Sink
	{
	public:
        Sink() = default;
        virtual ~Sink() = default;

        Sink(const Sink&) = delete;
        Sink(Sink&&) = delete;

        Sink& operator=(const Sink&) = delete;
        Sink& operator=(Sink&&) = delete;

        virtual void log(const LogMessage& msg) final;
        virtual void flush() final;

        LogLevel level() const;
        void setLevel(LogLevel logLevel);
        bool shouldLog(LogLevel msgLogLevel) const;

    protected:
        virtual void logImpl(const LogMessage& msg) = 0;
        virtual void flushImpl() = 0;

        std::mutex m_mutex;
        std::atomic_uchar m_logLevel { static_cast<std::underlying_type<LogLevel>::type>(LogLevel::Trace) };
	};

} // namespace logix
