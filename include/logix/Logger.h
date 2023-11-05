#pragma once

#include "logix/sinks/Sink.h"
#include "logix/misc/Utils.h"
#include "logix/misc/Common.h"

#include <vector>

#define LOGIX_EXCEPTION_HANDLER                                                                      \
            catch (const std::exception& e) {                                                        \
                Source src(__FILE__, __LINE__, __FUNCTION__);                                        \
                exceptionHandler(fmt::format("[]\t{}", sourceToString(src), e.what()));              \
            } catch (...) {                                                                          \
                Source src(__FILE__, __LINE__, __FUNCTION__);                                        \
                exceptionHandler(fmt::format("[]\tUnidentified exception encountered in the logger", \
                    sourceToString(src)));                                                           \
            }

namespace logix
{

    typedef std::shared_ptr<Sink> SinkPtr;
    typedef std::vector<SinkPtr> SinkList;

    /**
     * 'Logger' is a core class.
     */
    class Logger
    {
    public:
        explicit Logger(SinkList sinks)
            : m_sinks(std::begin(sinks), std::end(sinks)) {}
        virtual ~Logger() = default;

        const SinkList& sinks() const;
        SinkList& sinks();

        virtual void log(const LogMessage& msg) final;

        void doSink(const LogMessage& msg);

        LogLevel level() const;
        void setLevel(LogLevel logLevel);
        bool shouldLog(LogLevel msgLogLevel) const;

    protected:
        virtual void logImpl(const LogMessage& msg);
        void flush();
        void exceptionHandler(const std::string& msg);

    private:
        std::atomic_uchar m_logLevel{ static_cast<std::underlying_type<LogLevel>::type>(LogLevel::Trace) };
        SinkList m_sinks;
    };

} // namespace logix
