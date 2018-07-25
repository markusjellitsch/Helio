/* ************************************************************************** */
// NAME:    modbus_protocl.c
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU implementation for the PIC32. This module includes the 
// modbus protocol specification & registers
/* ************************************************************************** */

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>
#include <string.h>
#include "modbus_register.h"
#include "modbus_protocol.h"


// modbus state
static TModbusState m_modbus_state = eDevice;

static modbus_frame_t latest_frame;

// data buffer for incoming bytes
static uint8_t m_modbus_data_stack[MODBUS_DATASTACK_SIZE] = {0};
static uint8_t m_idx_data = 0;


// Checksum calculation (crc16)
void ModbusRTU_Crc(uint16_t * crc, uint8_t byte);


//---------------------------------------------------
// MODBUS RTU Parse - State Machine for verifying the
// protocol specification. If the received frame is
// valid (= CRC correct) then callback function is called.
// Finally the response is send back to the other device
//----------------------------------------------------
uint8_t modbus_protocol_parse_byte(uint8_t const byte){
    
    static uint8_t function = 0;
    static uint16_t crc = 0xFFFF;
    static uint8_t len = 0;
    static uint16_t tmp = 0;
    
    switch (m_modbus_state){
        
        // ----------------------
        // Byte 1: Device Addres
        // ----------------------
        case eDevice:
            if (byte == MODBUS_MY_DEVICEADDRESS){
                m_modbus_state = eFuntion;
                function = 0;
                crc = 0xFFFF;
                m_idx_data = 0;
            }
            break;
          
        // ----------------------
        // Byte 2: Function Code
        // ----------------------   
         case eFuntion:
             
             // check function code
             if (!MODBUS_VALID_FUNC(byte)){
               m_modbus_state = eDevice;
               break;
             }
             
             else  len = MODBUS_DATALEN_DEFAULT;
             function = byte;
             m_modbus_state = eData;
             
             break;
         
        // ----------------------
        // Byte 3 to n-2: Data 
        // ---------------------- 
         case eData:
             m_modbus_data_stack[m_idx_data++] = byte;
             if (m_idx_data == len || m_idx_data >= MODBUS_DATASTACK_SIZE) {
                 m_modbus_state = eCrcLow;
             }
             else if (m_idx_data > len) m_modbus_state = eDevice;
             break;
         
        // ----------------------
        // Byte n-1: CRC Low Byte 
        // ---------------------- 
         case eCrcLow:
             tmp = crc;

             tmp ^= (uint16_t)byte;
             m_modbus_state = eCrcHigh;
             break;
          
        // ----------------------
        // Byte n: CRC High Byte 
        // ---------------------- 
          case eCrcHigh:

              tmp ^= ((uint16_t)(byte)<<8);
              if (tmp ==0){ 
                 
                 // process the function (read/write holding)
                 //if (ModbusRTU_Process(function)){;
                 
                    // send response back 
                    //ModbusRTU_SendResponse(function);
									 
									  //if (m_fp_clbk != 0) m_fp_clbk(function,m_idx_data);
									  latest_frame.modbus_address = MODBUS_MY_DEVICEADDRESS;
								    latest_frame.function_code = function;
									  latest_frame.data_len = m_idx_data;
								    memcpy(latest_frame.data,m_modbus_data_stack,m_idx_data);
										
                 //}
                 //else ModbusRTU_SendError(function);
                 m_modbus_state = eDevice;
                 return true;
             }
             m_modbus_state = eDevice;
             return false; 

    }
       
       // calculate checksum
       ModbusRTU_Crc(&crc,byte);
		  return false;
}

uint8_t modbus_protocol_parse_data(uint8_t * data,uint16_t const len){
	
	uint8_t success = 0;
	for (uint16_t i = 0; i<len;i++){
			success = modbus_protocol_parse_byte(data[i]);
	}
	
	return success;

}

void modbus_protocol_reset(){
			m_modbus_state = eDevice;
}

uint8_t modbus_protocol_get_frame(modbus_frame_t * frame){
			
	memcpy(frame,&latest_frame,sizeof(modbus_frame_t));

	return 1;
}

//---------------------------------------------------
// MODBUS RTU Create Frame - Base Function to create
// a modbusRTU frame including checksum (crc16) at the
// end. The Sending interfac (uart,rs485..) is set by
// initialization. 
//----------------------------------------------------
modbus_frame_t modbus_protocol_create_frame(uint8_t const device, uint8_t const function, uint8_t const * data, uint8_t const len){
   
		modbus_frame_t tmp;
		tmp.data_len = 0;
		tmp.crc_high = 0;
		tmp.crc_low = 0;
		tmp.function_code = 0;

	 if (data == NULL) return tmp;
    
    uint16_t crc = 0xFFFF;
    
    // send device id
    tmp.modbus_address = device;
    ModbusRTU_Crc(&crc,device);
    
    // send function
    tmp.function_code=function;
    ModbusRTU_Crc(&crc,function);
    
    // send data
    uint8_t i = 0;
	  tmp.data_len = len;
		memcpy(tmp.data,data,len);
    for (i = 0; i < len;i++){
         ModbusRTU_Crc(&crc,data[i]);
    }
    
    // send crc16 (low,high)
    tmp.crc_low = crc & 0xFF;
    tmp.crc_high =crc >> 8;
		
		return tmp; 
}
 

void ModbusRTU_Crc(uint16_t * crc, uint8_t byte)
{
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
