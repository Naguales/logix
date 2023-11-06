#include "logix/sinks/FileSink.h"
#include "logix/misc/Utils.h"

namespace logix
{

	FileSink::FileSink(const std::string& fileName)
		: m_fileName(fileName)
	{
		if (m_fileName.empty())
			m_fileName = fmt::format("logs{}{}_log.txt", preferredSeparator, timestampToDateString(std::chrono::system_clock::now()));

		m_fileStream = fileOpen(m_fileName);
	}

	FileSink::~FileSink()
	{
		if (m_fileStream != nullptr)
			fileClose(m_fileStream);
	}

	// Opt for C-style file input/output here,
	// as it has been proven to be faster than C++-style I/O, as indicated by the benchmark results:
	// https://cristianadam.eu/20160410/c-plus-plus-i-slash-o-benchmark/

	void FileSink::logImpl(const LogMessage& msg)
	{
		if (m_fileStream == nullptr)
			throw LogixException { fmt::format("The file was not opened previously: \"{}\"", m_fileName) };

		auto logEntry = logMessageToString(msg);
		if (std::fwrite(logEntry.c_str(), 1, logEntry.size(), m_fileStream) != logEntry.size())
			throw LogixException { fmt::format("Failed writing to the file: \"{}\"", m_fileName) };
	}

	void FileSink::flushImpl()
	{
		if (m_fileStream == nullptr)
			throw LogixException{ fmt::format("The file was not opened previously: \"{}\"", m_fileName) };

		if (std::fflush(m_fileStream) != 0)
			throw LogixException{ fmt::format("Failed flush to the file: \"{}\"", m_fileName) };
	}

} // namespace logix
