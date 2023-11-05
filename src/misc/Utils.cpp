#include "logix/misc/Common.h"
#include "logix/misc/Utils.h"

#include <cstdio>
#ifdef _WIN32
    #include <fileapi.h>    // GetFileAttributesA
    #include <direct.h>     // _mkdir
#else   // Unix
    #ifdef __linux__
        #include <sys/stat.h>   // stat, mkdir, mode_t
    #endif
#endif // Unix
#include <sstream>
#include <iomanip>

namespace logix
{

	std::string parentPath(const std::string& givenPath)
	{
		auto pos = givenPath.find_last_of(preferredSeparator);
		return pos != std::string::npos ? givenPath.substr(0, pos) : std::string{};
	}

    std::string fileName(const std::string& givenPath)
    {
        auto pos = givenPath.find_last_of(preferredSeparator);
        return pos != std::string::npos ? givenPath.substr(pos + 1) : std::string{};
    }

    bool pathExists(const std::string& givenPath) noexcept
    {
        #ifdef _WIN32
            auto attribs = ::GetFileAttributesA(givenPath.c_str());
            return attribs != INVALID_FILE_ATTRIBUTES;
        #else  // The "stat" system call is present in most common Linux/Unix systems.
            struct stat buffer;
            return (::stat(filename.c_str(), &buffer) == 0);
        #endif
    }

    bool createDirectory(const std::string& givenPath)
    {
        if (pathExists(givenPath))
            return true;

        if (givenPath.empty())
            return false;

        auto makeDir = [](const std::string& in_path) {
            #ifdef _WIN32
                return ::_mkdir(in_path.c_str()) == 0;
            #else
                return ::mkdir(in_path.c_str(), mode_t(0755)) == 0;
            #endif
        };

        size_t searchOffset = 0;
        do {
            auto separatorPos = givenPath.find_first_of(preferredSeparator, searchOffset);
            
            // If no folder separator is found, consider the entire path as a folder.
            if (separatorPos == std::string::npos)
                separatorPos = givenPath.size();

            auto subDir = givenPath.substr(0, separatorPos);

            if (!subDir.empty() && !pathExists(subDir) && !makeDir(subDir))
                return false;  // Return in case of a failed directory creation.

            searchOffset = separatorPos + 1;
        } while (searchOffset < givenPath.size());

        return true;
    }

    // Opt for C-style file input/output here,
    // as it has been proven to be faster than C++-style I/O, as indicated by the benchmark results:
    // https://cristianadam.eu/20160410/c-plus-plus-i-slash-o-benchmark/

    std::FILE* fileOpen(const std::string& fileName)
    {
        // Create the parent folder if it doesn't already exist.
        createDirectory(parentPath(fileName));
        std::FILE* fileStream { nullptr };

        const std::string mode = "at";
        #ifdef _WIN32
            fileStream = ::_fsopen((fileName.c_str()), mode.c_str(), _SH_DENYNO);
        #else  // Unix
            fileStream = ::fopen((fileName.c_str()), mode.c_str());
        #endif

        return fileStream;
    }

    void fileClose(std::FILE* fileStream)
    {
        std::fclose(fileStream);
        fileStream = nullptr;
    }

    std::string sourceToString(const Source& src)
    {
        std::string output;

        if (src.m_filename && src.m_function)
            output.append(fmt::format("\t[{}:{}   {}]", src.m_filename, src.m_line, src.m_function));

        return output;
    }

    std::string timestampToString(const std::chrono::system_clock::time_point& timestamp)
    {
        // The fmt::format("{:%F %T}" is not supported in the current C++11 version of fmt.
        // On the other hand, the latest version only supports C++17 and does not compile out of the box for C++11.
        // While it may require further investigation, this issue is not critical.
        // Therefore, use std::put_time here.
        std::time_t tt = std::chrono::system_clock::to_time_t(timestamp);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&tt), "%F %T");
        return ss.str();
    }

    std::string timestampToDateString(const std::chrono::system_clock::time_point& timestamp)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(timestamp);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&tt), "%F");
        return ss.str();
    }

    std::string logMessageToString(const LogMessage& msg)
    {
        std::string output;

        auto duration = msg.m_timestamp.time_since_epoch();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration)
            - std::chrono::duration_cast<std::chrono::milliseconds>(secs);

        output = fmt::format("{}.{:^3}\t{:^10}", timestampToString(msg.m_timestamp),
            static_cast<uint32_t>(milliseconds.count()), logLevelToString(msg.m_level));

        if (msg.m_threadId)
            output.append(fmt::format("\t[{}]", msg.m_threadId));

        if (!msg.m_module.empty())
            output.append(fmt::format("\t{}", msg.m_module));

        output.append(sourceToString(msg.m_source));

        output.append(fmt::format("\t{}\n", msg.m_message));
        
        return output;
    }

    std::tm localtime(const std::time_t& time) noexcept
    {
        #ifdef _WIN32
            std::tm tm;
            ::localtime_s(&tm, &time);
        #else
            std::tm tm;
            ::localtime_r(&time, &tm);
        #endif
        return tm;
    }

} // namespace logix
