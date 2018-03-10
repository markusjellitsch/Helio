//------------------------------------------------------------------------
// NAME         : slave.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : slave base class
// -----------------------------------------------------------------------

#include "slave.h"
#include <stdint.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <Logger/logger.h>

using namespace std;

/* --------------------
 * Constructor
----------------------*/
Slave::Slave()
{
 mError = 0;
}


/* -------------------------------------
 * Constructor
--------------------------------------*/
Slave::Slave(unsigned char const slaveAddress){
        mSlaveAddress = slaveAddress;
}


/* ------------------------------------------
 * Set the  Communication Interface
-------------------------------------------*/
void Slave::setInterface(Interface * const interface,void * params){

  assert(interface != nullptr);
  mInterface = interface;
  mInterfaceParams = params;

}

/* ----------------------------------------
 *  Set logger
-------------------------------------------*/
void Slave::setLogger(Logger *logger){
    mLogger = logger;
    assert(mLogger!= nullptr);
    stringstream logStream;
    logStream << "I²C-Slave (0x"<<hex << mSlaveAddress << ")";
    mLogger->setLogName(logStream.str());
}

/* ---------------------------------------------
 *  Set slave address
-----------------------------------------------*/
void Slave::setSlaveAddress(unsigned char slaveAddress){
    mSlaveAddress = slaveAddress;
}


/* ---------------------------------------------
 * Return slave address
-----------------------------------------------*/
unsigned char Slave::getSlaveAddress() const{
    return mSlaveAddress;
}

/* ---------------------------------------------
 *  Return error
-----------------------------------------------*/
int Slave::getError()const{
    return mError;
}


/* ------------------------------------------------
 * Read from the interface
-------------------------------------------------*/
int Slave::readFromSlave(SlaveData_t * data, size_t numBytes){

        errno = 0;
        mError = 0;

       stringstream logString;
       logString << "Start reading Data from Slave";
       Log(logString.str());

        // try to open I²C-Interface
        if (mInterface == nullptr){
            mError = SLAVE_ERROR_INVALID_INTERFACE;
            Log("Interface not set!",LOGGER_SEVERITY_ERROR);
            return mError;
       }

      if (mInterface->openInterface(mInterfaceParams) != I_OK){
          mError = errno;
          Log("Opening I2C failed",LOGGER_SEVERITY_ERROR);
          return errno;
      }

      unsigned char const cBuffersize = 100;
      unsigned char buf[cBuffersize];

      int error = mInterface->readMulti(mSlaveAddress,buf,numBytes);
      if (error != I_OK){
          Log("Read Data from Slave failed",LOGGER_SEVERITY_ERROR);
          mError = error;
          return error;
      }

      for (unsigned int i = 0; i< numBytes;i++)
      data->push_back(buf[i]); // just a dummy at the moment

      mError = mInterface->closeInterface();

      Log("Reading Data from Slave successfull");

      return I_OK;
}

/* ------------------------------------------------------
 *  Write to the interface
----------------------------------------------------------*/
int Slave::writeToSlave(SlaveData_t data, size_t numBytes){

    errno = 0;
    mError = 0;

    Log("Start writing Data to Slave ");

   // try to open I²C-Interface
   if (mInterface == nullptr){
        mError = SLAVE_ERROR_INVALID_INTERFACE;
        Log("Interface not set!",LOGGER_SEVERITY_ERROR);
        return mError;
   }

  if (mInterface->openInterface(mInterfaceParams) != I_OK){
      mError = errno;
      Log("Opening I2C failed!",LOGGER_SEVERITY_ERROR);
      return mError;
      return errno;
  }

  unsigned char const cBuffersize = 100;
  unsigned char buf[cBuffersize];

  for (unsigned int i = 0; i< numBytes;i++)
    buf[i]= data[i]; // just a dummy at the moment

  int error = mInterface->writeMulti(mSlaveAddress,buf,numBytes);
  if (error != I_OK){
      Log("Writing Data to Slave failed",LOGGER_SEVERITY_ERROR);
      mError = error;
      return error;
  }

  error = mInterface->closeInterface();
  mError = error;

   Log("Writing Data to Slave successful");

  return error;
}

/* ----------------------------------------------
 * Log data
------------------------------------------------*/
void Slave::Log(std::string const & text,unsigned char const severity){
     if (mLogger == nullptr)return;

     switch (severity){
         case LOGGER_SEVERITY_ERROR:
             mLogger->Error(text);
             return;

         case LOGGER_SEVERITY_NOTE:
             mLogger->Note(text);
             return;

         case LOGGER_SEVERITY_WARNING:
             mLogger->Warning(text);
             return;
    }
}
