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
    mLastMessage = "";
    mLastVerbose = "";
}


/* --------------------------------------
 * Log a string
 * --------------------------------------*/
void Logger::Log(std:: string logLevel,std::string const & text){

   stringstream stream;
   stream << left << "[" << logLevel[0] << "] ";
   stream << setw(20) << left << getTimestamp();
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
void Logger::setLoggingOption(bool message,bool verbose, bool error){
    mMessageEnabled = message;
    mVerboseEnabled = verbose;
    mErrorEnabled = error;
}


/* --------------------------------------
 * Set the log name
 * --------------------------------------*/
void Logger::setLogName(std::string const & name){
    mLogName = name;
}

/* --------------------------------------
 * Print as message
 * --------------------------------------*/
void Logger::Message(std::string const & text){
     if (!mMessageEnabled) return;
     Log("Message",text);
     mLastMessage = text;
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
 * Print as verbose
 * --------------------------------------*/
void Logger::Verbose(std::string const & text){
     if (!mVerboseEnabled) return;
     Log("Verbose",text);
     mLastVerbose = text;
}

/* --------------------------------------
 * Get last message
 * --------------------------------------*/
std::string Logger::getLastMessage() const{
    return mLastMessage;
}

/* --------------------------------------
 * Get last error
 * --------------------------------------*/
std::string Logger::getLastError() const{
    return mLastError;
}

/* --------------------------------------
 * Get last verbose
 * --------------------------------------*/
std::string Logger::getLastVerbose() const{
    return mLastVerbose;
}
