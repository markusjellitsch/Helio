//------------------------------------------------------------------------
// NAME         : logger.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Logger class for printing error logs
// -----------------------------------------------------------------------

#include "logger.h"
#include "ctime"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>


using namespace std;

/* ------------------------------------
 * Constructor
 * --------------------------------------*/
Logger::Logger()
{
    mLastError = "";
    mLastWarning = "";
    mLastNote = "";
}


/* --------------------------------------
 * Log a string
 * --------------------------------------*/
void Logger::Log(std:: string severityLevel,std::string const & text){

   stringstream stream;
   stream << setw(20) << left << getTimestamp();
   stream << ": ";
   stream << setw(8) << mLogName << " ";
   stream << left << setw(5
                          ) <<  " -->" << left << setw(7) << severityLevel;
   stream << ": ";
   stream << text <<"!";

   DoLog (stream.str());
}


/* --------------------------------------
 * Get timestamp
 * --------------------------------------*/
std::string Logger::getTimestamp(){
    time_t t = time(0);
    struct tm *now = localtime(&t);

    size_t const cBufsize = 80;
    char buf[cBufsize];
    strftime(buf,sizeof(buf),"%d-%m-%Y %I:%M:%S",now);
    string str(buf);
    return str;
}

/* --------------------------------------
 * setting Logging options
 * --------------------------------------*/
void Logger::setLoggingOption(bool note,bool warning, bool error){
    mNoteEnabled = note;
    mWarningEnabled = warning;
    mErrorEnabled = error;
}


/* --------------------------------------
 * Set the log name
 * --------------------------------------*/
void Logger::setLogName(std::string const & name){
    mLogName = name;
}

/* --------------------------------------
 * Print as note
 * --------------------------------------*/
void Logger::Note(std::string const & text){
     if (!mNoteEnabled) return;
     Log("Note",text);
     mLastNote = text;
}

/* --------------------------------------
 * Print as error
 * --------------------------------------*/
void Logger::Error(std::string const & text){
    if (!mErrorEnabled) return;
    Log("Error",text);
    mLastError = text;
}

/* --------------------------------------
 * Print as warning
 * --------------------------------------*/
void Logger::Warning(std::string const & text){
     if (!mWarningEnabled) return;
     Log("Warning",text);
     mLastWarning = text;
}

/* --------------------------------------
 * Get last note
 * --------------------------------------*/
std::string Logger::getLastNote() const{
    return mLastNote;
}

/* --------------------------------------
 * Get last error
 * --------------------------------------*/
std::string Logger::getLastError() const{
    return mLastError;
}

/* --------------------------------------
 * Get last warning
 * --------------------------------------*/
std::string Logger::getLastWarning() const{
    return mLastWarning;
}
