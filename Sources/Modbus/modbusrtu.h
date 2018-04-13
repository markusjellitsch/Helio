//------------------------------------------------------------------------
// NAME         : modbus_rtu.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Protocol implementation for modbusRTU
// -----------------------------------------------------------------------

#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <stdint.h>

#define MODBUS_DEFAULT_ADDDRESS     0x11

#define MODBUS_FUNC_READHOLDING     0x03
#define MODBUS_FUNC_WRITESINGLE     0x06

#define MODBUS_DATASTACK_SIZE       255

#define MODBUS_FRAME_OVERHEAD       4


typedef struct{
    uint8_t slaveAddress;
    uint8_t functionCode;
    uint8_t data[MODBUS_DATASTACK_SIZE];
    uint8_t dataLen;
    uint8_t crcHigh;
    uint8_t crcLow;
}ModbusFrame_t;


typedef struct{
    uint8_t len;
    uint16_t * data;
}ReadHoldingRegisterResponse_t;


typedef struct{
     uint16_t regAddress;
    uint16_t regValue;
}WriteHoldingRegisterResponse_t;


class ModbusRTU
{
    // States
    typedef enum{
            eDevice,
             eFuntion,
             eData,
             eCrcLow,
             eCrcHigh}
         TModbusState;


public:

    ModbusRTU();
    ~ModbusRTU()=default;

    // parsing functions
    uint8_t parseByte(uint8_t const byte);
    uint8_t parseData(uint8_t * data, uint16_t const len);

    // create a modbus frame
    ModbusFrame_t createFrame(uint8_t const device, uint8_t const function, uint8_t const * data, uint8_t const len);

    // return last frame
    uint8_t getFrame(ModbusFrame_t * frame);

    // set payload for receiving frame
    void setPayloadSize(uint8_t const size);

    // set own address to identify frame
    void setMyAddress(uint8_t const size);

    // reset statemachine
    void reset();

private:

    // calculate checksum
    void Crc(uint16_t * crc, uint8_t byte);

    // members
    uint8_t mMyAddress = MODBUS_DEFAULT_ADDDRESS;
    uint8_t mPayloadSize = 8;

    ModbusFrame_t mLatestFrame;
    bool mValid = false;

    // current state
    ModbusRTU::TModbusState mState;
};

#endif // MODBUSRTU_H
