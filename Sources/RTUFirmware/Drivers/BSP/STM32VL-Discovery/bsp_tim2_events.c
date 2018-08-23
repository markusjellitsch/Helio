/* ************************************************************************** */
// NAME:    bsp_tim2_pwm.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for timer1 pwm generator						
/* ************************************************************************** */

#include "bsp_tim2_events.h"
#include <stm32f1xx_hal.h>
#include <string.h>
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef htim2;

static uint8_t m_num_events = 0;

static event_info_t m_event_table[TIM2_EVENT_NUM_EVENTS] = {0};

// static functions
static  uint8_t enable_interrupt(uint8_t const channel_nr);
static  uint8_t disable_interrupt(uint8_t const channel_nr);
static  uint8_t calculate_next_ccr(uint8_t const channel_nr);

uint8_t bsp_tim2_events_init(){

	 // enable clocks
	 __HAL_RCC_TIM2_CLK_ENABLE();

  // config control register 1
	TIM2->CR1 = 0;
	TIM2->CR1 |= TIM_CR1_UDIS_Msk;
	TIM2->CR1 |= TIM_CR1_URS_Msk;
	TIM2->ARR = 0xFFFF;
	TIM2->PSC = 	TIM2_EVENT_PRESCALER_DEFAULT -1;
	
  // disable all interrupts
	TIM2->DIER = 0;
  
	// reset status register
	TIM2->SR = 0;
	
	// enable timer1
	TIM2->CR1 &= ~TIM_CR1_UDIS_Msk;
	TIM2->CR1 |= TIM_CR1_CEN_Msk;
	
	// master output enable bit
	TIM2->BDTR |= TIM_BDTR_MOE_Msk;
		
	// enable nvic
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_ClearPendingIRQ(TIM2_IRQn);
  NVIC_SetPriority(TIM2_IRQn,2);

	return 1;
}

// start event timer
uint8_t bsp_tim2_events_start_timer(void){
	TIM2->CR1 |= TIM_CR1_CEN_Msk;
	return 1;
}

// start & stop event timer
uint8_t bsp_tim2_events_stop_timer(void){
	TIM2->CR1 &= ~TIM_CR1_CEN_Msk;
	TIM2->DIER = 0;
	TIM2->SR = 0;
	
	return 1;
}


// register a new event
uint8_t bsp_tim2_events_register_event(uint8_t const event_nr, event_trigger_t const trigger, event_handler_t const event_callback, uint8_t fire_and_forget, uint16_t const period){

	if ((event_nr >= TIM2_EVENT_NUM_EVENTS)) return 0;

	// increment event counter when no event registered
	if (m_event_table[event_nr].event !=0){
			bsp_tim2_events_forget_event(event_nr);
	}
	 
	m_num_events++;
 	
	m_event_table[event_nr].fire_and_forget = fire_and_forget;
	m_event_table[event_nr].period = period;
	m_event_table[event_nr].event = event_callback;
	m_event_table[event_nr].trigger = trigger;
	
	// calculate next ccr trigger event
	calculate_next_ccr(event_nr);
	
	enable_interrupt(event_nr);
	
	return 1;
}

// clear an event
uint8_t bsp_tim2_events_forget_event(uint8_t const event_nr){

	if ((event_nr >= TIM2_EVENT_NUM_EVENTS) || (m_num_events <= 0)) return 0;

	// decrement event counter
	if (m_event_table[event_nr].event !=0){
			m_num_events--;
	}
	
	m_event_table[event_nr].fire_and_forget = 0;
	m_event_table[event_nr].period = 0;
	m_event_table[event_nr].event = 0;
	m_event_table[event_nr].trigger = 0;
	
	disable_interrupt(event_nr);
	
	return 1;
}

// manually trigger an event
uint8_t bsp_tim1_event_trigger_event(uint8_t const event_nr);


static  uint8_t enable_interrupt(uint8_t const event_nr){
			
	switch (event_nr){

		case TIM2_EVENT1:
			 TIM2->DIER |= TIM_DIER_CC1IE_Msk;
		break;

		case TIM2_EVENT2:
			 TIM2->DIER |= TIM_DIER_CC2IE_Msk;
		break;

		case TIM2_EVENT3:
			 TIM2->DIER |= TIM_DIER_CC3IE_Msk;
		break;

		case TIM2_EVENT4:
			 TIM2->DIER |= TIM_DIER_CC4IE_Msk;
		break;

		default:
			return 0;
		}

	return 1;
}

static  uint8_t disable_interrupt(uint8_t const event_nr){
			
	switch (event_nr){

		case TIM2_EVENT1:
			 TIM2->DIER &= ~TIM_DIER_CC1IE_Msk;
		break;

		case TIM2_EVENT2:
			 TIM2->DIER &= ~TIM_DIER_CC2IE_Msk;
		break;

		case TIM2_EVENT3:
			 TIM2->DIER &= ~TIM_DIER_CC3IE_Msk;
		break;

		case TIM2_EVENT4:
			 TIM2->DIER &= ~TIM_DIER_CC4IE_Msk;
		break;

		default:
			return 0;
		}

	return 1;
}

