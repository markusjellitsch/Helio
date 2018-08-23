/* ************************************************************************** */
// NAME:    modbus_register.h
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU holdng regiter
/* ************************************************************************** */

#ifndef MODBUSRTUREG_H    /* Guard against multiple inclusion */
#define MODBUSRTUREG_H

#include <stdint.h>
#include "modbus_common.h"

#define MODBUS_NUM_LIMITS   2

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

// holding registers
typedef int16_t holding_register_t[MODBUS_NUM_REGISTER];

// make holdings globals for speed access. Make sure that register are accessed atomic
extern holding_register_t g_holding_reg_table;
extern uint16_t  g_modbus_readonly_table[MODBUS_NUM_REGISTER];


// Read & Write functions with ATOMIC ACCESS
uint8_t modbus_register_set_value(uint16_t const reg, int16_t const value);
uint8_t modbus_register_read_value(uint16_t const reg,int16_t * value);

// Set Limit for a Holding Register
uint8_t  modbus_register_set_limit(uint16_t const reg,int16_t const min, int16_t const max);

uint8_t  modbus_register_set_readonly(uint16_t const reg,uint8_t const readonly);

// Take high & low and make return int16_t
int16_t modbus_register_make_int16(uint8_t const low, uint8_t high);

#endif
