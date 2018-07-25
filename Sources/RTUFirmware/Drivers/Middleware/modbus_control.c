/* ************************************************************************** */
// NAME:    modbus_control.h
// AUTHOR:  Markus Jellitsch
// DATE:    19.7.2016
// DESCRIPTION: ModbusRTU Callback implementation. This modul implements the PIC32
// behaviour after a request was successfully received from the master (=jump to 
// the callback function). The function below must be given as parameter when
// initialiting the modbusRTU protocol. 
/* ************************************************************************** */

#include <stddef.h>                    
#include <stdbool.h>                    
#include <stdlib.h>
#include "string.h"
#include "modbus_common.h"
#include "modbus_register.h"
#include "modbus_protocol.h"
#include "modbus_control.h"


// global callback table
modbus_callback_table_t g_modbus_callback_table ={0}; 

// current frame 
static modbus_frame_t m_new_frame;
static modbus_frame_t m_response_frame;

// response buffer for response bytes 
static uint8_t m_modbus_response_stack[MODBUS_RESPONSESTACK_SIZE] = {0};
static uint8_t m_idx_response = 0;

// callback function pointers
static fp_modbus_send_t m_fp_send = NULL;
static fp_modbus_read_t m_fp_read = NULL;

// flag to indicate if callback handler is used after a valid command
static uint8_t m_callback_handler_enabled = 0;

// Forward declaration
static bool modbus_rtu_validate(void);
static bool modbus_rtu_validate_read_holding_register(void);
static bool modbus_rtu_validate_write_single_register(void);
static inline uint8_t modbus_rtu_pop_data_byte(void);
static inline void modbus_rtu_push_response_byte(uint8_t const byte); 
static void modbus_rtu_send_response(uint8_t const function);
static void modbus_rtu_send_error(uint8_t const function);

void modbus_rtu_init(fp_modbus_send_t const fp_send,fp_modbus_read_t fp_read){
			
	    m_fp_send = fp_send,
	    m_fp_read = fp_read;
	    
	    // by default callback handler is enabled
	    m_callback_handler_enabled = 1;
}



/* -----------------------------------------------------------------
// This function validates input data for a valid modbus frame
 -----------------------------------------------------------------*/
uint8_t modbus_rtu_process_input(uint8_t * data, uint16_t const len){
	  
	 memset(&m_new_frame,0,sizeof(modbus_frame_t));
	
	 // check if input is a valid frame
	 if(modbus_protocol_parse_data(data,len)){
		 modbus_protocol_get_frame(&m_new_frame);
		
		// validate frame for correct payload
		if (modbus_rtu_validate()){
				 
				 // when validation is correct, send response back to master
				 modbus_rtu_send_response(m_new_frame.function_code);
				 
				 // callback function
				 if (m_callback_handler_enabled){
					 modbus_rtu_callback_handler();
				 }
				return true;
		}
		else { 
				// when validation is not succesful an error with error info is sent back to master
			  modbus_rtu_send_error(m_new_frame.function_code);
				return false;
		}
	 }
	 return false;
 }


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// MODBUS FUNCTION IMPLEMENTATION: Following functions for modbus are 
//implemented:  -> READ HOLDING REGISTER (0x03)
//              -> WRITE SINGLE REGISTER (0x06)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool modbus_rtu_validate(){
    
    switch (m_new_frame.function_code){
         
			   // validate function 0x03 (read holding register)
        case MODBUS_FUNC_READHOLDINGREGISTER:
            return modbus_rtu_validate_read_holding_register();
        
				// validate function 0x06 (write single register
        case MODBUS_FUNC_WRITESINGLEREGISTER:
            return modbus_rtu_validate_write_single_register();
        
        // function not implemented => return ILLEGAL_FUNCTION
        default: 
            modbus_rtu_push_response_byte(MODBUS_EXCP_ILLEGAL_FUNCTION);
            return false;
    }
}


