//------------------------------------------------------------------------
// NAME         : logger.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Logger class for printing error logs
// -----------------------------------------------------------------------

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
public:
    Logger();
    virtual ~Logger()=default;

    // different logs
    void Message(std::string const & text);
    void Error(std::string const & text);
    void Verbose(std::string const & text);

    std::string getLastMessage() const;
    std::string getLastError() const;
    std::string getLastVerbose() const;

    void setLogName(std::string const & name);

    void setLoggingOption(bool message=true,bool verbose=true, bool error = true);

protected:

      std::string getTimestamp();
      virtual void DoLog(std::string const & text)=0;

private:
    void Log(std:: string logLevel,std::string const & text);

    std::string mLastMessage;
    std::string mLastVerbose;
    std::string mLastError;
    std::string mLogName;

    bool mMessageEnabled=true;
    bool mVerboseEnabled=true;
    bool mErrorEnabled=true;
};

#endif // ILOGGER_H
