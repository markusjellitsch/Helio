/* ************************************************************************** */
// NAME:    bsp_tim1_pwm.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for timer1 pwm generator						
/* ************************************************************************** */

#include "bsp_tim1_pwm.h"
#include <stm32f1xx_hal.h>
#include <string.h>
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef htim1;


static uint16_t m_channel1_count = 0;
static uint16_t m_channel2_count = 0;
static uint16_t m_channel3_count = 0;
static uint16_t m_channel4_count = 0;


uint8_t bsp_tim1_pwm_init(){

	GPIO_InitTypeDef GPIO_InitStruct;

	 // enable clocks
	 __HAL_RCC_TIM1_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	// config gpio for channel 4,3,2 & 1
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // config control register 1
	TIM1->CR1 = 0;
	TIM1->CR1 |= TIM_CR1_UDIS_Msk;
	TIM1->CR1 |= TIM_CR1_URS_Msk;
	TIM1->ARR = 0xFFFF;
	TIM1->PSC = 	TIM1_PWM_PRESCALER_DEFAULT -1;
	
	// config mode register for oc1 & oc2
	TIM1->CCMR1 = 0;
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE_Msk;
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_0;
	
	// config mode register for oc3 & oc4
	TIM1->CCMR2 = 0;
	TIM1->CCMR2 |=  TIM_CCMR2_OC4PE_Msk;
	TIM1->CCMR2 |=  TIM_CCMR2_OC4M_0;
	
	// enable output compare for oc1,oc2,oc3 & oc4
	TIM1->CCER = 0;
	TIM1->CCER |= TIM_CCER_CC1E_Msk  | TIM_CCER_CC4E_Msk; // enable 

  // disable all interrupts
	TIM1->DIER = 0;
  
	// reset status register
	TIM1->SR = 0;
	

	// master output enable bit
	TIM1->BDTR |= TIM_BDTR_MOE_Msk;
		
	// enable nvic
	NVIC_EnableIRQ(TIM1_CC_IRQn);
	NVIC_ClearPendingIRQ(TIM1_CC_IRQn);
	HAL_NVIC_SetPriority(TIM1_CC_IRQn,2,0);

	
	// enable timer1
	TIM1->CR1 &= ~TIM_CR1_UDIS_Msk;
  TIM1->CR1 |= TIM_CR1_CEN_Msk;
	
	return 1;
}

uint8_t bsp_tim1_pwm_set_resolution(uint16_t const prescaler){
	
	// value is preloaded.
	TIM1->PSC = prescaler-1;
	
	return 1;
}

 uint8_t bsp_tim1_pwm_set_period(uint16_t const period){

	if (period == 1) return 0;
	
	// duty cycle 50%
	TIM1->CCR1 = period /2;
	TIM1->CCR2 = period /2;
	TIM1->CCR3 = period /2;
	TIM1->CCR4 = period /2;
	TIM1->ARR = period-1;
	
	return 1;
}

 uint8_t bsp_tim1_pwm_set_duty_cycle(uint8_t const duty){
	
	uint32_t tmp = TIM1->ARR+1;
	if (duty <= 0 || duty >= 100) return 0;
	
	tmp = (uint16_t)((double)tmp * (double)duty / 100.0);
	
	// duty cycle 50%
	TIM1->CCR1 = tmp;
	TIM1->CCR2 = tmp;
	TIM1->CCR3 = tmp;
	TIM1->CCR4 = tmp;
	
	return 1;
}
 
 uint8_t bsp_tim1_pwm_set_count(uint8_t const channel_mask,uint16_t const count){
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL1)==TIM1_PWM_MASK_CHANNEL1){
			m_channel1_count = count;
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL2)==TIM1_PWM_MASK_CHANNEL2){
			m_channel2_count = count;
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL3)==TIM1_PWM_MASK_CHANNEL3){
			m_channel3_count = count;
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL4)==TIM1_PWM_MASK_CHANNEL4){
			m_channel4_count = count;
	}
	
	return 1;
}
 
uint16_t bsp_tim1_pwm_get_count(uint8_t const channel_mask){

	if ((channel_mask & TIM1_PWM_MASK_CHANNEL1)==TIM1_PWM_MASK_CHANNEL1){
			return m_channel1_count;
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL2)==TIM1_PWM_MASK_CHANNEL2){
			return m_channel2_count;
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL3)==TIM1_PWM_MASK_CHANNEL3){
		return 	m_channel3_count;
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL4)==TIM1_PWM_MASK_CHANNEL4){
			return m_channel4_count;
	}
	
	return 0;
}


// start capturing on a one or more channels
uint8_t bsp_tim1_pwm_start(uint8_t const channel_mask){
	
	// reset status register
	TIM1->SR = 0;
	TIM1->CNT = 0;
	
	// start oc1
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL1)==TIM1_PWM_MASK_CHANNEL1){	
		TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
		if (m_channel1_count >0) TIM1->DIER |= TIM_DIER_CC1IE_Msk;
	}
	
	// start oc2
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL2)==TIM1_PWM_MASK_CHANNEL2){
		TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;
		if (m_channel2_count >0)TIM1->DIER |= TIM_DIER_CC2IE_Msk; 
	}
	
	// start oc3
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL3)==TIM1_PWM_MASK_CHANNEL3){
	  TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;
		if (m_channel3_count >0) TIM1->DIER |= TIM_DIER_CC3IE_Msk;
	}
	
	// start oc4
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL4)==TIM1_PWM_MASK_CHANNEL4){
		TIM1->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0;
			TIM1->SR &=~TIM_SR_CC4IF_Msk;
		if (m_channel4_count >0)TIM1->DIER |= TIM_DIER_CC4IE_Msk;
	}

	return 1;
}

