/* ************************************************************************** */
// NAME:    app_callback.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: Application specific callbacks & wrapper functions
/* ************************************************************************** */

#ifndef APP_CALLBACKS_H
#define APP_CALLBACKS_H

#include "app_globals.h"


// modbus write holding callbacks
uint8_t app_callback_pwm_cr(uint8_t const function_code, int16_t const reg_val);
uint8_t app_callback_pwm_sr(uint8_t const function_code, int16_t const reg_val);

uint8_t app_trigger_cnt1_finished(void);
uint8_t app_trigger_cnt2_finished(void);
uint8_t app_trigger_cnt3_finished(void);
uint8_t app_trigger_cnt4_finished(void);

// timer event callbacks
void app_event_pwm_finished(uint8_t const event_nr);
void app_event_frequency_captured(uint8_t event_nr);


void app_event_cnt1_finished(uint8_t const event_nr);
void app_event_cnt2_finished(uint8_t const event_nr);
void app_event_cnt3_finished(uint8_t const event_nr);
void app_event_cnt4_finished(uint8_t const event_nr);

uint8_t app_trigger_cnt1_finished(void);
uint8_t app_trigger_cnt2_finished(void);
uint8_t app_trigger_cnt3_finished(void);
uint8_t app_trigger_cnt4_finished(void);


// dma callbacks for rx&tx
uint8_t spi1_dma_rx_callback(void);
uint8_t spi1_dma_tx_callback(void);

// modbus wrapper 
uint8_t modbus_tx_wrapper(uint8_t * byte);
void modbus_rx_wrapper(uint8_t byte);

// initialize modbus registers
void modbus_register_init(void);


#endif
