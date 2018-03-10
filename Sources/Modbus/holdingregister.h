//------------------------------------------------------------------------
// NAME         : holdingregister.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Holding registers for modbusRTU
// -----------------------------------------------------------------------

#ifndef MODBUSHOLDINGREGISTER_H
#define MODBUSHOLDINGREGISTER_H

#define HOLDING_START_REG 4001
#define HOLDING_REGISTER_DEFAULT_SIZE 25

#include <stdint.h>
#include <string>

#define MODBUS_OK                                   0
#define MODBUS_NOK                                  1

#define MODBUS_HOLDING_REG_4001 					0
#define MODBUS_HOLDING_REG_4002 					1
#define MODBUS_HOLDING_REG_4003 					2
#define MODBUS_HOLDING_REG_4004 					3
#define MODBUS_HOLDING_REG_4005 					4
#define MODBUS_HOLDING_REG_4006 					5
#define MODBUS_HOLDING_REG_4007 					6
#define MODBUS_HOLDING_REG_4008 					7
#define MODBUS_HOLDING_REG_4009 					8
#define MODBUS_HOLDING_REG_4010 					9
#define MODBUS_HOLDING_REG_4011 					10
#define MODBUS_HOLDING_REG_4012 					11
#define MODBUS_HOLDING_REG_4013 					12
#define MODBUS_HOLDING_REG_4014 					13
#define MODBUS_HOLDING_REG_4015 					14
#define MODBUS_HOLDING_REG_4016 					15
#define MODBUS_HOLDING_REG_4017 					16
#define MODBUS_HOLDING_REG_4018 					17
#define MODBUS_HOLDING_REG_4019 					18
#define MODBUS_HOLDING_REG_4020 					19
#define MODBUS_HOLDING_REG_4021 					20
#define MODBUS_HOLDING_REG_4022 					21
#define MODBUS_HOLDING_REG_4023 					22
#define MODBUS_HOLDING_REG_4024 					23
#define MODBUS_HOLDING_REG_4025 					24

#define MODBUS_ERROR_OUT_OF_BOUNDS                                      -2
#define MODBUS_ERROR_NULLPTR                                            -1



// SWAP function for uint16_t
#define SWAP16(x)                       \
{                                       \
 int16_t tmp = x >> 8;                  \
  x = x << 8;                           \
  x|=tmp;                               \
}

// holding reg struct
typedef struct{
    int16_t value;
    std::string name;
    std::string description;
    uint8_t readonly;
}holding_register_t;


class ModbusHoldingRegister
{
public:

    // constructor & desctructor
    ModbusHoldingRegister(int const size=HOLDING_REGISTER_DEFAULT_SIZE);
    ~ModbusHoldingRegister();

    // read & write function
    int setValue(int16_t const reg, int const value);
    int getValue(int16_t const reg, int16_t * value);

    // set description & name of a reg
    int setName(int16_t const reg,std::string const & name);
    int setDescription(int16_t const reg,std::string const & description);

    // print all registers on screen
    void dump();

private:
    // member
    holding_register_t * mRegisters = nullptr;
    int16_t mNumRegs = HOLDING_REGISTER_DEFAULT_SIZE;
};

#endif // MODBUSHOLDINGREGISTER_H