/* ----------------------------------------------------  */ 
/* +++++++++++++ READ HOLDING REGISTER +++++++++++++++   */ 
/* ----------------------------------------------------  */
/* | FUNCTION | HREGADDR | LREGADDR | HNUMREG | LNUMREG  */ 
/* ----------------------------------------------------- */

bool modbus_rtu_validate_read_holding_register(){
      
      int16_t regStart = 0;
      int16_t regEnd = 0;

      uint8_t byteLow = 0;
      uint8_t byteHigh = 0;
      
      uint8_t valLow = 0;
      uint8_t valHigh = 0;
      int16_t value = 0;
      
      uint8_t len  = 0;
      int16_t i = 0;
      
      // read start address
      byteHigh = modbus_rtu_pop_data_byte();
      byteLow = modbus_rtu_pop_data_byte();
      regStart = modbus_register_make_int16(byteLow,byteHigh);
      
      // read len
      byteHigh = modbus_rtu_pop_data_byte();
      byteLow = modbus_rtu_pop_data_byte();
      len= modbus_register_make_int16(byteLow,byteHigh);
      
      regEnd = regStart+len;
          
      // send len
      len = len*2;
      modbus_rtu_push_response_byte(len);
      
      for (i=regStart; i<regEnd;i++){
      
          // read value from holding
          modbus_register_read_value((uint8_t)i,&value);
          valLow = value & 0xFF;
          valHigh = value >> 8;
          
          // push data on response buffer
          modbus_rtu_push_response_byte(valHigh);
          modbus_rtu_push_response_byte(valLow);
      }
      
      return true;
}

/* ----------------------------------------------------  */ 
/* +++++++++++++ WRITE SINGLE REGISTER +++++++++++++++   */ 
/* ----------------------------------------------------  */
/* | FUNCTION | HREGADDR | LREGADDR | HVALUE | LVALUE    */ 
/* ----------------------------------------------------- */
bool modbus_rtu_validate_write_single_register(){
	
    
    uint8_t regLow = 0;
    uint8_t regHigh = 0;
    uint8_t reg = 0;
    
    uint8_t valLow = 0;
    uint8_t valHigh = 0;
    int16_t value = 0;
    
    uint8_t error = MODBUS_OK;
    
    // read address
    regHigh = modbus_rtu_pop_data_byte();
    regLow = modbus_rtu_pop_data_byte();
    reg = modbus_register_make_int16(regLow,regHigh);
    
    // read value
    valHigh = modbus_rtu_pop_data_byte();
    valLow = modbus_rtu_pop_data_byte();
    value =  modbus_register_make_int16(valLow,valHigh);
    
    // set value in holding reg
    error = modbus_register_set_value((uint8_t)reg,value);
    
    if (error == MODBUS_OK){        
        
        modbus_rtu_push_response_byte(regHigh); 
        modbus_rtu_push_response_byte(regLow);
    
        modbus_rtu_push_response_byte(valHigh); 
        modbus_rtu_push_response_byte(valLow);
        
        return true;
    }
    else if  (error == MODBUS_EXCP_ILLEGAL_DATAADDRESS){
        modbus_rtu_push_response_byte(MODBUS_EXCP_ILLEGAL_DATAADDRESS);
        return false;
    }
    
    else if (error == MODBUS_EXCP_ILLEGAL_DATAVALUE){
        modbus_rtu_push_response_byte(MODBUS_EXCP_ILLEGAL_DATAVALUE);
        return false;
    }
    
    return false;
}


//---------------------------------------------------
// Send function. This function sends the data to the
// specified serial interface (UART,SPI,I2C,etc.)
//----------------------------------------------------
uint8_t modbus_rtu_send_frame(){
		if (m_fp_send == 0 || (m_response_frame.data_len == 0)) return 0;
	
		m_fp_send(m_response_frame.modbus_address);
		m_fp_send(m_response_frame.function_code);
	  for (uint8_t i = 0;i< m_response_frame.data_len;i++){ 
	    m_fp_send(m_response_frame.data[i]);
	  }
	  
		m_fp_send(m_response_frame.crc_low);
		m_fp_send(m_response_frame.crc_high);

		return true;
}


