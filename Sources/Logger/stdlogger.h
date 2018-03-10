//------------------------------------------------------------------------
// NAME         : stdlogger.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Logger class to std out
// -----------------------------------------------------------------------

#ifndef STDLOGGER_H
#define STDLOGGER_H

#include <logger.h>
#include <string>

class StdLogger : public Logger
{
public:

      // Constructor
      StdLogger();
      StdLogger(std::string const & name);

      // Hook method
      virtual void DoLog(std::string const & text);

};

#endif // STDLOGGER_H
