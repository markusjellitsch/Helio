/* ************************************************************************** */
// NAME:    bsp_tim4_ic.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for timer3 input capture						
/* ************************************************************************** */

#include "bsp_tim3_ic.h"
#include <stm32f1xx_hal.h>
#include <string.h>
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef htim4;

static uint16_t m_count_table[TIM4_IC_NUM_CHANNELS] = {0};
static uint16_t m_limit_table[TIM4_IC_NUM_CHANNELS] = {TIM4_IC_LIMIT_DEFAULT,TIM4_IC_LIMIT_DEFAULT,TIM4_IC_LIMIT_DEFAULT,TIM4_IC_LIMIT_DEFAULT};
static uint8_t m_captured_table[TIM4_IC_NUM_CHANNELS] = {0};
static frequency_info_t m_frequency_table[TIM4_IC_NUM_CHANNELS] = {0};


uint8_t bsp_tim4_ic_init(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_IC_InitTypeDef sConfigIC;

	 // enable clocks
	 __HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// config gpio for channel 4,3,2 & 1
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_8 |  GPIO_PIN_7 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// config timer 3 
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = TIM4_IC_DEFAULT_RESOLUTION-1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 0xFFFF;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		return 0;
	}

	// config inpit capture
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	
	HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2);
	HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3);
	HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_4);

		
	memset(m_captured_table,0,sizeof(uint8_t)*TIM4_IC_NUM_CHANNELS);
	memset(m_limit_table,0,sizeof(uint16_t)*TIM4_IC_NUM_CHANNELS);
	memset(m_count_table,0,sizeof(uint16_t)*TIM4_IC_NUM_CHANNELS);
		
  TIM4->DIER = 0;
	
	// enable nvic 
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_ClearPendingIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn,0);
		
	return 1;
}



// start capturing on a one or more channels
uint8_t bsp_tim4_ic_start_capture(uint8_t const channel_mask){
	
	if (channel_mask > TIM4_IC_MASK_ALL) return 0;
	
	// switch all off
	bsp_tim4_ic_stop_capture(TIM4_IC_MASK_ALL);
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL1)==TIM4_IC_MASK_CHANNEL1){
		m_count_table[TIM4_IC_CHANNEL1] = 0;
		m_captured_table[TIM4_IC_CHANNEL1] = 0;
	  m_frequency_table[TIM4_IC_CHANNEL1].first = 0;
		HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL2)==TIM4_IC_MASK_CHANNEL2){
		m_count_table[TIM4_IC_CHANNEL2] = 0;
		m_captured_table[TIM4_IC_CHANNEL2] = 0;
		m_frequency_table[TIM4_IC_CHANNEL2].first = 0;
	  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL3)==TIM4_IC_MASK_CHANNEL3){
		m_count_table[TIM4_IC_CHANNEL3] = 0;
		m_captured_table[TIM4_IC_CHANNEL3] = 0;
		m_frequency_table[TIM4_IC_CHANNEL3].first = 0;
		HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL4)==TIM4_IC_MASK_CHANNEL4){
		m_count_table[TIM4_IC_CHANNEL4] = 0;
		m_captured_table[TIM4_IC_CHANNEL4] = 0;
		m_frequency_table[TIM4_IC_CHANNEL4].first = 0;
		HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_4);
	}
	
	return 1;
}

// stop capturing on a specific channel
uint8_t bsp_tim4_ic_stop_capture(uint8_t const channel_mask){
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL1)==TIM4_IC_MASK_CHANNEL1){
		HAL_TIM_IC_Stop_IT(&htim4,TIM_CHANNEL_1);
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL2)==TIM4_IC_MASK_CHANNEL2){
		HAL_TIM_IC_Stop_IT(&htim4,TIM_CHANNEL_2);
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL3)==TIM4_IC_MASK_CHANNEL3){
		HAL_TIM_IC_Stop_IT(&htim4,TIM_CHANNEL_3);
	}

	if ((channel_mask & TIM4_IC_MASK_CHANNEL4)==TIM4_IC_MASK_CHANNEL4){
		HAL_TIM_IC_Stop_IT(&htim4,TIM_CHANNEL_4);
	}
	
	
 
	return 1;
}

uint8_t bsp_tim4_ic_set_limit(uint8_t const channel_mask,uint16_t const limit){

	if (channel_mask > TIM4_IC_MASK_ALL) return 0;
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL1)==TIM4_IC_MASK_CHANNEL1){
		m_limit_table[TIM4_IC_CHANNEL1] = limit;
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL2)==TIM4_IC_MASK_CHANNEL2){
		m_limit_table[TIM4_IC_CHANNEL2] = limit;
	}
	
	if ((channel_mask & TIM4_IC_MASK_CHANNEL3)==TIM4_IC_MASK_CHANNEL3){
		m_limit_table[TIM4_IC_CHANNEL3] = limit;
	}

	if ((channel_mask & TIM4_IC_MASK_CHANNEL4)==TIM4_IC_MASK_CHANNEL4){
		m_limit_table[TIM4_IC_CHANNEL4] = limit;
	}
	
	return 1;
}

