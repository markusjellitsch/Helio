//------------------------------------------------------------------------
// NAME         : rs485_rpi.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry RS485 interface
// -----------------------------------------------------------------------

#ifndef RS485RPI_H
#define RS485RPI_H

#include <Interface/interface.h>
#include <string>
#include <termios.h>

#define RS485_RPI_DEV_FILENAME ((char *)"/dev/ttyAMA0")

#define SPI_RPI_CHIPSELECT_0            0
#define SPI_RPI_CHIPSELECT_1            1

#define RS485_RPI_BUFFERSIZE             256

#define RS485_RPI_BAUD19200              19200
#define RS485_RPI_BAUD115200             115200

#define RS485_RPI_DEFAULT_BAUDRATE       115200

#define RS485_RPI_DI_PIN                    25


typedef struct {
    char *   deviceName;
    unsigned int baudRate;
    unsigned int rxTimeout;
    unsigned int rxBytes;
}RS485Config_t;

class RS485Rpi : public Interface
{
public:
    RS485Rpi();
    virtual int writeMulti(unsigned char const slaveAddress, unsigned char * bytes, size_t const numBytes) override;
    virtual int readMulti(unsigned char const slaveAddress , unsigned char * bytes, size_t const numBytes) override;

    virtual int openInterface(void * params) override;
    virtual int closeInterface()override;

    virtual int sendMessage(std::string const & message, unsigned int const numBytes0);

    virtual void printf(const char * format,...);


private:

    unsigned char mTxBuffer[RS485_RPI_BUFFERSIZE];
    unsigned char mRxBuffer[RS485_RPI_BUFFERSIZE];
    int mFdRS485 = 0;
    bool mOpen = false;
};

#endif // RS485PI_H
