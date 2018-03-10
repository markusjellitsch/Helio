//------------------------------------------------------------------------
// NAME         : uart_rpi.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry UART interface
// -----------------------------------------------------------------------

#include "uart_rpi.h"
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <termios.h>
#include <stdarg.h>
#include <stdio.h>

using namespace std;


/*---------------------------------------------------------------------------
 * Constructor
 *--------------------------------------------------------------------------*/
UARTRpi::UARTRpi(){

    for (int i = 0; i< UART_RPI_BUFFERSIZE;i++){
        mRxBuffer[i] = 0;
        mTxBuffer[i] = 0;
    }
}


/*---------------------------------------------------------------------------
 * UART Write Function to write data on the UART-Bus.
 *--------------------------------------------------------------------------*/
int UARTRpi::writeMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     memcpy(mTxBuffer,buffer,numBytes);

     int count = write(mFdUART,mTxBuffer,numBytes);

     if (count < 0) return I_NOK;

     return I_OK;
 }

int UARTRpi::sendMessage(std::string const & message, unsigned int const numBytes){

    unsigned char buffer[UART_RPI_BUFFERSIZE];
    memcpy(buffer,message.c_str(),numBytes);


   return  writeMulti(0,buffer,numBytes);
}


/*---------------------------------------------------------------------------
 * UART Function to read data from the UART-Bus.
 *--------------------------------------------------------------------------*/
 int UARTRpi::readMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     slaveAddress;
     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     unsigned int len = numBytes;
     if (numBytes > UART_RPI_BUFFERSIZE) len = UART_RPI_BUFFERSIZE;

     int count = -1;

     //while (count == -1){
        count = read(mFdUART,(void *)mRxBuffer,len);
     //}

     if (count <0) return I_NOK;

     memcpy(buffer,mRxBuffer,count);

     return count;
 }

void UARTRpi::printf(const unsigned char * format,...){

    unsigned char buf[UART_RPI_BUFFERSIZE];

    // get argumentes and convert to snptrintf
    va_list args;
    va_start (args, format);
    int len = vsnprintf((char *)buf,UART_RPI_BUFFERSIZE,(char *)format,args);
    writeMulti(0,buf,len);
    va_end (args);
}


 /*---------------------------------------------------------------------------
  * UART open
  *--------------------------------------------------------------------------*/
 int UARTRpi::openInterface(void * params){


     if (params == nullptr) return I_NOK;

     UARTConfig_t config = *static_cast<UARTConfig_t*>(params);

    mFdUART =open(config.deviceName, O_RDWR | O_NOCTTY);

    if (mFdUART < 0){

         return errno;
     }

    struct termios options;
    if (config.baudRate == UART_RPI_BAUD19200)
        options.c_cflag = B19200;
    else options.c_cflag = B115200;
    options.c_cflag |= CS8 | CLOCAL| CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 8;
    tcflush(mFdUART,TCIFLUSH);
    int success = tcsetattr(mFdUART,TCSANOW,&options);

    if (success != 0) return I_NOK;

    mOpen = true;

    return I_OK;
 }

 /*---------------------------------------------------------------------------
  * UART close
  *--------------------------------------------------------------------------*/
 int UARTRpi::closeInterface(){
        close(mFdUART);
        mOpen = false;
        return I_OK;
 }
