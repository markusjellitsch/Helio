/* ************************************************************************** */
// NAME:    bsp_dac.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a driver for digital-analog-converter					
/* ************************************************************************** */
#ifndef BSP_DAC_H
#define BSP_DAC_H

#include <stdbool.h>
#include <stdint.h>


#define DAC_DEFAULT_VDDA 						3004.0
#define DAC_DEFAULT_RESOLUTION			4096.0
#define DAC_DEFAULT_DOR_VAL 				2100.0

// initialize dac interface
uint8_t bsp_dac_init(void);

// set voltage in millivolts
uint8_t bsp_dac_set_voltage(uint16_t const voltage);

// get voltage in millivolts
uint16_t bsp_dac_get_voltage(void);

#endif
