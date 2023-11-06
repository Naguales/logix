#pragma once

#include "logix/AsyncLogger.h"

#include <mutex>
#include <sstream>
#include <type_traits>

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

    /**
    * Call this method before utilizing the logger.
    * @param [in] async - specifies whether to use the asynchronous version of the logger
    *                     with a thread pool or the synchronous version.
    */
    void defaultInitialization(bool async = true);

    /**
     * Call this method after you've finished using the asynchronous logger.
     * NB: This workaround is temporarily utilized to ensure the proper finalization of the thread pool
     * and the destruction of sinks; improvements will be made later.
     */
    void finalize();

    Logger* loggerPtr();

    void log(const LogMessage& msg);

    /**
     * The auxiliary structure for using log macros in the RAII approach.
     */
    struct LogWrapper
    {
        LogWrapper(LogLevel level, const char* filename, int line, const char* func)
            : m_logMessage(level, filename, line, func) {}
        ~LogWrapper();

        inline std::ostringstream& oss() { return m_oss; }

        std::ostringstream m_oss;
        LogMessage m_logMessage;
    };

} // namespace logix

#define LOG_CALL(level) logix::LogWrapper(level, __FILENAME__, __LINE__, __FUNCTION__)
#define LOG(level) LOG_CALL(level).oss()

#define LOG_TRACE LOG(logix::LogLevel::Trace)
#define LOG_DEBUG LOG(logix::LogLevel::Debug)
#define LOG_INFO LOG(logix::LogLevel::Info)
#define LOG_WARNING LOG(logix::LogLevel::Warning)
#define LOG_ERROR LOG(logix::LogLevel::Error)
#define LOG_CRITICAL LOG(logix::LogLevel::Critical)
