#include "Logix.h"
#include "logix/sinks/FileSink.h"
#include "logix/sinks/ConsoleSink.h"
#include "logix/AsyncLogger.h"
#include "logix/misc/scope_guard.hpp"

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

void test() {
	std::ostringstream s;
	s << "Greetings from the thread [" << std::this_thread::get_id() << "]!";
	std::string id = s.str();
	for (size_t i = 0; i < 100; ++i) {
		LOG_ERROR << id;
		LOG_WARNING << id;
		LOG_INFO << id;
		LOG_DEBUG << id;
		LOG_TRACE << id;
	}
}

int main()
{
	logix::defaultInitialization();

	auto onExit = sg::make_scope_guard([&]() { logix::finalize(); });

	LOG_INFO << "START";
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	auto threadCount = std::thread::hardware_concurrency();
	std::vector<std::shared_ptr<std::thread>> threads(threadCount);
	for (auto& thread : threads) {
		thread.reset(new std::thread(test));
	}

	LOG_INFO << "JOIN";

	for (auto& thread : threads) {
		thread->join();
	}

	std::chrono::duration<double, std::milli> elapsedTimeMs = std::chrono::steady_clock::now() - start;
	double logMessageCount = 5 * 100 * threadCount;
	LOG_INFO << fmt::format("Number of threads {}", threadCount);
	LOG_INFO << fmt::format("Number of log messages {}", logMessageCount);
	LOG_INFO << fmt::format("Elapsed time {} ms", elapsedTimeMs.count());
	LOG_INFO << fmt::format("Time per message logged {} ms", elapsedTimeMs.count() / logMessageCount);

	return 0;
}
