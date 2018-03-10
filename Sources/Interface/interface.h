//------------------------------------------------------------------------
// NAME         : interface.hp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Interface for serial transmissions (e.g. i2c, spi)
// -----------------------------------------------------------------------

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>
#include <stdlib.h>

#define I_OK                                   0
#define I_NOK                                 -1


class Interface
{
public:
    Interface() = default;
    virtual ~Interface()=default;
    virtual int writeMulti(unsigned char const slaveAddress, unsigned char * bytes, size_t const numBytes)=0;
    virtual int readMulti(unsigned char const slaveAddress, unsigned char * bytes, size_t const numBytes)=0;
    virtual int openInterface(void  * params=nullptr)=0; // TODO:
    virtual int closeInterface()=0;
};

#endif // I2CINTERFACE_H