// stop capturing on a specific channel
uint8_t bsp_tim1_pwm_stop(uint8_t const channel_mask){
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL1)==TIM1_PWM_MASK_CHANNEL1){
		TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_1 & ~TIM_CCMR1_OC1M_2;
	  TIM1->SR &=~TIM_SR_CC1IF_Msk;
		if (m_channel1_count > 0) TIM1->DIER &= ~TIM_DIER_CC1IE_Msk;
    m_channel1_count = 0;		
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL2)==TIM1_PWM_MASK_CHANNEL2){
		TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_1 & ~TIM_CCMR1_OC2M_2;
		if (m_channel2_count > 0)TIM1->DIER &= ~TIM_DIER_CC2IE_Msk;
    m_channel2_count = 0;			
	}
	
	if ((channel_mask & TIM1_PWM_MASK_CHANNEL3)==TIM1_PWM_MASK_CHANNEL3){
		TIM1->CCMR2 &= ~TIM_CCMR2_OC3M_1 & ~TIM_CCMR2_OC3M_2;
		if (m_channel3_count > 0)TIM1->DIER &= ~TIM_DIER_CC3IE_Msk;
    m_channel3_count = 0;		
	}

	if ((channel_mask & TIM1_PWM_MASK_CHANNEL4)==TIM1_PWM_MASK_CHANNEL4){
		TIM1->CCMR2 &= ~TIM_CCMR2_OC4M_1 & ~TIM_CCMR2_OC4M_2;
		TIM1->SR &=~TIM_SR_CC4IF_Msk;
		if (m_channel4_count > 0)TIM1->DIER &= ~TIM_DIER_CC4IE_Msk;
    m_channel4_count = 0;			
	}

 
	return 1;
}

// return if a channel is enabled
uint8_t bsp_tim1_pwm_is_enabled(uint8_t const channel){
	
	if (channel == TIM1_PWM_CHANNEL1){
		return ((TIM1->DIER & TIM_DIER_CC1IE_Msk) == TIM_DIER_CC1IE_Msk);
	}
	
	if (channel == TIM1_PWM_CHANNEL2){
		return ((TIM1->DIER & TIM_DIER_CC2IE_Msk) == TIM_DIER_CC2IE_Msk);
	}

	if (channel == TIM1_PWM_CHANNEL3){
		return ((TIM1->DIER & TIM_DIER_CC3IE_Msk) == TIM_DIER_CC3IE_Msk);
	}

	if (channel == TIM1_PWM_CHANNEL4){
		return ((TIM1->DIER & TIM_DIER_CC4IE_Msk) == TIM_DIER_CC4IE_Msk);
	}	
	
	return 0;
}



void TIM1_CC_IRQHandler(){

	NVIC_ClearPendingIRQ(TIM1_CC_IRQn);
	
	// save tmp register
	uint32_t ccer = TIM1->CCER;

	// switch off channel1 when counter is reached
	if (((TIM1->DIER & TIM_DIER_CC1IE_Msk) == TIM_DIER_CC1IE_Msk) && ((TIM1->SR & TIM_SR_CC1IF_Msk) == TIM_SR_CC1IF_Msk)){
		TIM1->SR &=~TIM_SR_CC1IF_Msk;
		if (m_channel1_count--==0){
				TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_1 & ~TIM_CCMR1_OC1M_2;
				TIM1->DIER &= ~TIM_DIER_CC1IE_Msk;			
		}
	}

	// switch off channel2 when counter is reached
if (((TIM1->DIER & TIM_DIER_CC2IE_Msk) == TIM_DIER_CC2IE_Msk) && ((TIM1->SR & TIM_SR_CC2IF_Msk) == TIM_SR_CC2IF_Msk)){
		TIM1->SR &=~TIM_SR_CC2IF_Msk;
		if (m_channel2_count--==0){
				TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_1 & ~TIM_CCMR1_OC2M_2;
				TIM1->DIER &= ~TIM_DIER_CC2IE_Msk;			
		}
	}

	// switch off channel3 when counter is reached
	if (((TIM1->DIER & TIM_DIER_CC3IE_Msk) == TIM_DIER_CC3IE_Msk) && ((TIM1->SR & TIM_SR_CC3IF_Msk) == TIM_SR_CC3IF_Msk)){
		TIM1->SR &=~TIM_SR_CC3IF_Msk;
		if (m_channel3_count--==0){
				TIM1->CCMR2 &= ~TIM_CCMR2_OC3M_1 & ~TIM_CCMR2_OC3M_2;
				TIM1->DIER &= ~TIM_DIER_CC3IE_Msk;			
		}
	}

	// switch off channel4 when counter is reached
	if (((TIM1->DIER & TIM_DIER_CC4IE_Msk) == TIM_DIER_CC4IE_Msk) && ((TIM1->SR & TIM_SR_CC4IF_Msk) == TIM_SR_CC4IF_Msk)){
		TIM1->SR &=~TIM_SR_CC4IF_Msk;
		if (m_channel4_count--==0){
				TIM1->CCMR2 &= ~TIM_CCMR2_OC4M_1 & ~TIM_CCMR2_OC4M_2;
				TIM1->DIER &= ~TIM_DIER_CC4IE_Msk;			
		}
	}
}





