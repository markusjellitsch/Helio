/* ************************************************************************** */
// NAME:    		bsp_uart1.c
// AUTHOR:  		Markus Jellitsch
// DATE:    		09.1.2018
// DESCRIPTION: UART1 driver module for stm32vl-discovery				
/* ************************************************************************** */

#include "bsp_uart1.h"
#include <stm32f1xx_hal.h>
#include <string.h>

// rx buffer
static uint8_t m_uart_rx_buffer[UART1_RX_BUFFER_SIZE] = {0};
static uint32_t m_rx_head = 0;
static uint32_t m_rx_tail = 0;

// tx buffer
static uint8_t m_uart_tx_buffer [UART1_TX_BUFFER_SIZE] = {0};
static uint32_t m_tx_head = 0;
static uint32_t m_tx_tail = 0;

/* --------------------------------------------------*/
// Initialize UART1 with specified baudrate
/* --------------------------------------------------*/
uint8_t bsp_uart1_init(uint32_t const baudrate)
{
  
	// enable clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();

	//Enable Usart
	USART1->CR1 |= USART_CR1_UE;	
	//set 2 stop bits
	USART1->CR2 |= USART_CR2_STOP_1;
	
	//set baudrate
	USART1->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK2Freq(), baudrate);
	//enable receiver and transmitter and rxne interrupt
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;	
	
	// config gpio tx
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_9;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&gpio);
	
	// config gpio rx
	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_10;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&gpio);
	
	// enable nvic interupt
	NVIC_SetPriority(USART1_IRQn,4);
	NVIC_EnableIRQ(USART1_IRQn);
  
  return 1;	
}

/* --------------------------------------------------*/
// Send a single byte
/* --------------------------------------------------*/
void bsp_uart1_put_byte(uint8_t data)
{	

	// disable interrupt
	NVIC_DisableIRQ(USART1_IRQn);	
	
	// write byte to tx buffer
	m_uart_tx_buffer[m_tx_head] = data;
	m_tx_head = (m_tx_head+1) % UART1_TX_BUFFER_SIZE;	
	
	// enable transmitter empty interrupt
	ENABLE_TE_INTERRUPT
	
	// enable interrupt
	NVIC_EnableIRQ(USART1_IRQn);	
}


/* --------------------------------------------------*/
// Send a string
/* --------------------------------------------------*/
void bsp_uart1_put_string(uint8_t* data)
{	
	uint32_t len = strlen((char*)data);
	
	while(len--)
	{
		// write each single byte
		bsp_uart1_put_byte(*data);
		data++;
	}
}

/* --------------------------------------------------*/
// Send multiple data 
/* --------------------------------------------------*/
void bsp_uart1_put_data(uint8_t* data,uint16_t len)
{	
	while(len--)
	{
		// write each single byte
		bsp_uart1_put_byte(*data);
		data++;
	}
}

/* --------------------------------------------------*/
// Sreturn if new rx data is available
/* --------------------------------------------------*/
uint8_t bsp_uart1_data_available(){
		return (m_rx_head != m_rx_tail);
}


/* --------------------------------------------------*/
// Return byte
/* --------------------------------------------------*/
uint8_t bsp_uart1_get_byte(uint8_t* data)
{	
	if(data == NULL){
		return 0;
	}
	
	// exit when no new data available
	if (m_rx_head == m_rx_tail) return false;

	//read byte from buffer within critical section	
	NVIC_DisableIRQ(USART1_IRQn);	
	
	// return byte
	*data = m_uart_rx_buffer[m_rx_tail];
	m_rx_tail = (m_rx_tail+1) % UART1_RX_BUFFER_SIZE;	
	
	NVIC_EnableIRQ(USART1_IRQn);
	
	return 1;
}

/* --------------------------------------------------*/
// Interrupt Handler
/* --------------------------------------------------*/
void USART1_IRQHandler(void)
{
	
		NVIC_ClearPendingIRQ(USART1_IRQn);
	
	//-----------------------------------------------
	// receiver buffer is not empty -> new data 
	//-----------------------------------------------
	if( USART1->SR & USART_SR_RXNE)
	{	
		uint32_t newHead = (m_rx_head+1) % UART1_RX_BUFFER_SIZE;
		
		//check if rx buffer is full
		if(newHead != m_rx_tail)
		{
			// write received data to rx buffer
			m_uart_rx_buffer[m_rx_head] = USART1->DR;
			m_rx_head = newHead;
		}
			else  USART1->DR;
	}	
	
	//-----------------------------------------------
	// transmitter empty -> can send new data
	//-----------------------------------------------
	if(USART1->SR & USART_SR_TXE)
	{
		if(m_tx_tail != m_tx_head)
		{						
			USART1->DR = m_uart_tx_buffer[m_tx_tail];
			m_tx_tail = (m_tx_tail+1) % (UART1_TX_BUFFER_SIZE);
		}
		else
		{
			// transmission finished -> disable tx
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
	}	
	


}

