/* ************************************************************************** */
// NAME:    modbus_protocl.h
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU implementation for the PIC32. This module includes the 
// modbus protocol specification & registers
/* ************************************************************************** */

#ifndef MODBUSRTUPROTOCOL_H    /* Guard against multiple inclusion */
#define MODBUSRTUPROTOCOL_H

#include "modbus_common.h"
#include <stdint.h>

#define MODBUS_DATASTACK_SIZE       256

typedef struct {
	uint8_t modbus_address;
	uint8_t function_code;
	uint8_t data[MODBUS_DATASTACK_SIZE];
  uint8_t data_len;
	uint8_t crc_low;
	uint8_t crc_high;
}modbus_frame_t;

// parse function states
typedef enum {eDevice,eFuntion,eData,eCrcLow, eCrcHigh}TModbusState;

// parse byte & return true if a new valid command has been detected.
uint8_t modbus_protocol_parse_byte(uint8_t const byte);
uint8_t  modbus_protocol_parse_data(uint8_t * data,uint16_t const len);

// create a modbus rtu frame
modbus_frame_t modbus_protocol_create_frame(uint8_t const device, uint8_t  const function, uint8_t const * data, uint8_t const len);

// return last parsed frame
uint8_t modbus_protocol_get_frame(modbus_frame_t * frame);

// reset internal statemachine
void modbus_protocol_reset(void);



#endif

