/* ************************************************************************** */
// NAME:    app_callback.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: Application specific callbacks & wrapper functions
/* ************************************************************************** */

#include "stm32f1xx_hal.h"
#include "stm32vl_discovery.h"
#include "app_callbacks.h"
#include "app_globals.h"

// global variable
uint8_t g_flag_new_command = 0;

uint8_t g_dma_cmd_buffer[DMA_COMMAND_BUFFER_SIZE]={0};
static uint16_t m_buffer_index = DMA_CMD_DATALEN;
 

/* ***********************************************************************************************************************************************/
/* **************************************************** CALLBACKS FOR DMA ********************************************************************** */
/* ***********************************************************************************************************************************************/


// -----------------------------------------------
// DMA callback RX function
// -----------------------------------------------
uint8_t spi1_dma_rx_callback(void){

	bsp_spi1_dma_get_data(g_dma_cmd_buffer, DMA_COMMAND_BUFFER_SIZE+1);
	// check if data is valid 
	if (g_dma_cmd_buffer[0] != DMA_CMD_DATALEN) return false;
	
	uint8_t new_command = modbus_rtu_process_input(g_dma_cmd_buffer+1,DMA_CMD_DATALEN);
	
	if (new_command){
		g_flag_new_command = 1;
		return true;
	}
	else{
		g_flag_new_command = 0;
		modbus_protocol_reset();
		return false;
	}
}


// -----------------------------------------------
// DMA callback TX function
// -----------------------------------------------
uint8_t spi1_dma_tx_callback(void){
   
	// TODO 
	return false;
}

/* ********************************************************************************************************************************************** */
/* **************************************************** CALLBACKS FOR SYS CONF ******************************************************************* */
/* ********************************************************************************************************************************************** */

uint8_t app_callback_sys_config_cr1(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called SYS_CONF_CR1 (value: %d)",reg_val);
	
	return 1;
}

static uint16_t sys_en_backup = 0;

uint8_t app_callback_sys_config_en(uint8_t const function_code, int16_t const reg_val){
  //DEBUG_PRINT("Called SYS_CONF_EN (value: %d)",reg_val);

	volatile uint16_t tmp = 0;
	
   tmp = sys_en_backup ^ (uint16_t)reg_val;
	
	if (tmp == 0) return 1;
	
	// enable pwm1
	if (ISSET(tmp,RTU_SYS_EN_PWM1_Msk)){
			if (ISSET(reg_val,RTU_SYS_EN_PWM1_Msk)){
					bsp_tim1_pwm_start(TIM1_PWM_MASK_CHANNEL1);
				  RTU_PWM->SR |= RTU_PWM_SR_BUSY1_Msk;
			}
			else bsp_tim1_pwm_stop(TIM1_PWM_MASK_CHANNEL1);
	}
	
	// enable pwm2
	if (ISSET(tmp,RTU_SYS_EN_PWM2_Msk)){
			if (ISSET(reg_val,RTU_SYS_EN_PWM2_Msk)){
					bsp_tim1_pwm_start(TIM1_PWM_MASK_CHANNEL2);
				  RTU_PWM->SR |= RTU_PWM_SR_BUSY2_Msk;
			}
			else bsp_tim1_pwm_stop(TIM1_PWM_MASK_CHANNEL2);
	}
	
	// enable pwm3
	if (ISSET(tmp,RTU_SYS_EN_PWM3_Msk)){
			if (ISSET(reg_val,RTU_SYS_EN_PWM3_Msk)){
					bsp_tim1_pwm_start(TIM1_PWM_MASK_CHANNEL3);
				  RTU_PWM->SR |= RTU_PWM_SR_BUSY3_Msk;
			}
			else bsp_tim1_pwm_stop(TIM1_PWM_MASK_CHANNEL3);
	}
	
	// enable pwm4
	if (ISSET(tmp,RTU_SYS_EN_PWM4_Msk)){
			if (ISSET(reg_val,RTU_SYS_EN_PWM4_Msk)){
				bsp_tim1_pwm_start(TIM1_PWM_MASK_CHANNEL4);
				RTU_PWM->SR |= RTU_PWM_SR_BUSY4_Msk;
			}
			else bsp_tim1_pwm_stop(TIM1_PWM_MASK_CHANNEL4);
	}
	
	// enable cnt1
	if (ISSET(tmp,RTU_SYS_EN_CNT1)){
			if (ISSET(reg_val,RTU_SYS_EN_CNT1)){
					bsp_tim4_ic_start_capture(TIM4_IC_MASK_CHANNEL1);
				  RTU_CNT->SR |= RTU_CNT_SR_BUSY1_Msk;
			}
			else {
				bsp_tim4_ic_stop_capture(TIM4_IC_MASK_CHANNEL1);
				RTU_CNT->SR &= ~RTU_CNT_SR_BUSY1_Msk;
			}
	}
	
	// enable cnt2
	if (ISSET(tmp,RTU_SYS_EN_CNT2)){
			if (ISSET(reg_val,RTU_SYS_EN_CNT2)){
					bsp_tim4_ic_start_capture(TIM4_IC_MASK_CHANNEL2);
				  RTU_CNT->SR |= RTU_CNT_SR_BUSY2_Msk;
			}
			else {
				bsp_tim4_ic_stop_capture(TIM4_IC_MASK_CHANNEL2);
				RTU_CNT->SR &= ~RTU_CNT_SR_BUSY2_Msk;
			}
	}
	
	// enable cnt3
	if (ISSET(tmp,RTU_SYS_EN_CNT3)){
			if (ISSET(reg_val,RTU_SYS_EN_CNT3)){
					RTU_CNT->SR |= RTU_CNT_SR_BUSY3_Msk;
					bsp_tim4_ic_start_capture(TIM4_IC_MASK_CHANNEL3);
			}
			else {
				bsp_tim4_ic_stop_capture(TIM4_IC_MASK_CHANNEL3);
				RTU_CNT->SR &= ~RTU_CNT_SR_BUSY3_Msk;
			}
	}
	
	// enable cnt4
	if (ISSET(tmp,RTU_SYS_EN_CNT4)){
			if (ISSET(reg_val,RTU_SYS_EN_CNT4)){
				  RTU_CNT->SR |= RTU_CNT_SR_BUSY4_Msk;
					bsp_tim4_ic_start_capture(TIM4_IC_MASK_CHANNEL4);
			}
			else {
				bsp_tim4_ic_stop_capture(TIM4_IC_MASK_CHANNEL4);
				RTU_CNT->SR &= ~RTU_CNT_SR_BUSY4_Msk;
			}
	}
	
  sys_en_backup = (uint16_t) reg_val;	
	
	return 1;
}

