//------------------------------------------------------------------------
// NAME         : spirpi.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry spi interface
// -----------------------------------------------------------------------

#ifndef SPIRPI_H
#define SPIRPI_H

#include <Interface/interface.h>
#include <string>

#define SPI_RPI_CS0_FILENAME ((char *)"/dev/spidev0.0")
#define SPI_RPI_CS1_FILENAME ((char *)"/dev/spidev0.1")

#define SPI_RPI_CHIPSELECT_0        0
#define SPI_RPI_CHIPSELECT_1        1

#define SPI_RPI_BITS                8
#define SPI_RPI_BUSSPEED            100000

#define SPI_RPI_BUFFERSIZE          256

// pin configuration for cs & dry
#define SPI_SLAVE_CS_PIN            5
#define SPI_SLAVE_DRY_PIN           6

#define SPI_COUNT_DEFAULT           3000

typedef struct {
    std::string   deviceName;
    unsigned int busSpeed;
    unsigned char bits;
    bool invertCS;
}SPIConfig_t;

class SPIRPi : public Interface
{
public:
    SPIRPi();
    virtual int writeMulti(unsigned char const slaveAddress, unsigned char * bytes, size_t const numBytes) override;
    virtual int readMulti(unsigned char const slaveAddress , unsigned char * bytes, size_t const numBytes) override;
    int writeMulti(unsigned char const slaveAddress, uint16_t * buffer, size_t const numBytes);
    virtual int openInterface(void * params) override;
    virtual int closeInterface()override;
    virtual int setCount(unsigned int count);

    virtual int startTransaction(unsigned char * tx_buffer, size_t const num_tx_bytes, unsigned char * rx_buffer, size_t const num_rx_bytes);
    SPIConfig_t getConfig()const;

private:

    SPIConfig_t mConfig;
    unsigned char mMosiBuffer[SPI_RPI_BUFFERSIZE];
    unsigned char mMisoBuffer[SPI_RPI_BUFFERSIZE];
    int mFdSPI = 0;
    bool mOpen = false;
    unsigned int mCount=SPI_COUNT_DEFAULT;

};

#endif // I2CRPI_H