//---------------------------------------------------
// MODBUS RTU SENDRESPONSE - This function sends a
// modbusRTU frame back to the modbus master. Data which
// has to be transmitted is in the response buffer
//----------------------------------------------------
void modbus_rtu_send_response(uint8_t const function){
	m_response_frame = modbus_protocol_create_frame(MODBUS_MY_DEVICEADDRESS,function,m_modbus_response_stack,m_idx_response);
	modbus_rtu_send_frame();
  m_idx_response = 0;
	m_response_frame.data_len = 0;
}

//---------------------------------------------------
// MODBUS RTU SEND ERROR - If an error occurs (e.g invalid 
// register address, invalid data) error is sent back to
// the modbus master. For error the MSB (0x80) is set.
// The corresponding error code is sent via response buffer
//----------------------------------------------------
void modbus_rtu_send_error(uint8_t const function){
    m_response_frame = modbus_protocol_create_frame(MODBUS_MY_DEVICEADDRESS,(function | MODBUS_ERRORCODE),m_modbus_response_stack,1);
    modbus_rtu_send_frame();
	  m_idx_response = 0;
	  m_response_frame.data_len = 0;
}

//---------------------------------------------------
// MODBUS RTU PUSHR - Send a byte to the Response Buffer
//----------------------------------------------------
inline void modbus_rtu_push_response_byte(uint8_t const byte){
    m_modbus_response_stack[m_idx_response++] = byte;
    m_idx_response = m_idx_response % MODBUS_RESPONSESTACK_SIZE;
}


//---------------------------------------------------------
// MODBUS RTU POPD - Read first byte from the Data Stack (FIFO)
//---------------------------------------------------------
static uint8_t modbus_rtu_pop_data_byte(){
    static uint8_t idx = 0;
  
    uint8_t tmp = m_new_frame.data[idx++];
    
    if (idx == m_new_frame.data_len) idx = 0;
    
    return tmp;
}

//-------------------------------------------------------------------------
// MODBUS RTU CALLBACK FUNCTION - This function is set as function pointer
// when initialiting ModbusRTU. When a Modbus frame has been successfully 
// received, this function is called. Within this function, the function code
// (e.g WRITE_SINGLE_REGISTER, READ_HOLDING_REGISTER) is evaluated. For
// example, if a WRITE_SINGLE_REGISTER to RGB_LED_COLOR (Reg 4002) has happend,
// this function sets the RGB color appropriateley. In fact, this function, is
// the control unit between modbusRTU & the pic32mx system. :-)
// -------------------------------------------------------------------------s
uint8_t modbus_rtu_callback_handler(){
	

	write_single_reg_payload_t * tmp = ((write_single_reg_payload_t*)(m_new_frame.data));
	BSWAP_16(tmp->reg_address);
	BSWAP_16(tmp->reg_value);
	
	if (tmp->reg_address >= MODBUS_NUM_REGISTER) return 0;
	int error = 0;

	switch (m_new_frame.function_code){
	
		case MODBUS_FUNC_READHOLDINGREGISTER:
			// TODO
		break;
		
		case MODBUS_FUNC_WRITESINGLEREGISTER:
				
		  // callback table call
		  if (g_modbus_callback_table[tmp->reg_address]!=0){
				error = g_modbus_callback_table[tmp->reg_address](m_new_frame.function_code, tmp->reg_value);
			}
			break;
	
		default:
			break;
	}
	
	return error;
}

uint8_t modbus_rtu_register_callback(uint8_t const reg, fp_callback_t fp){
				
	  if (reg >=MODBUS_NUM_REGISTER) return 0;
	  g_modbus_callback_table[reg] = fp;
		return 1;
}

void modbus_rtu_enable_callback_handler(){
	m_callback_handler_enabled = 1;
}

void modbus_rtu_disable_callback_handler(){
	m_callback_handler_enabled = 0;
}



			 


