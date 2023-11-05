#include "logix/AsyncLogger.h"

namespace logix
{

    void AsyncLogger::logImpl(const LogMessage& msg)
    {
        try {
            if (auto threadPoolPtr = m_threadPool.lock()) {
                threadPoolPtr->log(msg);
            }
        } LOGIX_EXCEPTION_HANDLER
    }

} // namespace logix
