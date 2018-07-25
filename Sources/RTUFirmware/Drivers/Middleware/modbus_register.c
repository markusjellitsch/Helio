/* ************************************************************************** */
// NAME:    modbus_register.c
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU holding register
/* ************************************************************************** */

#include <stddef.h>                    
#include <stdbool.h>                    
#include <stdlib.h>
#include "modbus_register.h"

// global  holding register
holding_register_t g_holding_reg_table = {0};
uint16_t  g_modbus_readonly_table[MODBUS_NUM_REGISTER] ={0};
int16_t g_modbus_limit_table[MODBUS_NUM_REGISTER][2];


// ----------------------------------------------
// MODBUS WRITE - Write to the holding register
// ----------------------------------------------
uint8_t modbus_register_set_value(uint16_t const reg, int16_t const value){
    
    // address check
    if (reg >= MODBUS_NUM_REGISTER) return MODBUS_EXCP_ILLEGAL_DATAADDRESS;
    
		
	// check if register has write protection
	 if (g_modbus_readonly_table[reg]) return MODBUS_EXCP_ILLEGAL_DATAVALUE;
	
    // value check
    int16_t min = g_modbus_limit_table[reg][0];
    int16_t max = g_modbus_limit_table[reg][1];
    if ((max) && ((value < min) || (value > max))) {
        
        return MODBUS_EXCP_ILLEGAL_DATAVALUE;
    }
    
    // write value
    g_holding_reg_table[reg] = value;
    
  
    return MODBUS_OK;
}

// ----------------------------------------------
// MODBUS READ - Read from the holding register
// ----------------------------------------------
uint8_t modbus_register_read_value(uint16_t const reg,int16_t * value){
    
    // address check
    if (reg >= MODBUS_NUM_REGISTER) return MODBUS_EXCP_ILLEGAL_DATAADDRESS;
    
      
    // read value
    if(value != NULL) *value = g_holding_reg_table[reg];
    
    
    return MODBUS_OK;
}


// ----------------------------------------------
// MODBUS MAKE INT16 - Take low & high byte and
// make a int16_t register value
// ----------------------------------------------
int16_t modbus_register_make_int16(uint8_t const low, uint8_t high){
    uint16_t tmp = (uint16_t)(high) << 8;
    return (int16_t)(tmp|low);
}

// ----------------------------------------------
// MODBUS SET LIMIT - Set a limit for the holding
// register with min and max value
// ----------------------------------------------
uint8_t  modbus_register_set_limit(uint16_t const reg,int16_t const min, int16_t const max){

    if (reg >= MODBUS_NUM_REGISTER) return MODBUS_EXCP_ILLEGAL_DATAADDRESS;
    if (min > max) return MODBUS_EXCP_ILLEGAL_DATAVALUE;
    
    g_modbus_limit_table[reg][0] = min;
    g_modbus_limit_table[reg][1] = max;
    
    return MODBUS_OK;
}

// ----------------------------------------------
// MODBUS READ ONLYS - Set register readonly
// ----------------------------------------------
uint8_t  modbus_register_set_readonly(uint16_t const reg,uint8_t const readonly){
		
	 if (reg >= MODBUS_NUM_REGISTER) return 0;
	
	 // set write protection
	 if (readonly){
			g_modbus_readonly_table[reg] = 1;
	 }
	 else g_modbus_readonly_table[reg] = 0;
	
	return MODBUS_OK;
}