uint8_t app_callback_sys_config_sr(uint8_t const function_code, int16_t const reg_val){
  
	DEBUG_PRINT("Called SYS_CONF_CR1 (value: %d)",reg_val);
	
	if (reg_val == 0) return 1;
	
	uint8_t busy_bit = 0;
	busy_bit += bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL1);
	busy_bit += bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL2);
	busy_bit += bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL3);
	busy_bit += bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL4);
	busy_bit += bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL1);
	busy_bit += bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL2);
	busy_bit += bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL3);
	busy_bit += bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL4);
	
	if (busy_bit >0) RTU_SYS->SR |= RTU_SYS_SR_BSY_Msk;
	else RTU_SYS->SR &= ~RTU_SYS_SR_BSY_Msk;
	
	return 1;
}

/* ********************************************************************************************************************************************** */
/* **************************************************** CALLBACKS FOR PWM  ********************************************************************** */
/* ********************************************************************************************************************************************** */
uint8_t app_callback_pwm_cr(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called PWM_CR (value: %d)",reg_val);
	
	// TODO: THINK ABOUT A POSSIBLE COMMAND!
	
	return 1;
}

uint8_t app_callback_pwm_sr(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called PWM_SR (value: %d)",reg_val);
	
	if (reg_val == 0) return 1;
	
	// clear the status register
	RTU_PWM->SR = RTU_PWM_SR_BUSY_Msk;
		
	volatile uint16_t  pwm_en = RTU_SYS->EN;
	uint8_t cnt_pwm1 = bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL1);
	uint8_t cnt_pwm2 = bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL2);
	uint8_t cnt_pwm3 = bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL3);
	uint8_t cnt_pwm4 = bsp_tim1_pwm_is_enabled(TIM1_PWM_CHANNEL4);
	
	// unset busy for pwm1
	if (!cnt_pwm1){
			RTU_PWM->SR &= RTU_PWM_SR_BUSY1_Msk;
	}
	
	// unset busy for pwm2
	if (!cnt_pwm1){
			RTU_PWM->SR |= RTU_PWM_SR_BUSY2_Msk;
	}
		
	// unset busy for pwm3
	if (!cnt_pwm3){
			RTU_PWM->SR |= RTU_PWM_SR_BUSY3_Msk;
	}
  
	// unset busy for pwm4
	if (!cnt_pwm4){
			RTU_PWM->SR |= RTU_PWM_SR_BUSY4_Msk;
	}
	
	if (!cnt_pwm1 && !cnt_pwm2 & !cnt_pwm3 & !cnt_pwm4){
		DEBUG_PRINT("All PWMs finished!\n");
		
	}
	
	return 1;
}

