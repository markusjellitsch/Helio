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

    // different logs
    void Note(std::string const & text);
    void Error(std::string const & text);
    void Warning(std::string const & text);

    std::string getLastNote() const;
    std::string getLastError() const;
    std::string getLastWarning() const;

    void setLogName(std::string const & name);

    void setLoggingOption(bool note=true,bool warning=true, bool error = true);

protected:

      std::string getTimestamp();
      virtual void DoLog(std::string const & text)=0;

private:
    void Log(std:: string severityLevel,std::string const & text);

    std::string mLastNote;
    std::string mLastError;
    std::string mLastWarning;
    std::string mLogName;

    bool mNoteEnabled=true;
    bool mWarningEnabled=true;
    bool mErrorEnabled=true;
};

#endif // ILOGGER_H
