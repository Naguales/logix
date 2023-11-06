<div align="center">
  <h1>Logix</h1>
  <p><b>Asynchronous and Lightweight C++11 Logging Library</b></p>
</div>

<br>

- [Logix](#logix)
- [Features](#features)
- [Basic Usage](#basic-usage)
- [Performance](#performance)
- [Output](#output)
- [Architecture](#architecture)
- [License](#license)

# Logix
Logix is a lightweight, thread-safe logging library designed for C++11.
It includes essential logger entities and architecture.

## Features
 - Support for **Multiple Logging Targets** (extensible), including:
	- File logging.
	- Console logging with color support.
 - **Log different data types**:
   - Fundamentals (char, bool, integers, floats)
   - Strings (const char pointer, string)
   - Custom structures (when adapted)
 - **Asynchronous logging** via thread pool
 - **Log filtering**
 
## Platforms
* Windows (MSVC 2022 tested)
* Linux

## Basic Usage
```c++
#include "Logix.h"

/** 
 * An example of the asynchronous logger usage.
 */
void main()
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
```

```c++
#include "Logix.h"

/**
 * An example of the synchronous logger usage.
 */
void main()
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
```
## Performance

### Latency

The following message is logged a total of 100,000 times, with varying thread counts:
`LOG_INFO << fmt::format("Reference message {}: {} ~ {}", i, 1.61803398875, M_PI)`.

The table below showcases latency measurements per message in nanoseconds (ns).

| Thread count | Latency per message, ns |
|:------------:|:-----------------------:|
|      1       |          1257           |
|      4       |           679           |
|      8       |           710           |
|     16       |           957           |

Test machine configuration:
- OS: Windows 11
- CPU: Intel(R) Core(TM) i7-11700K @ 3.60GHz
- RAM: 64 GB

## Output
[![LogixConsole.png](https://github.com/Naguales/logix/assets/12149564/c35d6ea7-2760-4804-824a-25e0752c8465)](https://github.com/Naguales/logix/assets/12149564/c35d6ea7-2760-4804-824a-25e0752c8465)

## Architecture
![Logix Architecture](https://github.com/Naguales/logix/assets/12149564/6faa2429-2814-4605-bf39-d1bd8531e4c4)

## License
Logix is licensed under the [MIT License](http://opensource.org/licenses/MIT)

Logix depends on third party libraries with separate copyright notices and license terms.
Your use of the source code for these subcomponents is subject to the terms and conditions of the following licenses.

- ([MIT License](http://opensource.org/licenses/MIT)) {fmt} (https://github.com/fmtlib/fmt/blob/master/LICENSE)
- ([BSD 3-Clause](https://opensource.org/license/bsd-3-clause/)) googletest (https://github.com/google/googletest/blob/main/LICENSE)
- (The Unlicense) scope_guard (https://github.com/ricab/scope_guard/blob/main/LICENSE)
