//------------------------------------------------------------------------
// NAME         : i2crpi.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : This is the implementation for the raspberry i2c interface
// -----------------------------------------------------------------------

#include "i2crpi.h"
#include "linux/i2c-dev.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



/*---------------------------------------------------------------------------
 * I²C Write Function to write data on the I²C-Bus.
 *--------------------------------------------------------------------------*/
int I2CRPi::writeMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     if (buffer == nullptr || numBytes ==0 || !mOpen) return I_NOK;

     // create the message
     i2c_msg message;
     message.addr = slaveAddress;
     message.len = numBytes;
     message.flags = 0;         // write command
     message.buf = (char *)buffer;

     // create whole data frame
     i2c_rdwr_ioctl_data data;
     data.msgs = &message;
      data.nmsgs = 1;

     // write to the I²C-Bus
     int success = ioctl(mFdI2C,I2C_RDWR,&data);

     //
     if (success < 0) return errno;

     return I_OK;
 }

/*---------------------------------------------------------------------------
 * I²C Function to read data from the I²C-Bus.
 *--------------------------------------------------------------------------*/
 int I2CRPi::readMulti(unsigned char const slaveAddress,unsigned char * buffer, size_t const numBytes){

     if (buffer == nullptr || numBytes == 0 || !mOpen) return I_NOK;

     // create message
     i2c_msg message;
     message.addr = slaveAddress;
     message.len = numBytes;
     message.flags = I2C_M_RD | I2C_M_NOSTART;  // read command
     message.buf = (char *) buffer;

     i2c_rdwr_ioctl_data data;
     data.msgs = &message;
     data.nmsgs = 1;

     int success = ioctl(mFdI2C,I2C_RDWR,&data);

     if (success < 0) return errno;

     return I_OK;

 }

 /*---------------------------------------------------------------------------
  * I²C open I²C-Bus
  *--------------------------------------------------------------------------*/
 int I2CRPi::openInterface(void  * params){

     if (params == nullptr) return I_NOK;

     // cast params
     char * filename = static_cast<char *>(params);

     mOpen = false;

     mFdI2C = open(filename,O_RDWR);

     if (mFdI2C < 0){

         return errno;
     }

     mOpen = true;

     return I_OK;
 }

 /*---------------------------------------------------------------------------
  * I²C close I²C-Bus
  *--------------------------------------------------------------------------*/
 int I2CRPi::closeInterface(){
        close(mFdI2C);
        mOpen = false;
        return I_OK;
 }