static  uint8_t calculate_next_ccr(uint8_t const event_nr){
	
	volatile uint16_t period = m_event_table[event_nr].period + TIM2->CNT;
	
	switch (event_nr){

		case TIM2_EVENT1:
			 TIM2->CCR1 = period ;
		break;

		case TIM2_EVENT2:
			  TIM2->CCR2 = period;
		break;

		case TIM2_EVENT3:
			  TIM2->CCR3 = period;
		break;

		case TIM2_EVENT4:
			 TIM2->CCR4 = period;
		break;

		default:
			return 0;
		}

	return 1;
	
}


void TIM2_IRQHandler(){

	NVIC_ClearPendingIRQ(TIM2_IRQn);
	
	
	// ----------------
	// EVENT 1
	//-----------------
	if (((TIM2->DIER & TIM_DIER_CC1IE_Msk) ==TIM_DIER_CC1IE_Msk) && (TIM2->SR & TIM_SR_CC1IF_Msk) == TIM_SR_CC1IF_Msk){
		// clear flag
		TIM2->SR &= ~TIM_SR_CC1IF_Msk;
		
		// periodic event
		if (m_event_table[TIM2_EVENT1].trigger == 0){
				if (m_event_table[TIM2_EVENT1].event !=0)m_event_table[TIM2_EVENT1].event(TIM2_EVENT1);
		 	  calculate_next_ccr(TIM2_EVENT1);
		}
		// triggered event
		else {
			
			calculate_next_ccr(TIM2_EVENT1);
					
			if (m_event_table[TIM2_EVENT1].trigger()){
				if (m_event_table[TIM2_EVENT1].event !=0)m_event_table[TIM2_EVENT1].event(TIM2_EVENT1);
			
			
			 if (m_event_table[TIM2_EVENT1].fire_and_forget){			 
					bsp_tim2_events_forget_event(TIM2_EVENT1); 
			 }
			}
		}
	}
	
	
	// ----------------
	// EVENT 2
	//-----------------
	
	if (((TIM2->DIER & TIM_DIER_CC2IE_Msk) ==TIM_DIER_CC2IE_Msk) && (TIM2->SR & TIM_SR_CC2IF_Msk) == TIM_SR_CC2IF_Msk){
		// clear flag
		TIM2->SR &= ~TIM_SR_CC2IF_Msk;
		
		// periodic event
		if (m_event_table[TIM2_EVENT2].trigger == 0){
				if (m_event_table[TIM2_EVENT4].event !=0)m_event_table[TIM2_EVENT2].event(TIM2_EVENT2);
		 	  calculate_next_ccr(TIM2_EVENT2);
		}
		// triggered event
		else {

			calculate_next_ccr(TIM2_EVENT2);			
			
			if (m_event_table[TIM2_EVENT2].trigger()){
				if (m_event_table[TIM2_EVENT2].event !=0)m_event_table[TIM2_EVENT2].event(TIM2_EVENT2);
			
			  if (m_event_table[TIM2_EVENT2].fire_and_forget){			 
					bsp_tim2_events_forget_event(TIM2_EVENT2); 
			 } 
			}
		}
	}
	
	
	// ----------------
	// EVENT 3
	//-----------------
	if (((TIM2->DIER & TIM_DIER_CC3IE_Msk) ==TIM_DIER_CC3IE_Msk) &&(TIM2->SR & TIM_SR_CC3IF_Msk) == TIM_SR_CC3IF_Msk){
		// clear flag
		TIM2->SR &= ~TIM_SR_CC3IF_Msk;
		
		// periodic event
		if (m_event_table[TIM2_EVENT3].trigger == 0){
				if (m_event_table[TIM2_EVENT3].event !=0) m_event_table[TIM2_EVENT3].event(TIM2_EVENT3);
		 	  calculate_next_ccr(TIM2_EVENT3);
		}
		// triggered event
		else {
				
			calculate_next_ccr(TIM2_EVENT3);
			
			if (m_event_table[TIM2_EVENT3].trigger()){
				
				if (m_event_table[TIM2_EVENT3].event !=0)m_event_table[TIM2_EVENT3].event(TIM2_EVENT3);
			
				if (m_event_table[TIM2_EVENT3].fire_and_forget){			 
					bsp_tim2_events_forget_event(TIM2_EVENT3); 
				} 
			}	
		}
	}
	
	
	// ----------------
	// EVENT 4
	//-----------------
	if (((TIM2->DIER & TIM_DIER_CC4IE_Msk) ==TIM_DIER_CC4IE_Msk) &&(TIM2->SR & TIM_SR_CC4IF_Msk) == TIM_SR_CC4IF_Msk){
		// clear flag
		TIM2->SR &= ~TIM_SR_CC4IF_Msk;
		
		// periodic event
		if (m_event_table[TIM2_EVENT4].trigger == 0){
				if (m_event_table[TIM2_EVENT4].event !=0)m_event_table[TIM2_EVENT4].event(TIM2_EVENT4);
		 	  calculate_next_ccr(TIM2_EVENT4);
		}
		// triggered event
		else {
			
			calculate_next_ccr(TIM2_EVENT4);
			
			if (m_event_table[TIM2_EVENT4].trigger()){
				 
				 if (m_event_table[TIM2_EVENT4].event !=0)m_event_table[TIM2_EVENT4].event(TIM2_EVENT4);
			
				 if (m_event_table[TIM2_EVENT4].fire_and_forget){			 
						bsp_tim2_events_forget_event(TIM2_EVENT4); 
				 }
			}
		}
	}
}