uint8_t app_callback_pwm_prd(uint8_t const function_code, int16_t const reg_val){
  
	DEBUG_PRINT("Called PWM_PRD (value: %d)",reg_val);
	
	// set update bit
	bsp_tim1_pwm_set_period((uint16_t)reg_val);

	return 1;
}

uint8_t app_callback_pwm_cnt(uint8_t const function_code, int16_t const reg_val){
  
	DEBUG_PRINT("Called PWM_CNT (value: %d)",reg_val);
	
	// set update bit
	bsp_tim1_pwm_set_count(TIM1_PWM_MASK_ALL,(uint16_t)reg_val);
	
	return 1;
}

uint8_t app_callback_pwm_psc(uint8_t const function_code, int16_t const reg_val){
  
	DEBUG_PRINT("Called PWM_PSC (value: %d)",reg_val);
	
	// set update bit
	bsp_tim1_pwm_set_resolution((uint16_t)reg_val);

	return 1;
}

uint8_t app_callback_pwm_dut(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called PWM_DUT (value: %d)",reg_val);
	
	// set update bit
	bsp_tim1_pwm_set_duty_cycle((uint8_t)reg_val);
	return 1;
}


/* ********************************************************************************************************************************************** */
/* **************************************************** CALLBACKS FOR CNT  ********************************************************************** */
/* ********************************************************************************************************************************************** */

uint8_t app_callback_cnt_prd(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_PRD (value: %d)",reg_val);
	
	// get period for channel 1
	if (reg_val == RTU_CNT_CR_CH1_Msk){
			RTU_CNT->PRD = bsp_tim4_ic_get_frequency(TIM4_IC_CHANNEL1);
	}

	// get period for channel 2
	if (reg_val == RTU_CNT_CR_CH2_Msk){
			RTU_CNT->PRD = bsp_tim4_ic_get_frequency(TIM4_IC_CHANNEL2);
	}

	// get period for channel 3
	if (reg_val == RTU_CNT_CR_CH3_Msk){
			RTU_CNT->PRD = bsp_tim4_ic_get_frequency(TIM4_IC_CHANNEL3);
	} 	
	
	// get period for channel 4
	if (reg_val == RTU_CNT_CR_CH4_Msk){
			RTU_CNT->PRD = bsp_tim4_ic_get_frequency(TIM4_IC_CHANNEL4);
	} 
	
	return 1;
}

uint8_t app_callback_cnt_sr(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_SR (value: %d)",reg_val);
	
	if (reg_val == 0) return 1;
	
	// clear the status register
	RTU_CNT->SR = RTU_CNT_SR_BUSY_Msk;
		
	uint8_t cnt_freq1 = bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL1);
	uint8_t cnt_freq2 = bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL2);
	uint8_t cnt_freq3 = bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL3);
	uint8_t cnt_freq4 = bsp_tim4_ic_is_captured(TIM4_IC_CHANNEL4);
	
	// unset busy for ch1
	if (cnt_freq1){
			RTU_CNT->SR &= ~RTU_CNT_SR_BUSY1_Msk;
	}
	
	// unset busy for ch2
	if (cnt_freq2){
			RTU_CNT->SR &= ~RTU_CNT_SR_BUSY2_Msk;
	}
		
	// unset busy for ch3
	if (cnt_freq3){
			RTU_CNT->SR &= ~RTU_CNT_SR_BUSY3_Msk;
	}
  
	// unset busy for ch4
	if (cnt_freq4){
			RTU_CNT->SR &= ~RTU_CNT_SR_BUSY4_Msk;
	}
	
	if (cnt_freq1 && cnt_freq2 & cnt_freq3 & cnt_freq4){
		DEBUG_PRINT("All FREQ COUNTS finished!\n");
		
	}
	
	return 1;
}

