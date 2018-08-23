/* ************************************************************************** */
// NAME:    bsp_tim1_pwm.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for timer1 pwm generator						
/* ************************************************************************** */

#ifndef BSP_TIM1_INPUT_CAPTURE_H
#define BSP_TIM1_INPUT_CAPTURE_H

#include <stdbool.h>
#include <stdint.h>

#define TIM1_PWM_CLOCK_FREQUENCY    24000000 

#define TIM1_PWM_RESOLUTION_BASE		 (TIM1_PWM_CLOCK_FREQUENCY / 1000000)
#define TIM1_PWM_RESOLUTION_1US      (TIM1_PWM_RESOLUTION_BASE)
#define TIM1_PWM_RESOLUTION_10US     (TIM1_PWM_RESOLUTION_BASE*10)
#define TIM1_PWM_RESOLUTION_100US    (TIM1_PWM_RESOLUTION_BASE*100)
#define TIM1_PWM_RESOLUTION_1MS      (TIM1_PWM_RESOLUTION_BASE*1000)

#define TIM1_PWM_CHANNEL1    	0x01
#define TIM1_PWM_CHANNEL2    	0x02
#define TIM1_PWM_CHANNEL3    	0x03
#define TIM1_PWM_CHANNEL4    	0x04

#define TIM1_PWM_MASK_ALL         0x0F
#define TIM1_PWM_MASK_CHANNEL1    0x01
#define TIM1_PWM_MASK_CHANNEL2    0x02
#define TIM1_PWM_MASK_CHANNEL3    0x04
#define TIM1_PWM_MASK_CHANNEL4    0x08

#define TIM1_PWM_COUNT_DEFAULT 		1000

// for this board it is 1 MHz
#define TIM1_PWM_PRESCALER_DEFAULT 24 

// initialize timer3 for input capturing
uint8_t bsp_tim1_pwm_init(void);

// start capturing on a one or more channels
uint8_t bsp_tim1_pwm_start(uint8_t const channel_mask);

// set timer resolution (=prescaler value)
uint8_t bsp_tim1_pwm_set_resolution(uint16_t const prescaler);

// set the pwm period 
uint8_t bsp_tim1_pwm_set_period(uint16_t const period);

// set pwm  duty cycle 
uint8_t bsp_tim1_pwm_set_duty_cycle(uint8_t const duty);

// getter & setter for count
uint8_t bsp_tim1_pwm_set_count(uint8_t const channel_mask, uint16_t const count);
uint16_t bsp_tim1_pwm_get_count(uint8_t const channel_mask);

// stop capturing on a specific channel
uint8_t bsp_tim1_pwm_stop(uint8_t const channel_mask);

// return if a channel is enabled
uint8_t bsp_tim1_pwm_is_enabled(uint8_t const channel);


#endif
