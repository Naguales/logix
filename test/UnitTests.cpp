#include "test/UnitTests.h"
#include "test/googletest/gtest.h"

#include "logix/misc/Utils.h"

// LogLevel
// -------------------------------------------------

TEST(LogLevelToStringTest, ValidInput)
{
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Trace), "TRACE");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Debug), "DEBUG");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Info), "INFO");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Warning), "WARNING");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Error), "ERROR");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Critical), "CRITICAL");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::Backtrace), "BACKTRACE");
    EXPECT_EQ(logix::logLevelToString(logix::LogLevel::None), "NONE");
}

TEST(LogLevelToStringTest, InvalidInput)
{
    // Test for an out-of-range value.
    EXPECT_THROW(logix::logLevelToString(static_cast<logix::LogLevel>(100)), logix::LogixException);
}

// Source
// -------------------------------------------------

TEST(SourceTest, DefaultConstructor)
{
    logix::Source source;
    EXPECT_STREQ(source.m_filename, "LogMessage.cpp");
    EXPECT_EQ(source.m_line, 10);
    EXPECT_STREQ(source.m_function, "logix::Source::Source");
}

// LogMessage
// -------------------------------------------------

TEST(LogMessageTest, DefaultConstructor)
{
    logix::LogMessage logMessage;

    EXPECT_EQ(logMessage.m_level, logix::LogLevel::None);
    EXPECT_EQ(logMessage.m_threadId, 0);
}

TEST(LogMessageTest, ConstructorWithSource)
{
    auto filename = __FILENAME__;
    auto func = __FUNCTION__;
    auto line = __LINE__;
    logix::LogMessage logMessage(logix::LogLevel::Error, filename, line, func);

    EXPECT_EQ(logMessage.m_level, logix::LogLevel::Error);
    EXPECT_STREQ(logMessage.m_source.m_filename, filename);
    EXPECT_STREQ(logMessage.m_source.m_function, func);
    EXPECT_EQ(logMessage.m_source.m_line, line);
    EXPECT_EQ(logMessage.m_message, "");
}

TEST(LogMessageTest, ConstructorWithSourceAndMessage)
{
    logix::Source source("Benchmarks.cpp", 48, "asyncMultithreadedTest");
    logix::LogMessage logMessage(logix::LogLevel::Info, source, "This is a test message");

    EXPECT_EQ(logMessage.m_level, logix::LogLevel::Info);
    EXPECT_STREQ(logMessage.m_source.m_filename, "Benchmarks.cpp");
    EXPECT_STREQ(logMessage.m_source.m_function, "asyncMultithreadedTest");
    EXPECT_EQ(logMessage.m_source.m_line, 48);
    EXPECT_EQ(logMessage.m_message, "This is a test message");
}

TEST(LogMessageTest, ConstructorWithMessage)
{
    logix::LogMessage logMessage(logix::LogLevel::Warning, "A different test message");

    EXPECT_EQ(logMessage.m_level, logix::LogLevel::Warning);
    EXPECT_EQ(logMessage.m_message, "A different test message");
}

// Utils::parentPath
// -------------------------------------------------

TEST(ParentPathTest, WindowsPath)
{
    const char* windowsPath = "C:\\Program Files\\Example\\file.txt";
    std::string result = logix::parentPath(windowsPath);
    EXPECT_EQ(result, "C:\\Program Files\\Example");
}

TEST(ParentPathTest, UnixPath)
{
    const char* unixPath = "/var/log/kernel/kern.log";
    std::string result = logix::parentPath(unixPath);
    EXPECT_EQ(result, "/var/log/kernel");
}

TEST(ParentPathTest, NoSeparator)
{
    const char* noSeparatorPath = "noseparatorpath";
    std::string result = logix::parentPath(noSeparatorPath);
    EXPECT_EQ(result, "");
}

TEST(ParentPathTest, RootPath)
{
    const char* rootPath = "/";
    std::string result = logix::parentPath(rootPath);
    EXPECT_EQ(result, "");
}

