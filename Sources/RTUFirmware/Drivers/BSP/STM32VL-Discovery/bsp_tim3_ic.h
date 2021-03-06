/* ************************************************************************** */
// NAME:    bsp_tim4_ic.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for timer3 input capture						
/* ************************************************************************** */
#ifndef BSP_tim4_INPUT_CAPTURE_H
#define BSP_tim4_INPUT_CAPTURE_H

#include <stdbool.h>
#include <stdint.h>

#define ISSET(reg,bit) 		((reg & bit) == bit)

#define TIM4_IC_CLOCK_FREQUENCY    24000000 

#define TIM4_IC_RESOLUTION_BASE			(TIM4_IC_CLOCK_FREQUENCY / 1000000)
#define TIM4_IC_RESOLUTION_1US      (TIM4_IC_RESOLUTION_BASE)
#define TIM4_IC_RESOLUTION_10US     (TIM4_IC_RESOLUTION_BASE*10)
#define TIM4_IC_RESOLUTION_100US    (TIM4_IC_RESOLUTION_BASE*100)
#define TIM4_IC_RESOLUTION_1MS      (TIM4_IC_RESOLUTION_BASE*1000)

#define TIM4_IC_DEFAULT_RESOLUTION  TIM4_IC_RESOLUTION_10US

#define TIM4_IC_NUM_CHANNELS 4

#define TIM4_IC_CHANNEL1    			0x00
#define TIM4_IC_CHANNEL2    			0x01
#define TIM4_IC_CHANNEL3    			0x02
#define TIM4_IC_CHANNEL4    			0x03

#define TIM4_IC_MASK_ALL					0x0F            
#define TIM4_IC_MASK_CHANNEL1    	0x01
#define TIM4_IC_MASK_CHANNEL2    	0x02
#define TIM4_IC_MASK_CHANNEL3    	0x04
#define TIM4_IC_MASK_CHANNEL4    	0x08

#define TIM4_IC_LIMIT_DEFAULT 		1000

typedef struct{
	uint16_t first;
	uint16_t second;
}frequency_info_t;

// initialize timer3 for input capturing
uint8_t bsp_tim4_ic_init(void);

// deinitialize peripheral
uint8_t bsp_tim4_ic_deinit(void);

// start capturing on a one or more channels
uint8_t bsp_tim4_ic_start_capture(uint8_t const channel_mask);

// stop capturing on a specific channel
uint8_t bsp_tim4_ic_stop_capture(uint8_t const channel_mask);

// set prescaler for timer
uint8_t	bsp_tim4_ic_set_resolution(uint16_t const prescaler);

// set the limit until input capure stops counting
uint8_t bsp_tim4_ic_set_limit(uint8_t const channel_mask,uint16_t const limit);

// return if num of counts was capured
uint8_t bsp_tim4_ic_is_captured(uint8_t const channel_mask);

// get num of captures
uint16_t bsp_tim4_ic_get_count(uint8_t const channel);

// get last captured frequency
uint16_t bsp_tim4_ic_get_frequency(uint8_t const channel);

#endif
