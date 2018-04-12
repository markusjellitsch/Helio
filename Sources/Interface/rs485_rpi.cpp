//------------------------------------------------------------------------
// NAME         : uart_rpi.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry UART interface
// -----------------------------------------------------------------------

#include "rs485_rpi.h"
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
#include <wiringPi.h>

using namespace std;


/*---------------------------------------------------------------------------
 * Constructor
 *--------------------------------------------------------------------------*/
RS485Rpi::RS485Rpi(){

    for (int i = 0; i< RS485_RPI_BUFFERSIZE;i++){
        mRxBuffer[i] = 0;
        mTxBuffer[i] = 0;
    }

    wiringPiSetup();
    pinMode(RS485_RPI_DI_PIN,OUTPUT);
    digitalWrite(RS485_RPI_DI_PIN,0);
}


/*---------------------------------------------------------------------------
 * RS485 Write Function to write data on the RS485-Bus.
 *--------------------------------------------------------------------------*/
int RS485Rpi::writeMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     memcpy(mTxBuffer,buffer,numBytes);

     // set DI-pin
     digitalWrite(RS485_RPI_DI_PIN,1);

     int count = write(mFdRS485,mTxBuffer,numBytes);

     // wait until tx buffer is flushed.
     int success = tcdrain(mFdRS485);
     if (success!=0){
        return I_NOK;
     }

     digitalWrite(RS485_RPI_DI_PIN,0);

     if (count < 0) return I_NOK;

     return I_OK;
 }

int RS485Rpi::sendMessage(std::string const & message, unsigned int const numBytes){

    unsigned char buffer[RS485_RPI_BUFFERSIZE];
    memcpy(buffer,message.c_str(),numBytes);


   return  writeMulti(0,buffer,numBytes);
}


/*---------------------------------------------------------------------------
 * RS485 Function to read data from the RS485-Bus.
 *--------------------------------------------------------------------------*/
 int RS485Rpi::readMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     slaveAddress;
     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     unsigned int len = numBytes;
     if (numBytes > RS485_RPI_BUFFERSIZE) len = RS485_RPI_BUFFERSIZE;

     int count = -1;

     //while (count == -1){
        count = read(mFdRS485,(void *)mRxBuffer,len);
     //}

     if (count <0) return I_NOK;

     memcpy(buffer,mRxBuffer,count);

     return count;
 }

void RS485Rpi::printf(const unsigned char * format,...){

    unsigned char buf[RS485_RPI_BUFFERSIZE];

    // get argumentes and convert to snptrintf
    va_list args;
    va_start (args, format);
    int len = vsnprintf((char *)buf,RS485_RPI_BUFFERSIZE,(char *)format,args);
    writeMulti(0,buf,len);
    va_end (args);
}


 /*---------------------------------------------------------------------------
  * RS485 open
  *--------------------------------------------------------------------------*/
 int RS485Rpi::openInterface(void * params){


     if (params == nullptr) return I_NOK;

     RS485Config_t config = *static_cast<RS485Config_t*>(params);

    mFdRS485 =open(config.deviceName, O_WRONLY | O_NOCTTY |O_SYNC);

    if (mFdRS485 < 0){

         return errno;
     }

    struct termios options;
    if (config.baudRate == RS485_RPI_BAUD19200)
        options.c_cflag = B19200;
    else options.c_cflag = B115200;
    options.c_cflag |= CS8 | CLOCAL| CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 8;
    tcflush(mFdRS485,TCIFLUSH);
    int success = tcsetattr(mFdRS485,TCSANOW,&options);

    if (success != 0) return I_NOK;

    mOpen = true;

    return I_OK;
 }

 /*---------------------------------------------------------------------------
  * RS485 close
  *--------------------------------------------------------------------------*/
 int RS485Rpi::closeInterface(){
        close(mFdRS485);
        mOpen = false;
        return I_OK;
 }