TEST(ParentPathTest, EmptyPath)
{
    const char* emptyPath = "";
    std::string result = logix::parentPath(emptyPath);
    EXPECT_EQ(result, "");
}

// Utils::parentPath
// -------------------------------------------------

TEST(FileNameTest, WindowsPath)
{
    const char* windowsPath = "C:\\Program Files\\Example\\file.txt";
    std::string result = logix::fileName(windowsPath);
    EXPECT_EQ(result, "file.txt");
}

TEST(FileNameTest, UnixPath)
{
    const char* unixPath = "/var/log/kernel/kern.log";
    std::string result = logix::fileName(unixPath);
    EXPECT_EQ(result, "kern.log");
}

TEST(FileNameTest, NoSeparator)
{
    const char* noSeparatorPath = "noseparatorpath";
    std::string result = logix::fileName(noSeparatorPath);
    EXPECT_EQ(result, "");
}

TEST(FileNameTest, RootPath)
{
    const char* rootPath = "/";
    std::string result = logix::fileName(rootPath);
    EXPECT_EQ(result, "");
}

TEST(FileNameTest, EmptyPath)
{
    const char* emptyPath = "";
    std::string result = logix::fileName(emptyPath);
    EXPECT_EQ(result, "");
}

// Utils::pathExists
// -------------------------------------------------

TEST(PathExistsTest, NonExistingPath)
{
    const char* nonExistingPath = "path/to/nonexistent/file.txt";
    bool result = logix::pathExists(nonExistingPath);
    EXPECT_FALSE(result);
}

TEST(PathExistsTest, EmptyPath)
{
    const char* emptyPath = "";
    bool result = logix::pathExists(emptyPath);
    EXPECT_FALSE(result);
}

TEST(PathExistsTest, WindowsRootPath)
{
    #ifdef _WIN32
        const char* windowsRootPath = "C:\\";
        bool result = logix::pathExists(windowsRootPath);
        EXPECT_TRUE(result);
    #else
        EXPECT_TRUE(true);
    #endif
}

TEST(PathExistsTest, UnixRootPath)
{
    const char* unixRootPath = "/";
    bool result = logix::pathExists(unixRootPath);
    EXPECT_TRUE(result);
}

// Utils::fileOpen
// -------------------------------------------------

TEST(FileOpenTest, ValidFileName)
{
    const char* validFileName = "testfile.txt";
    std::FILE* fileStream = logix::fileOpen(validFileName);
    EXPECT_NE(fileStream, nullptr);
    fclose(fileStream);
}

TEST(FileOpenTest, EmptyFileName)
{
    const char* emptyFileName = "";
    std::FILE* fileStream = logix::fileOpen(emptyFileName);
    EXPECT_EQ(fileStream, nullptr);
}

// Utils::sourceToString
// -------------------------------------------------

TEST(SourceToStringTest, ValidSource)
{
    logix::Source validSource("Benchmarks.cpp", 7, "asyncOneThreadTest");
    std::string result = logix::sourceToString(validSource);
    EXPECT_EQ(result, "\t[Benchmarks.cpp:7   asyncOneThreadTest]");
}

TEST(SourceToStringTest, NullFilename)
{
    logix::Source nullFilenameSource(nullptr, 3, "test");
    std::string result = logix::sourceToString(nullFilenameSource);
    EXPECT_EQ(result, "");
}

TEST(SourceToStringTest, NullFunction)
{
    logix::Source nullFunctionSource("AsyncLogger.h", 48, nullptr);
    std::string result = logix::sourceToString(nullFunctionSource);
    EXPECT_EQ(result, "");
}

TEST(SourceToStringTest, NullFilenameAndFunction)
{
    logix::Source nullSource(nullptr, 0, nullptr);
    std::string result = logix::sourceToString(nullSource);
    EXPECT_EQ(result, "");
}

int runUnitTests()
{
	int argc = 0;
	char** argv = nullptr;

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
