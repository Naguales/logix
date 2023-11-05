#pragma once

#include "logix/Logger.h"
#include "logix/ThreadPool.h"

namespace logix
{

	/**
	 * 'AsyncLogger' is a core class equipped with a thread pool.
	 */
	class AsyncLogger final : public Logger
	{
	public:
		explicit AsyncLogger(std::weak_ptr<ThreadPool> threadPool, SinkList sinks)
			: Logger(sinks),
			m_threadPool(std::move(threadPool)) {}

	protected:
		void logImpl(const LogMessage& msg) override;
	
	private:
		std::weak_ptr<ThreadPool> m_threadPool;
	};

} // namespace logix
