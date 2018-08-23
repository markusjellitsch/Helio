/* ************************************************************************** */
// NAME:    		bsp_uart1.h
// AUTHOR:  		Markus Jellitsch
// DATE:    		09.1.2018
// DESCRIPTION:  UART1 driver module for stm32vl-discovery			
/* ************************************************************************** */

#ifndef UART1_DRIVER_H_
#define UART1_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

/* Definitions */
#define UART1_TX_BUFFER_SIZE				150
#define UART1_RX_BUFFER_SIZE				50

#define ENABLE_TE_INTERRUPT					USART1->CR1 |= USART_CR1_TXEIE;
#define DISABLE_TE_INTERRUPT				USART1->CR1 &= ~USART_CR1_TXEIE;

// initialize uart
uint8_t bsp_uart1_init(uint32_t const baudrate);

// write to uart tx buffer & send
void bsp_uart1_put_byte(uint8_t data);
void bsp_uart1_put_data(uint8_t* data, uint16_t len);
void bsp_uart1_put_string(uint8_t* data);

// read a byte from uart rx buffer
uint8_t bsp_uart1_get_byte(uint8_t* data);

// return if new data is available
uint8_t bsp_uart1_data_available(void);

#endif
