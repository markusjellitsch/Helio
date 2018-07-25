/* ************************************************************************** */
// NAME:    modbus_control.h
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU Callback implementation. This modul implements the PIC32
// behaviour after a request was successfully received from the master (=jump to 
// the callback function). The function below must be given as parameter when
// initialiting the modbusRTU protocol. 
/* ************************************************************************** */


#ifndef MODBUSCONTROL_H
#define MODBUSCONTROL_H

#include <stdint.h>
#include "modbus_common.h"
#include "modbus_protocol.h"

#define MODBUS_VERSION 1


#define MODBUS_RESPONSESTACK_SIZE   255

#define BSWAP_16(x)  (x = (x >>8)|(x<<8))

typedef struct {
	uint16_t start_address;
	uint16_t num_regisers;
}read_holding_reg_payload_t;

typedef struct {
	uint16_t reg_address;
	uint16_t reg_value;
}write_single_reg_payload_t;

// read & write function pointers to the transmission interface (UART,I2C,SPI)
typedef uint8_t (*fp_modbus_read_t)(uint8_t * byte);
typedef void (*fp_modbus_send_t)(uint8_t const byte);

// this is a function pointer for a writte single register
typedef uint8_t (*fp_callback_t)(uint8_t const function_code,int16_t const value);

// function
typedef fp_callback_t modbus_callback_table_t[MODBUS_NUM_REGISTER];

// make callback table global
extern modbus_callback_table_t g_modbus_callback_table;

// initialize modbus rtu
void modbus_rtu_init(fp_modbus_send_t const fpSend, fp_modbus_read_t fpRead);

// enable or disable callback handler when a frame is parsed succesfully
void modbus_rtu_enable_callback_handler(void);
void modbus_rtu_disable_callback_handler(void);

// process data and check for valid modbus rtu command.
uint8_t modbus_rtu_process_input(uint8_t * data, uint16_t const len);

// register a write single register callback function
uint8_t modbus_rtu_register_callback(uint8_t const reg, fp_callback_t fp);

// call the internal global callback handler manually (e.g when callback handler is disabled)
uint8_t modbus_rtu_callback_handler(void);

#endif
