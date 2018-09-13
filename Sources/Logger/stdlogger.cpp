//------------------------------------------------------------------------
// NAME         : stdlogger.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : logger class to std out
// -----------------------------------------------------------------------

#include "stdlogger.h"
#include <string>
#include <logger.h>
#include <iostream>

using namespace std;


/* --------------------------------------
 * Constructor
 * --------------------------------------*/
StdLogger::StdLogger()
{}

/* --------------------------------------
 * Constructor
 * --------------------------------------*/
StdLogger::StdLogger(string const & name)
{
 setLogName(name);
}

/* --------------------------------------
 * Override DoLog
 * --------------------------------------*/
void StdLogger::DoLog(std::string const & text){
        cout << text << endl;

}