// set prescaler for timer
uint8_t	bsp_tim4_ic_set_resolution(uint16_t const prescaler){
	
	TIM4->PSC = prescaler -1;
	
	return 1;
}


// return if num of counts was capured
uint8_t bsp_tim4_ic_is_captured(uint8_t const channel){
 
	if (channel >= TIM4_IC_NUM_CHANNELS) return 0;
	
	return m_captured_table[channel];
	
}

// get num of captures
uint16_t bsp_tim4_ic_get_count(uint8_t const channel){
  
  if (channel >= TIM4_IC_NUM_CHANNELS) return 0;

	return m_count_table[channel];
}

// get last captured frequency
uint16_t bsp_tim4_ic_get_frequency(uint8_t const channel){
	
	if (channel >= TIM4_IC_NUM_CHANNELS) return 0; 

	volatile uint16_t tmp = 0;
	
	if (m_frequency_table[channel].first > m_frequency_table[channel].second){
			tmp = 0xFFFF - m_frequency_table[channel].first + m_frequency_table[channel].second;
	}
	else tmp = m_frequency_table[channel].second -m_frequency_table[channel].first;
	
	return tmp;
}



void TIM4_IRQHandler(){

	NVIC_ClearPendingIRQ(TIM4_IRQn);

	
	// -------------------------
	// input capture channel 1
	// -------------------------
	if (ISSET(TIM4->DIER,TIM_DIER_CC1IE_Msk) && (ISSET(TIM4->SR,TIM_SR_CC1IF_Msk) || ISSET(TIM4->SR,TIM_SR_CC1OF_Msk))){
		 if (++m_count_table[TIM4_IC_CHANNEL1] >= m_limit_table[TIM4_IC_CHANNEL1]){
			 TIM4->DIER &= ~TIM_DIER_CC1IE_Msk;
			 m_frequency_table[TIM4_IC_CHANNEL1].second = TIM4->CCR1;
			 m_captured_table[TIM4_IC_CHANNEL1] = 1;
		 }
		 else m_frequency_table[TIM4_IC_CHANNEL1].first = TIM4->CCR1;
		 
		 TIM4->SR &= ~TIM_SR_CC1OF_Msk & ~TIM_SR_CC1IF_Msk;
	}
	
	// -------------------------
	// input capture channel 2
	// -------------------------
	if (ISSET(TIM4->DIER,TIM_DIER_CC2IE_Msk) && (ISSET(TIM4->SR,TIM_SR_CC2IF_Msk) || ISSET(TIM4->SR,TIM_SR_CC2OF_Msk))){
		if (++m_count_table[TIM4_IC_CHANNEL2] >= m_limit_table[TIM4_IC_CHANNEL2]){
		    m_frequency_table[TIM4_IC_CHANNEL2].second = TIM4->CCR2;
				TIM4->DIER &= ~TIM_DIER_CC2IE_Msk;
				TIM4->CCR2= 0;
				m_captured_table[TIM4_IC_CHANNEL2] = 1;
		 }
		 else m_frequency_table[TIM4_IC_CHANNEL2].first = TIM4->CCR2;
		 TIM4->SR &= ~TIM_SR_CC2OF_Msk & ~TIM_SR_CC2IF_Msk;
	}
	
	// -------------------------
	// input capture channel 3
	// -------------------------
	if (ISSET(TIM4->DIER,TIM_DIER_CC3IE_Msk) && (ISSET(TIM4->SR,TIM_SR_CC3IF_Msk) || ISSET(TIM4->SR,TIM_SR_CC3OF_Msk))){
		if (++m_count_table[TIM4_IC_CHANNEL3] >= m_limit_table[TIM4_IC_CHANNEL3]){
		   m_frequency_table[TIM4_IC_CHANNEL3].second = TIM4->CCR3;
			 TIM4->DIER &= ~TIM_DIER_CC3IE_Msk;
			 TIM4->CCR3= 0;
			 m_captured_table[TIM4_IC_CHANNEL3] = 1;
		 }
		 else m_frequency_table[TIM4_IC_CHANNEL3].first = TIM4->CCR3;
		 TIM4->SR &= ~TIM_SR_CC3OF_Msk & ~TIM_SR_CC3IF_Msk;
	}
	
	// -------------------------
	// input capture channel 4
	// -------------------------
	if (ISSET(TIM4->DIER,TIM_DIER_CC4IE_Msk) && (ISSET(TIM4->SR,TIM_SR_CC4IF_Msk) || ISSET(TIM4->SR,TIM_SR_CC4OF_Msk))){
		if (++m_count_table[TIM4_IC_CHANNEL4] >= m_limit_table[TIM4_IC_CHANNEL4]){
		  m_frequency_table[TIM4_IC_CHANNEL4].second = TIM4->CCR4; 
			TIM4->DIER &= ~TIM_DIER_CC4IE_Msk;
			 TIM4->CCR4= 0;
			 m_captured_table[TIM4_IC_CHANNEL4] = 1;
		 }
		 else m_frequency_table[TIM4_IC_CHANNEL4].first = TIM4->CCR4;
		 TIM4->SR &= ~TIM_SR_CC4OF_Msk & ~TIM_SR_CC4IF_Msk;
	}
}


