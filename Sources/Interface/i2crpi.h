//------------------------------------------------------------------------
// NAME         : i2crpi.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry i2c interface
// -----------------------------------------------------------------------

#ifndef I2CRPI_H
#define I2CRPI_H

#include <interface.h>

#define I2C_RPI_DEVICE_FILE (char *)"/dev/i2c-1"

class I2CRPi : public Interface
{
public:
    I2CRPi()=default;
    virtual int writeMulti(unsigned char const slaveAddress, unsigned char * bytes, size_t const numBytes) override;
    virtual int readMulti(unsigned char const slaveAddress , unsigned char * bytes, size_t const numBytes)override;
    virtual int openInterface(void * params=I2C_RPI_DEVICE_FILE)override;
    virtual int closeInterface()override;

private:

    int mFdI2C = 0;
    bool mOpen = false;

};

#endif // I2CRPI_H
