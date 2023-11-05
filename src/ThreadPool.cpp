#include "logix/ThreadPool.h"
#include "Logix.h"

#include <algorithm>

namespace logix
{

    ThreadPool::ThreadPool(unsigned int in_threadCount)
    {
        // Check the count of threads supported by the system.
        auto threadCount = std::thread::hardware_concurrency();
        threadCount = std::min(in_threadCount, std::max(threadCount, decltype(threadCount)(1)));

        for (unsigned int i = 0; i < threadCount; ++i) {
            // The threads will run the 'workerLoop' method.
            m_threads.push_back(std::thread(&ThreadPool::workerLoop, this));
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_finish = true;
        }

        // Awaken all threads so that they can complete their tasks and be joined.
        m_cv.notify_all();
        
        // Join all the threads to ensure a smooth program exit.
        for (auto& thread : m_threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    void ThreadPool::log(const LogMessage& msg)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_logMessageQueue.push(msg);
        }

        // Notify one thread that there are log messages ready for processing.
        m_cv.notify_one();
    }

    void ThreadPool::workerLoop()
    {
        while (true) {
            LogMessage msg;

            // Set up a scope to minimize queue locking duration.
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [&] {
                    // Wake up only when there are log messages in the queue
                    // or when the thread pool is shutting down.
                    return !m_logMessageQueue.empty() || m_finish;
                });

                // When the shutdown process is underway,
                // exit without any further work processing attempts.
                if (m_finish && m_logMessageQueue.empty())
                    break;

                msg = m_logMessageQueue.front();
                m_logMessageQueue.pop();
            }

            processLog(msg);
        }
    }

    void ThreadPool::processLog(const LogMessage& msg)
    {
        // TODO: Utilize this workaround for logger access temporarily; fix it later.
        Logix::instance().loggerPtr()->doSink(msg);
    }

} // namespace logix
