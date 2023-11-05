#pragma once

#include "logix/sinks/Sink.h"
#include "fmt/format.h"

#include <unordered_map>

namespace logix
{

    /**
     * The 'ConsoleSink' handles the output of log messages to a console.
     */
    class ConsoleSink : public Sink
    {
    public:
        ConsoleSink();
        ~ConsoleSink();

    protected:
        void logImpl(const LogMessage& msg) override;
        void flushImpl() override;

    private:
        std::unordered_map<LogLevel, fmt::Color> m_logLevelColors;
    };

} // namespace logix