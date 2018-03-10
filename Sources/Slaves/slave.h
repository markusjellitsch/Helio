//------------------------------------------------------------------------
// NAME         : slave.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : slave base class
// -----------------------------------------------------------------------

#ifndef SLAVE_H
#define SLAVE_H

#include <interface.h>
#include <vector>
#include <logger.h>


#define SLAVE_ERROR_INVALID_INTERFACE   -2

#define LOGGER_SEVERITY_NOTE            0x1
#define LOGGER_SEVERITY_WARNING         0x2
#define LOGGER_SEVERITY_ERROR           0x3

#define SLAVE_OK                        0
#define SLAVE_NOK                       -1


typedef std::vector<unsigned char> SlaveData_t;


class Slave
{
public:

    Slave();
    Slave(unsigned char const slaveAddress);

    // set the slave interface
    void setInterface(Interface * const interface,void * params);

    // set the logger
    void setLogger(Logger * logger);

    // set slave address
    void setSlaveAddress(unsigned char address);
    unsigned char getSlaveAddress() const;

    // read from the slave interface
    int readFromSlave(SlaveData_t * data, size_t numBytes);

    // write to the slave.
    int writeToSlave(SlaveData_t data, size_t numBytes);

    // return error
    int getError()const;


private:

    // log an error
    void Log(std::string const & text,unsigned char const severity=LOGGER_SEVERITY_NOTE);

    bool mBusy = false;
    unsigned int mSlaveAddress = 0x00;

    int mError;

protected:
    Logger * mLogger;
    void * mInterfaceParams;
       Interface * mInterface = nullptr;
};

#endif // I2CSLAVE_H
