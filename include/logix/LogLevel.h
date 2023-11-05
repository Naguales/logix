#pragma once

#include <string>

namespace logix
{

	enum class LogLevel : unsigned char
	{
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Critical,
		Backtrace,
		None
	};

	/**
	 * Converts a LogLevel enum value to a string.
	 * @param [in] logLevel - LogLevel.
	 * @return the corresponding string value.
	 */
	std::string logLevelToString(LogLevel logLevel);

	/**
	 * Converts a string to a LogLevel enum value.
	 * @param [in] logLevel - a log level string value.
	 * The possible values: "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL", "BACKTRACE", "NONE".
	 * @return the corresponding LogLevel enum value.
	 */
	LogLevel logLevelFromString(std::string logLevel);

} // namespace logix
