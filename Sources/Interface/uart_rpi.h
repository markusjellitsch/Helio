//------------------------------------------------------------------------
// NAME         : uart_rpi.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry uart interface
// -----------------------------------------------------------------------

#ifndef UARTRPI_H
#define UARTRPI_H

#include <interface.h>
#include <string>
#include <termios.h>

#define UART_RPI_DEV_FILENAME ((char *)"/dev/ttyAMA0")

#define SPI_RPI_CHIPSELECT_0            0
#define SPI_RPI_CHIPSELECT_1            1
#define UART_RPI_BUFFERSIZE             256

#define UART_RPI_BAUD19200              19200
#define UART_RPI_BAUD115200             115200

#define UART_RPI_DEFAULT_BAUDRATE       115200


typedef struct {
    char *   deviceName;
    unsigned int baudRate;
}UARTConfig_t;

class UARTRpi : public Interface
{
public:
    UARTRpi();
    virtual int writeMulti(unsigned char const slaveAddress, unsigned char * bytes, size_t const numBytes) override;
    virtual int readMulti(unsigned char const slaveAddress , unsigned char * bytes, size_t const numBytes) override;

    virtual int openInterface(void * params) override;
    virtual int closeInterface()override;

    virtual int sendMessage(std::string const & message, unsigned int const numBytes0);

    virtual void printf(const unsigned char * format,...);


private:

    unsigned char mTxBuffer[UART_RPI_BUFFERSIZE];
    unsigned char mRxBuffer[UART_RPI_BUFFERSIZE];
    int mFdUART = 0;
    bool mOpen = false;
};

#endif // UARTPI_H
