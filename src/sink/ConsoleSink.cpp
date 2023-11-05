#include "logix/sinks/ConsoleSink.h"
#include "logix/misc/Common.h"
#include "logix/misc/Utils.h"

#include <iostream>

namespace logix
{

	ConsoleSink::ConsoleSink()
	{
		m_logLevelColors = {
			{ LogLevel::Trace, fmt::WHITE },
			{ LogLevel::Debug, fmt::CYAN },
			{ LogLevel::Info, fmt::GREEN },
			{ LogLevel::Warning, fmt::YELLOW },
			{ LogLevel::Error, fmt::RED },
			{ LogLevel::Critical, fmt::RED }
		};
	}

	ConsoleSink::~ConsoleSink()
	{
		flushImpl();
	}

	void ConsoleSink::logImpl(const LogMessage& msg)
	{
		// Apply distinct colors to messages based on their log levels.
		auto logEntry = logMessageToString(msg);
		auto colorIt = m_logLevelColors.find(msg.m_level);
		fmt::Color color = colorIt != std::end(m_logLevelColors) ? colorIt->second : fmt::WHITE;
		fmt::print_colored(color, "{}", logEntry);
	}

	void ConsoleSink::flushImpl()
	{
		std::cout << std::flush;
	}

} // namespace logix
