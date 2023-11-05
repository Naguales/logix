#include "Logix.h"
#include "logix/sinks/FileSink.h"
#include "logix/sinks/ConsoleSink.h"
#include "logix/AsyncLogger.h"

#include <iostream>
#include <algorithm>

namespace logix
{
	inline std::shared_ptr<ThreadPool> Logix::threadPool()
	{
		std::lock_guard<std::recursive_mutex> lock(m_threadPoolMutex);
		return m_threadPool;
	}

	inline void Logix::setThreadPool(std::shared_ptr<ThreadPool> threadPool)
	{
		std::lock_guard<std::recursive_mutex> lock(m_threadPoolMutex);
		m_threadPool = std::move(threadPool);
	}

	inline std::shared_ptr<Logger> Logix::logger()
	{
		std::lock_guard<std::mutex> lock(m_loggerMutex);
		return m_logger;
	}

	inline Logger* Logix::loggerPtr()
	{
		return m_logger.get();
	}

	inline void Logix::setLogger(std::shared_ptr<Logger> logger)
	{
		std::lock_guard<std::mutex> lock(m_loggerMutex);
		m_logger = std::move(logger);
	}

	void Logix::finalize()
	{
		{
			std::lock_guard<std::recursive_mutex> lock(m_threadPoolMutex);
			m_threadPool.reset();
		}

		{
			std::lock_guard<std::mutex> lock(m_loggerMutex);
			m_logger.reset();
		}
	}

	void defaultInitialization(bool async)
	{
		if (async)
			createThreadPool();
		auto fileSink = std::make_shared<FileSink>(std::string());
		auto consoleSink = std::make_shared<ConsoleSink>();
		SinkList sinks { fileSink, consoleSink };
		auto logger = async ? std::make_shared<AsyncLogger>(Logix::instance().threadPool(), sinks) :
			std::make_shared<Logger>(sinks);
		Logix::instance().setLogger(logger);
	}

	void finalize()
	{
		return Logix::instance().finalize();
	}

	Logger* loggerPtr()
	{
		return Logix::instance().loggerPtr();
	}

	void log(const LogMessage& msg)
	{
		Logix::instance().logger()->log(msg);
	}

	LogWrapper::~LogWrapper()
	{
		m_logMessage.m_message = m_oss.str();
		log(m_logMessage);
	}

} // namespace logix
