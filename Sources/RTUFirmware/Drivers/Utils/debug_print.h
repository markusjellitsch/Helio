/* ************************************************************************** */
// NAME:    debug_print.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a debug functionality for stm32 project		
/* ************************************************************************** */


#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <stdint.h>
#include <stdio.h>

typedef void (*fp_debug_send_t)(uint8_t byte);

void debug_init(fp_debug_send_t const fp_send);

void debug_dummy(char * data,...);
	

#ifdef DEBUG
	#define DEBUG_PRINT printf
#else
	#define DEBUG_PRINT debug_dummy
#endif

#define ASSERT(x)   														\
{                   									 					\
		if (x != 1){																\
			debug_assert_failed((uint8_t *)__FILE__,__LINE__); 	\
		} 																					\
	}		


void debug_assert_failed(uint8_t * file, uint32_t const line);

#endif
