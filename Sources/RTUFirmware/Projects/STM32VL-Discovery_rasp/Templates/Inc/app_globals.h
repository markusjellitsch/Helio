/* ************************************************************************** */
// NAME:    app_globals.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: Application specific global variables & definitions
/* ************************************************************************** */

#ifndef APP_GLOBALS_H
#define APP_GLOBALS_H

// standard library
#include "stdint.h"
#include "string.h"

// stm32 library
#include "stm32f1xx_hal.h"

// board support librareis
#include "stm32vl_discovery.h"
#include "bsp_uart1.h"
#include "bsp_tim1_pwm.h"
#include "bsp_tim2_events.h"
#include "bsp_tim3_ic.h"
#include "bsp_spi1_dma.h"

// middleware
#include "modbus_protocol.h"
#include "modbus_register.h"
#include "modbus_control.h"

// debug
#include "debug_print.h"

/* UART1 Definitions */
#define UART1_BAUDRATE_DEFAULT 			115200



#define EVENT_PWM1						0
#define EVENT_PWM2						1
#define EVENT_PWM3						2

#define EVENT_CNT1_FINISHED  0
#define EVENT_CNT2_FINISHED  1
#define EVENT_CNT3_FINISHED  2
#define EVENT_CNT4_FINISHED  3

#define RTU_SYS_BASE_REG		(MODBUS_HOLDING_REG_4001)
#define RTU_SYS_CR1					(RTU_SYS_BASE_REG)				
#define RTU_SYS_EN					(RTU_SYS_BASE_REG + 1) 					
#define RTU_SYS_SR					(RTU_SYS_BASE_REG + 2)
#define RTU_SYS_BOOT				(RTU_SYS_BASE_REG + 3) 	

#define RTU_PWM_BASE 				(MODBUS_HOLDING_REG_4006) 
#define RTU_PWM_CR 					(RTU_PWM_BASE) 							// control register
#define RTU_PWM_SR				  (RTU_PWM_BASE + 1)					// status register
#define RTU_PWM_PRD				  (RTU_PWM_BASE + 2)					// period register
#define RTU_PWM_CNT				  (RTU_PWM_BASE + 3)					// period register
#define RTU_PWM_PSC				  (RTU_PWM_BASE + 4)					// period register
#define RTU_PWM_DUT				  (RTU_PWM_BASE + 5)					// period register

#define RTU_CNT_BASE				(MODBUS_HOLDING_REG_4012) 
#define RTU_CNT_PRD 					(RTU_CNT_BASE ) 					
#define RTU_CNT_SR					(RTU_CNT_BASE + 1)  					
#define RTU_CNT_CH1 				(RTU_CNT_BASE + 2)  					
#define RTU_CNT_CH2 				(RTU_CNT_BASE + 3)  					
#define RTU_CNT_CH3 				(RTU_CNT_BASE + 4)  					
#define RTU_CNT_CH4 				(RTU_CNT_BASE + 5)  	
#define RTU_CNT_RES 				(RTU_CNT_BASE + 6)  	

#define RTU_DAC_BASE				(MODBUS_HOLDING_REG_4020) 
#define RTU_DAC_CR					(RTU_DAC_BASE)					
#define RTU_DAC_SR					(RTU_DAC_BASE + 1)	
		



typedef struct{
 __IO int16_t CR1;
 __IO int16_t EN;
 __IO int16_t SR;
 __IO int16_t BOOT;
}rtu_sys_t;

#define RTU_SYS			((rtu_sys_t*)(g_holding_reg_table + MODBUS_HOLDING_REG_4001))

typedef struct{
 __IO int16_t CR;
 __IO int16_t SR;
 __IO int16_t PRD;
 __IO int16_t CNT;
 __IO int16_t PSC;
 __IO int16_t DUR;
}rtu_pwm_t;

#define RTU_PWM			((rtu_pwm_t*)(g_holding_reg_table + MODBUS_HOLDING_REG_4006))

typedef struct{
 __IO int16_t PRD;
 __IO int16_t SR;
 __IO int16_t RES;
 __IO int16_t CH1;
 __IO int16_t CH2;
 __IO int16_t CH3;
 __IO int16_t CH4;
}rtu_cnt_t;

#define RTU_CNT		((rtu_cnt_t*)(g_holding_reg_table + MODBUS_HOLDING_REG_4012))

/* ************************ Bit definition for Sys Config *************************************/

#define RTU_SYS_CR1_UR_Msk	 (0x01 << 0)

#define RTU_SYS_SR_BSY_Msk		(0x01 <<0)
#define RTU_SYS_SR_NUM_Msk		(0x03 <<1)

#define RTU_SYS_EN_PWM1_Msk             (0x01 << 0)
#define RTU_SYS_EN_PWM2_Msk             (0x01 << 1)
#define RTU_SYS_EN_PWM3_Msk             (0x01 << 2)
#define RTU_SYS_EN_PWM4_Msk             (0x01 << 3)
#define RTU_SYS_EN_CNT1             		(0x01 << 4)
#define RTU_SYS_EN_CNT2             		(0x01 << 5)
#define RTU_SYS_EN_CNT3             		(0x01 << 6)
#define RTU_SYS_EN_CNT4             		(0x01 << 7)


/* ************************ Bit definition for PWM  *************************************/

#define RTU_PWM_CR_CH_Msk	 						(0x07 << 0)
#define RTU_PWM_CR_CH1_Msk	 					(0x01 << 0)
#define RTU_PWM_CR_CH2_Msk	 					(0x01 << 1)
#define RTU_PWM_CR_CH3_Msk	 					(0x01 << 2)
#define RTU_PWM_CR_CH4_Msk	 					(0x01 << 3)

#define RTU_PWM_SR_BUSY_Msk						(0x0F << 0)
#define RTU_PWM_SR_BUSY1_Msk	 				(0x01 << 0)
#define RTU_PWM_SR_BUSY2_Msk	 				(0x01 << 1)
#define RTU_PWM_SR_BUSY3_Msk	 				(0x01 << 2)
#define RTU_PWM_SR_BUSY4_Msk	 				(0x01 << 3)


/* ************************ Bit definition for COUNT  *************************************/

#define RTU_CNT_CR_CH_Msk	 						(0x07 << 0)
#define RTU_CNT_CR_CH1_Msk	 					(0x01 << 0)
#define RTU_CNT_CR_CH2_Msk	 					(0x01 << 1)
#define RTU_CNT_CR_CH3_Msk	 					(0x01 << 2)
#define RTU_CNT_CR_CH4_Msk	 					(0x01 << 3)

#define RTU_CNT_SR_BUSY_Msk						(0x0F << 0)
#define RTU_CNT_SR_BUSY1_Msk	 				(0x01 << 0)
#define RTU_CNT_SR_BUSY2_Msk	 				(0x01 << 1)
#define RTU_CNT_SR_BUSY3_Msk	 				(0x01 << 2)
#define RTU_CNT_SR_BUSY4_Msk	 				(0x01 << 3)


/* DMA (Modbus Commands) Definitions */
#define DMA_CMD_DATALEN	 						8
#define DMA_COMMAND_BUFFER_SIZE 		20

/* this byte indicates that rtu has initialized correctly */
#define RTU_BOOT_SEQUENCE				0x4711

// flag indicating new modbus command available
extern uint8_t g_flag_new_command;

// dma command buffer
extern uint8_t g_dma_cmd_buffer[DMA_COMMAND_BUFFER_SIZE];


#endif
