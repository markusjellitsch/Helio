//------------------------------------------------------------------------
// NAME         : spirpi.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry spi interface
// -----------------------------------------------------------------------

#include "spirpi.h"
#include <linux/spi/spidev.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <wiringPi.h>

using namespace std;


/*---------------------------------------------------------------------------
 * Constructor
 *--------------------------------------------------------------------------*/
SPIRPi::SPIRPi(){

    for (int i = 0; i< SPI_RPI_BUFFERSIZE;i++){
        mMisoBuffer[i] = 0;
        mMosiBuffer[i] = 0;
    }
}


/*---------------------------------------------------------------------------
 * SPI Write Function to write data on the SPI-Bus.
 *--------------------------------------------------------------------------*/
int SPIRPi::writeMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     memcpy(mMosiBuffer,buffer,numBytes);
     struct spi_ioc_transfer spi[numBytes];


     unsigned int i = 0;
     int retVal = -1;

     for (i = 0; i<numBytes;i++){
         spi[i].tx_nbits  = 0;
         spi[i].rx_nbits = 0;
         spi[i].tx_buf = (unsigned long)(mMosiBuffer +i);
         spi[i].rx_buf = (unsigned long) (mMisoBuffer + i);
         spi[i].len = sizeof(*(mMosiBuffer +i));
         spi[i].delay_usecs = 0;
         spi[i].speed_hz = mBusSpeed;
         spi[i].bits_per_word = 0;
         spi[i].cs_change = mInvertCS;
         spi[i].pad = 0;
     }

     retVal = ioctl(mFdSPI,SPI_IOC_MESSAGE(numBytes),&spi);

    if(retVal < 0){
      return errno;
    }

    return I_OK;

 }

int SPIRPi::writeMulti(unsigned char const slaveAddress, uint16_t * buffer, size_t const numBytes){

     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     memcpy(mMosiBuffer,buffer,numBytes);
     struct spi_ioc_transfer spi[numBytes];


     unsigned int i = 0;
     int retVal = -1;

     for (i = 0; i<numBytes;i++){
         spi[i].tx_nbits  = 0;
         spi[i].rx_nbits = 0;
         spi[i].tx_buf = (unsigned long)(buffer +i);
         spi[i].rx_buf = 0;
         spi[i].len = sizeof(*(buffer +i));
         spi[i].delay_usecs = 0;
         spi[i].speed_hz = mBusSpeed;
         spi[i].bits_per_word = 0;
         spi[i].cs_change = mInvertCS;
         spi[i].pad = 0;
     }

     retVal = ioctl(mFdSPI,SPI_IOC_MESSAGE(numBytes),&spi);

    if(retVal < 0){
      return errno;
    }

    return I_OK;

 }


/*---------------------------------------------------------------------------
 * SPI Function to read data from the SPI-Bus.
 *--------------------------------------------------------------------------*/
 int SPIRPi::readMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     slaveAddress;

     struct spi_ioc_transfer spi[numBytes];

     unsigned int i = 0;
     int retVal = -1;

     for (i = 0; i<1;i++){
         spi[i].tx_nbits  = 0;
         spi[i].rx_nbits = 0;
         spi[i].tx_buf = 0;
         spi[i].rx_buf = (unsigned long) (buffer);
         spi[i].len = numBytes;
         spi[i].delay_usecs = 0;
         spi[i].speed_hz = mBusSpeed;
         spi[i].bits_per_word = 0;
         spi[i].cs_change = mInvertCS;
         spi[i].pad = 0;
     }

     retVal = ioctl(mFdSPI,SPI_IOC_MESSAGE(1),&spi);

    if(retVal < 0){
      return errno;
    }

    return I_OK;
 }

int SPIRPi::startTransaction(unsigned char * tx_buffer, size_t const num_tx_bytes, unsigned char * rx_buffer, size_t const num_rx_bytes){


    size_t const cNumBytes = 2;
    struct spi_ioc_transfer transmit;
      struct spi_ioc_transfer receive;

    unsigned int i = 0;
    int retVal = -1;

    // transmit struct
    transmit.tx_nbits  = 0;
    transmit.rx_nbits = 0;
    transmit.tx_buf = (unsigned long) tx_buffer;
    transmit.rx_buf = 0;
    transmit.len = num_tx_bytes;
    transmit.delay_usecs = 0;
    transmit.speed_hz = mBusSpeed;
    transmit.bits_per_word = 0;
    transmit.cs_change = 0;
    transmit.pad = 0;

    // receive struct
    receive.tx_nbits  = 0;
    receive.rx_nbits = 0;
    receive.tx_buf = 0;
    receive.rx_buf = (unsigned long) rx_buffer;
    receive.len = num_rx_bytes;
    receive.delay_usecs = 0;
    receive.speed_hz = mBusSpeed;
    receive.bits_per_word = 0;
    receive.cs_change = 0;
    receive.pad = 0;


    // transmit data
    retVal = ioctl(mFdSPI,SPI_IOC_MESSAGE(2),&transmit);

   if(retVal < 0){
     return errno;
   }

   return num_rx_bytes;
}

int SPIRPi::setCount(unsigned int count){

    mCount = count;
     return I_OK;
 }

 /*---------------------------------------------------------------------------
  * SPI open SPI-Bus
  *--------------------------------------------------------------------------*/
 int SPIRPi::openInterface(void * params){


     if (params == nullptr) return I_NOK;

     SPIConfig_t config = *static_cast<SPIConfig_t*>(params);
     mBits = config.bits;
     mBusSpeed = config.busSpeed;
     mInvertCS = config.invertCS;

     uint8_t const mode = SPI_MODE_0;
     mOpen = false;
     mFdSPI = open(config.deviceName,O_RDWR);

    if (mFdSPI < 0){

         return errno;
     }


     int statusVal = -1;

     if(mFdSPI< 0){
        return errno;

     }

     statusVal = ioctl (mFdSPI, SPI_IOC_RD_BITS_PER_WORD, &mBits);
     if(statusVal < 0) {
       return errno;
     }

     statusVal = ioctl (mFdSPI, SPI_IOC_WR_BITS_PER_WORD, &mBits);
     if(statusVal < 0) {
      return errno;
     }



     statusVal = ioctl (mFdSPI, SPI_IOC_WR_MODE, &mode);
     if(statusVal < 0){
         return errno;
     }

     statusVal = ioctl (mFdSPI, SPI_IOC_RD_MODE, &mode);
     if(statusVal < 0) {
       return errno;
     }



     statusVal = ioctl (mFdSPI, SPI_IOC_WR_MAX_SPEED_HZ, &mBusSpeed);
     if(statusVal < 0) {
      return errno;
     }

     statusVal = ioctl (mFdSPI, SPI_IOC_RD_MAX_SPEED_HZ, &mBusSpeed);
     if(statusVal < 0) {
       return errno;
     }


     mOpen = true;

     return I_OK;
 }

 /*---------------------------------------------------------------------------
  * SPI close SPI-Bus
  *--------------------------------------------------------------------------*/
 int SPIRPi::closeInterface(){
        close(mFdSPI);
        mOpen = false;
        return I_OK;
 }
