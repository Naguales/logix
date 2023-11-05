# CMakeList.txt : CMake project for logix, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("logix")

include_directories (${PROJECT_SOURCE_DIR}/include)
include_directories (${PROJECT_SOURCE_DIR}/test)
include_directories (${PROJECT_SOURCE_DIR})

# Add source to this project's executable.
add_executable (logix
	"fmt/format.h" "fmt/format.cc"
	"fmt/ostream.h" "fmt/ostream.cc"
	"fmt/printf.h" "fmt/printf.cc"
	"include/logix/misc/Common.h" "include/logix/misc/scope_guard.hpp"
	"include/logix/misc/Utils.h" "src/misc/Utils.cpp"
	"include/logix/LogLevel.h" "src/LogLevel.cpp"
	"include/logix/LogMessage.h" "src/LogMessage.cpp"
	"include/logix/sinks/Sink.h" "src/sink/Sink.cpp"
	"include/logix/sinks/FileSink.h" "src/sink/FileSink.cpp"
	"include/logix/sinks/ConsoleSink.h" "src/sink/ConsoleSink.cpp"
	"include/logix/ThreadPool.h" "src/ThreadPool.cpp"
	"include/logix/Logger.h" "src/Logger.cpp"
	"include/logix/AsyncLogger.h" "src/AsyncLogger.cpp"
	"Logix.h" "Logix.cpp"
	"test/Benchmarks.h" "test/Benchmarks.cpp"
	"src/App.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET logix PROPERTY CXX_STANDARD 11)
endif()

# TODO: Add tests and install targets if needed.
