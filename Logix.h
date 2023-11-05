#pragma once

#include "logix/ThreadPool.h"
#include "logix/Logger.h"
#include "fmt/format.h"

#include <type_traits>
#include <mutex>
#include <sstream>

namespace logix
{

    /**
     * Meyer’s Singleton.
     */
    template <typename Derived>
    class Singleton
    {
    public:
        static Derived& instance() noexcept(std::is_nothrow_default_constructible<Derived>::value)
        {
            static Derived instance;
            return instance;
        }

    protected:
        Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) noexcept = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&) noexcept = delete;
        ~Singleton() = default;
    };

    /**
     * 'Logix' is the top-level class for user operations.
     */
    class Logix : public Singleton<Logix>
    {
    public:
        explicit Logix() {};

        std::shared_ptr<ThreadPool> threadPool();
        void setThreadPool(std::shared_ptr<ThreadPool>);

        std::shared_ptr<Logger> logger();
        Logger* loggerPtr();
        void setLogger(std::shared_ptr<Logger>);

        void finalize();

    private:
        std::recursive_mutex m_threadPoolMutex;
        std::mutex m_loggerMutex;
        std::shared_ptr<ThreadPool> m_threadPool;
        std::shared_ptr<Logger> m_logger;
    };

    inline void createThreadPool(unsigned int threadCount = 1)
    {
        auto threadPool = std::make_shared<ThreadPool>(threadCount);
        Logix::instance().setThreadPool(threadPool);
    }

    void defaultInitialization(bool async = true);
    void finalize();

    Logger* loggerPtr();

    void log(const LogMessage& msg);

    /**
     * The auxiliary structure for using log macros in the RAII approach.
     */
    struct LogWrapper
    {
        LogWrapper(LogLevel level)
            : m_logMessage(level) {}
        ~LogWrapper();

        inline std::ostringstream& oss() { return m_oss; }

        std::ostringstream m_oss;
        LogMessage m_logMessage;
    };

} // namespace logix

#define LOG_CALL(level) logix::LogWrapper(level)
#define LOG(level) LOG_CALL(level).oss()

#define LOG_TRACE LOG(logix::LogLevel::Trace)
#define LOG_DEBUG LOG(logix::LogLevel::Debug)
#define LOG_INFO LOG(logix::LogLevel::Info)
#define LOG_WARNING LOG(logix::LogLevel::Warning)
#define LOG_ERROR LOG(logix::LogLevel::Error)
#define LOG_CRITICAL LOG(logix::LogLevel::Critical)
