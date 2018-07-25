/* ************************************************************************** */
// NAME:    bsp_dac.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for digital-analog-converter (DAC)						
/* ************************************************************************** */

#include "bsp_dac.h"
#include <stm32f1xx_hal.h>
#include <string.h>
#include "stm32f1xx_hal_tim.h"


uint8_t bsp_dac_init(void){

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_DAC_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;

	// config gpio for channel 1
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	DAC->CR = 0;
	

	return 1;
}

// set voltage in millivolts
uint8_t bsp_dac_set_voltage(uint16_t const voltage){

	
	if (voltage > DAC_DEFAULT_VDDA) return 0;
	
	DAC->CR &= ~DAC_CR_EN1_Msk;

	// calculate dor value
	volatile uint16_t dor_val = (uint16_t)((double)voltage * DAC_DEFAULT_RESOLUTION / DAC_DEFAULT_VDDA);
	dor_val &= 0xFFF;
	
	// enable dac
	DAC->DHR12RD = dor_val;
	DAC->CR |= DAC_CR_EN1_Msk;
	
		
	return 0;
}

// get voltage in millivolts
uint16_t bsp_dac_get_voltage(void){

	uint16_t voltage = (uint16_t)((double)(DAC->DOR1) / DAC_DEFAULT_RESOLUTION * DAC_DEFAULT_VDDA);
	return voltage;
}





