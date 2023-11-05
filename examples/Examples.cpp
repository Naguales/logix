#include "Logix.h"
#include "logix/LogLevel.h"
#include "logix/misc/scope_guard.hpp"
#include "fmt/format.h"

#include <numeric>

#define _USE_MATH_DEFINES
#include <math.h>

/** 
 * An example of the asynchronous logger usage.
 */
void asyncLoggerUsageExample()
{
	// The default initialization for using the asynchronous logger.
	logix::defaultInitialization();

	// By default, the trace log level is utilized.
	// Setting the log level is optional.
	logix::loggerPtr()->setLevel(logix::LogLevel::Trace);

	// Cleanup is required after you've finished using the asynchronous logger.
	auto onExit = sg::make_scope_guard([&]() { logix::finalize(); });

	// Log to all registered sinks, in this case: file and console.
	LOG_ERROR << "This represents an ERROR log entry";
	LOG_WARNING << fmt::format("This represents a {} log entry {}",
		logLevelToString(logix::LogLevel::Warning), std::numeric_limits<int>::min());
	LOG_INFO << fmt::format("This represents an {} log entry {}",
		logLevelToString(logix::LogLevel::Info), M_PI);
	LOG_DEBUG << fmt::format("This represents a {} log entry {}",
		logLevelToString(logix::LogLevel::Debug), std::numeric_limits<size_t>::max());
	LOG_TRACE << fmt::format("This represents a {} log entry {}",
		logLevelToString(logix::LogLevel::Trace), std::numeric_limits<double>::max());
}

/**
 * An example of the synchronous logger usage.
 */
void syncLoggerUsageExample()
{
	// The default initialization for using the synchronous logger.
	logix::defaultInitialization(false);
	logix::loggerPtr()->setLevel(logix::LogLevel::Debug);

	// Log to all registered sinks, in this case: file and console.
	LOG_ERROR << "This represents an ERROR log entry";
	LOG_WARNING << fmt::format("This represents a {} log entry {}",
		logLevelToString(logix::LogLevel::Warning), std::numeric_limits<int>::min());
	LOG_INFO << fmt::format("This represents an {} log entry {}",
		logLevelToString(logix::LogLevel::Info), M_PI);
	LOG_DEBUG << fmt::format("This represents a {} log entry {}",
		logLevelToString(logix::LogLevel::Debug), std::numeric_limits<size_t>::max());
	LOG_TRACE << fmt::format("This represents a {} log entry {}",
		logLevelToString(logix::LogLevel::Trace), std::numeric_limits<double>::max());
}
