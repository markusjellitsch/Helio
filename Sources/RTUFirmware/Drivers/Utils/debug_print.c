/* ************************************************************************** */
// NAME:    debug_print.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: This module provides a debug functionality for stm32 project		
/* ************************************************************************** */

#include "debug_print.h"
#include  <errno.h>
#include "stdio.h"

static fp_debug_send_t m_fp_send;


// initialize the function pointer for sending debug info
void debug_init(fp_debug_send_t const fp_send){
	m_fp_send = fp_send;
}


void debug_dummy(char * data,...){}

__weak void debug_assert_failed(uint8_t * file, uint32_t const line){
	printf("Assert fail in file %s Line %d",file,line);
	while(1);
}


struct __FILE {
    int dummy;
};
 
/* You need this if you want use printf */
/* Struct FILE is implemented in stdio.h */
FILE __stdout;
 
int fputc(int ch, FILE *f) {
    /* Do your stuff here */
    /* Send your custom byte */
    /* Send byte to USART */
    
	 if (m_fp_send !=0) m_fp_send((uint8_t)ch);
    
    /* If everything is OK, you have to return character written */
    return ch;
    /* If character is not correct, you can return EOF (-1) to stop writing */
    //return -1;
}


