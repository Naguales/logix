#pragma once

#include "logix/LogMessage.h"

#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>

namespace logix
{

    /**
     * Represents a thread pool responsible for queuing log messages and outputting them to sinks.
     */
    class ThreadPool
    {
    public:
        ThreadPool(unsigned int in_threadCount = 2);
        ~ThreadPool();

        /**
         * Queues the log message for processing.
         */
        void log(const LogMessage&);

    private:
        void workerLoop();

        /**
         * Handles the next message in the queue.
         */
        void processLog(const LogMessage&);

        std::vector<std::thread> m_threads;
        std::mutex m_mutex;

        // The condition variable is utilized for the threads to await log messages before proceeding.
        std::condition_variable_any m_cv;

        // The queue of pending log messages awaiting processing.
        std::queue<LogMessage> m_logMessageQueue;

        // During thread pool shutdown, setting this flag to true signals the threads to stop looping and finish.
        bool m_finish { false };
    };

} // namespace logix