uint8_t app_callback_cnt_ch1(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_CH1 (value: %d)",reg_val);
  bsp_tim4_ic_set_limit(TIM4_IC_MASK_CHANNEL1,(uint16_t) reg_val);
	return 1;
}

uint8_t app_callback_cnt_ch2(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_CH2 (value: %d)",reg_val);
  bsp_tim4_ic_set_limit(TIM4_IC_MASK_CHANNEL2,(uint16_t) reg_val);
	return 1;
}

uint8_t app_callback_cnt_ch3(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_CH3 (value: %d)",reg_val);
  bsp_tim4_ic_set_limit(TIM4_IC_MASK_CHANNEL3,(uint16_t) reg_val);
	return 1;
}

uint8_t app_callback_cnt_ch4(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_CH4 (value: %d)",reg_val);
  bsp_tim4_ic_set_limit(TIM4_IC_MASK_CHANNEL4,(uint16_t) reg_val);
	return 1;
}

uint8_t app_callback_cnt_res(uint8_t const function_code, int16_t const reg_val){
  DEBUG_PRINT("Called CNT_RES (value: %d)",reg_val);
	bsp_tim4_ic_set_resolution((uint16_t)reg_val);
	return 1;
}

/* ********************************************************************************************************************************************** */
/* **************************************************** EVENTS ** ******************************************************************* */
/* ********************************************************************************************************************************************** */

uint8_t app_callback_event_synchronize(void){
  
	DEBUG_PRINT("EVENT SYNCHRONIZING");

	uint16_t dummy_val = 0x01;
	app_callback_cnt_sr(MODBUS_FUNC_WRITESINGLEREGISTER,dummy_val);
	app_callback_pwm_sr(MODBUS_FUNC_WRITESINGLEREGISTER,dummy_val);
	
	return 1;
}


/* ********************************************************************************************************************************************** */
/* **************************************************** WRAPPER FOR MODBUS ** ******************************************************************* */
/* ********************************************************************************************************************************************** */

// -----------------------------------------------
// Wrapper function for modbus TX
// -----------------------------------------------
void modbus_rx_wrapper(uint8_t byte){
	bsp_spi1_dma_put_byte(byte);
}


// -----------------------------------------------
// Wrapper function for modbus RX
// -----------------------------------------------
uint8_t modbus_tx_wrapper(uint8_t * byte){

	if (m_buffer_index >= DMA_CMD_DATALEN) return false;
	*byte=g_dma_cmd_buffer[m_buffer_index++];
	return true;
}

void modbus_register_init(void){
	
	modbus_register_set_value(RTU_SYS_BOOT,RTU_BOOT_SEQUENCE);
	modbus_register_set_readonly(RTU_SYS_BOOT,1);

	
	RTU_SYS->CR1 = 0;
	
	// register system callbacks
  modbus_rtu_register_callback(RTU_SYS_CR1,app_callback_sys_config_cr1);
	modbus_rtu_register_callback(RTU_SYS_SR,app_callback_sys_config_sr);
	modbus_rtu_register_callback(RTU_SYS_EN,app_callback_sys_config_en);
	
	// register pwm callbacks
	modbus_rtu_register_callback(RTU_PWM_CR,app_callback_pwm_cr);
	modbus_rtu_register_callback(RTU_PWM_PRD,app_callback_pwm_prd);
	modbus_rtu_register_callback(RTU_PWM_PSC,app_callback_pwm_psc);
  modbus_rtu_register_callback(RTU_PWM_CNT,app_callback_pwm_cnt);
  modbus_rtu_register_callback(RTU_PWM_DUT,app_callback_pwm_dut);

	// register cnt callbacks
	modbus_rtu_register_callback(RTU_CNT_PRD,app_callback_cnt_prd);	
  modbus_rtu_register_callback(RTU_CNT_SR,app_callback_cnt_sr);
	modbus_rtu_register_callback(RTU_CNT_CH1,app_callback_cnt_ch1);	
	modbus_rtu_register_callback(RTU_CNT_CH2,app_callback_cnt_ch2);	
	modbus_rtu_register_callback(RTU_CNT_CH3,app_callback_cnt_ch3);	
	modbus_rtu_register_callback(RTU_CNT_CH4,app_callback_cnt_ch4);	
	modbus_rtu_register_callback(RTU_CNT_RES,app_callback_cnt_res);	
}






