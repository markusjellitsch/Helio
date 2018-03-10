//------------------------------------------------------------------------
// NAME         : modbus_rtu.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Protocol implementation for modbusRTU
// -----------------------------------------------------------------------

#include "modbusrtu.h"
#include <QDebug>
#include <string.h>
#include <iostream>

//----------------------------------------------------------
// MODBUS RTU - Constructor. Connect ReadyRead()-Singal
// To the Parse function
//----------------------------------------------------------
ModbusRTU::ModbusRTU(){

        mState = eDevice;   
        mValid = false;
}

//----------------------------------------------------------------
// MODBUS RTU PARSE - This is the slot function, which is called,
// when incomin data arrive. Data is read from the SerialPort buffer.
// Then the Data is passed through the modbus protocol, implemented
// as statemachine. If the frame is correct, a signal is emitted
// (FrameReceived).
//----------------------------------------------------------------
uint8_t ModbusRTU::parseByte(uint8_t const byte){

         static uint16_t tmp = 0;
         static  uint16_t crc;
         mValid = false;

           switch (mState){

                    // ----------------------
                    // Byte 1: Device Addres
                    // ----------------------
                    case eDevice:
                        if (byte == mMyAddress){
                            mState = eFuntion;
                            mLatestFrame.slaveAddress = byte;
                            crc = 0xFFFF;
                        }

                        break;

                    // ----------------------
                    // Byte 2: Function Code
                    // ----------------------
                     case eFuntion:
                           mLatestFrame.functionCode = byte;
                           mState = eData;
                           mLatestFrame.dataLen = 0;
                         break;

                    // ----------------------
                    // Byte 3 to n-2: Data
                    // ----------------------
                     case eData:
                           mLatestFrame.data[mLatestFrame.dataLen++]=byte;
                         if (mLatestFrame.dataLen == mPayloadSize) {
                             mState = eCrcLow;
                         }
                         else if (mLatestFrame.dataLen > mPayloadSize){
                             mState = eDevice;
                         }
                         break;

                    // ----------------------
                    // Byte n-1: CRC Low Byte
                    // ----------------------
                     case eCrcLow:
                         tmp = crc;
                         tmp ^= (uint16_t)byte;
                         mLatestFrame.crcLow = byte;
                         mState = eCrcHigh;
                         break;

                    // ----------------------
                    // Byte n: CRC High Byte
                    // ----------------------
                      case eCrcHigh:

                         tmp ^= ((uint16_t)(byte)<<8);
                         mLatestFrame.crcHigh = byte;
                         mState = eDevice;
                         if (tmp ==0){
                            mValid = true;
                            return true;
                         }
                         break;


            }

           // calculate checksum
          Crc(&crc,byte);

          return false;
}

 uint8_t ModbusRTU::parseData(uint8_t * data, uint16_t const len){

     uint8_t success =0;
     for (int i = 0; i< len;i++){
        success =  parseByte(data[i]);
     }

     return success;
 }

 uint8_t ModbusRTU::getFrame(ModbusFrame_t * frame){
    memcpy(frame,&mLatestFrame,sizeof(ModbusFrame_t));
    return mValid;
 }

 void ModbusRTU::setPayloadSize(uint8_t const size){
      mPayloadSize = size;
 }


 //----------------------------------------------------------------
 // MODBUS RTU - Constructor. Connect ReadyRead()-Singal
 // To the Parse function.
 //----------------------------------------------------------------
ModbusFrame_t ModbusRTU::createFrame(uint8_t const device, uint8_t const function, uint8_t const * data, uint8_t const len){

    ModbusFrame_t tmp;

    // crc code
    uint16_t crc = 0xFFFF;

    // set device address
    tmp.slaveAddress = device;
    Crc(&crc,device);

    // set function code
    tmp.functionCode = function;
    Crc(&crc,function);

    // set data & len
    uint8_t i = 0;
    tmp.dataLen = len;
    for (i = 0; i < len;i++){
    tmp.data[i] = data[i];
    Crc(&crc,data[i]);
    }

    // set crc16 low & high
    tmp.crcLow = crc & 0xFF;
    tmp.crcHigh = crc >> 8;

    return tmp;
 }


 void ModbusRTU::setMyAddress(uint8_t const device){
     mMyAddress = device;
 }

// reset statemachine
void ModbusRTU::reset(){
     mState = eDevice;
     memset(&mLatestFrame,0,sizeof(ModbusFrame_t));
     mValid= false;
}


 //----------------------------------------------------------------
 // MODBUS RTU CRC - Calculate Checksum
 //----------------------------------------------------------------
void ModbusRTU::Crc(uint16_t * crc, uint8_t byte){
    *crc ^= (uint16_t)byte;

            int i = 0;
            // XOR byte into least sig. byte of crc
            for (i = 8; i != 0; i--) {
                if ((*crc & 0x0001) != 0) {      // If the LSB is set
                    *crc >>= 1;                    // Shift right and XOR 0xA001
                    *crc ^= 0xA001;
                }
                else                            // Else LSB is not set
                *crc >>= 1;                    // Just shift right
            }
}

