#pragma once

#include "logix/LogMessage.h"

namespace logix
{

	// Due to the absence of std::filesystem support in C++11, implement basic filesystem functions.

	static const char preferredSeparator[] =
	#ifdef _WIN32
		"\\/";
	#else
		"/";
	#endif

	/**
	 * @param [in] givenPath - the path to a directiry or file.
	 * @return the path to the parent directory.
	 */
	std::string parentPath(const std::string& givenPath);

	/**
	 * @param [in] givenPath - the path to a directiry or file.
	 * @return the generic-format filename component of the path.
	 */
	std::string fileName(const std::string& givenPath);

	/**
	 * Verify if the path, whether it's a file or a directory, exists.
	 * @param [in] givenPath - the path to a directiry or file.
	 * @return true if the path exists; otherwise, return false.
	 */
	bool pathExists(const std::string& givenPath) noexcept;

	/**
	 * Creates the specified directory, along with all its parent directories in the hierarchy.
	 * @param [in] givenPath - the path to a directiry or file.
	 * @return true upon success or if the directory already exists; otherwise, return false.
	 */
	bool createDirectory(const std::string& givenPath);

	/**
	 * Opens the specified file for writing.
	 * @param [in] fileName - the path to a file.
	 * @return a pointer to the opened file stream.
	 */
	std::FILE* fileOpen(const std::string& fileName);

	/**
	 * Closes the specified file stream.
	 * @param [inout] fileStream - the pointer to a file stream.
	 */
	void fileClose(std::FILE* fileStream);

	/**
	 * Converts the provided source into a string.
	 * @param [in] src - source info.
	 * @return a string representation of the given source structure.
	 */
	std::string sourceToString(const Source& src);

	/**
	 * Converts the given time point into a string using the date-time format "%F %T",
	 * which is equivalent to the "%Y-%m-%d %H:%M:%S" (the ISO 8601 date format).
	 * @param [in] src - the source info.
	 * @return a date-time string representation of the given source structure.
	 */
	std::string timestampToString(const std::chrono::system_clock::time_point&);
	
	/**
	 * Converts the given time point into a string using the date format "%F",
	 * which is equivalent to the "%Y-%m-%d" (the ISO 8601 date format).
	 * @param [in] src - the source info.
	 * @return a date string representation of the given source structure.
	 */
	std::string timestampToDateString(const std::chrono::system_clock::time_point&);

	/**
	 * Converts the provided log message into a string.
	 * NB: In a practical implementation, this should be substituted with a formatter class.
	 * @param [in] msg - log message.
	 */
	std::string logMessageToString(const LogMessage& msg);

	std::tm localtime(const std::time_t&) noexcept;

} // namespace logix
