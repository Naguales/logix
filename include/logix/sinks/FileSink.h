#pragma once

#include "logix/sinks/Sink.h"

namespace logix
{

    /**
     * The 'FileSink' handles the output of log messages to a file.
     */
    class FileSink : public Sink
    {
    public:
        FileSink(const std::string& fileName);
        ~FileSink();

        std::string filename() const
        { return m_fileName; }

    protected:
        void logImpl(const LogMessage& msg) override;
        void flushImpl() override;

    private:
        std::string m_fileName;
        std::FILE* m_fileStream { nullptr };
    };

} // namespace logix
