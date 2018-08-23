////////////////////////////////////////////////////////////////////////
// Uart Driver, interrupt driven
// (c) Gandler Martin
// last modified: 02.05.2016
////////////////////////////////////////////////////////////////////////


#include "bsp_spi.h"
#include <stm32f1xx_hal.h>
#include <string.h>



static SPI_HandleTypeDef spi = { .Instance = SPI1 };
// receive buffer
static uint8_t spi_rx_buffer [SPI_RX_BUFFER_SIZE] = {0};
static uint32_t rx_buffer_head = 0;
static uint32_t rx_buffer_tail = 0;

// transmission buffer
static uint8_t spi_tx_buffer [SPI_TX_BUFFER_SIZE] = {0};
static uint32_t tx_buffer_head = 0;
static uint32_t tx_buffer_tail = 0;

// send 1 byte
void bsp_spi1_put_char(uint8_t data);

// read byte from spi data buffer
uint8_t bsp_spi1_get_char(uint8_t* data);


void bsp_spi1_init(uint8_t mode)
{
  
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	spi.Init.Direction = SPI_DIRECTION_2LINES;
	spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi.Init.DataSize = SPI_DATASIZE_8BIT;
	spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi.Init.NSS = SPI_NSS_SOFT;
	spi.Init.TIMode = SPI_TIMODE_DISABLE;
	spi.Init.Mode = SPI_MODE_SLAVE; 
	if (HAL_SPI_Init(&spi) != HAL_OK){
		return;
	}

	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin  = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_SPI_ENABLE_IT(&spi,SPI_IT_RXNE);
	__HAL_SPI_ENABLE(&spi);

	 NVIC_EnableIRQ(SPI1_IRQn);		 
}


 void bsp_spi1_put_char(uint8_t data)
{	
	NVIC_DisableIRQ(SPI1_IRQn);	
	
	spi_tx_buffer[tx_buffer_head] = data;
	tx_buffer_head = (tx_buffer_head+1) % SPI_TX_BUFFER_SIZE;	
	
	NVIC_EnableIRQ(SPI1_IRQn);	
}



void bsp_spi1_put_data(uint8_t* data,uint16_t len)
{	
	while(len--)
	{
		bsp_spi1_put_char(*data);
		data++;
	}
}

bool bsp_spi1_data_available(){
		return (rx_buffer_head != rx_buffer_tail);
}


uint8_t bsp_spi1_get_char(uint8_t* data)
{	
	if(data == NULL)
	{
		return false;
	}
	
	if (rx_buffer_head == rx_buffer_tail) return false;

	//yeeah i got it
	//read byte from buffer within critical section	
	NVIC_DisableIRQ(SPI1_IRQn);	
	
	*data = spi_rx_buffer[rx_buffer_tail];
	rx_buffer_tail = (rx_buffer_tail+1) % SPI_RX_BUFFER_SIZE;	
	
	NVIC_EnableIRQ(SPI1_IRQn);
	
	return true;
}

void bsp_spi1_disable_rx(){
	__HAL_SPI_DISABLE_IT(&spi,SPI_IT_RXNE);
}


void bsp_spi1_enable_rx(){
	__HAL_SPI_ENABLE_IT(&spi,SPI_IT_RXNE);
}

void bsp_spi1_enable_tx(){
		__HAL_SPI_ENABLE_IT(&spi,SPI_IT_TXE);
}
void bsp_spi1_disable_tx(){
	__HAL_SPI_DISABLE_IT(&spi,SPI_IT_TXE);
}

uint16_t bsp_spi1_get_unread_bytes(void){
		if (rx_buffer_head <= rx_buffer_tail) return (rx_buffer_tail-rx_buffer_head);
	  else return (rx_buffer_head - rx_buffer_tail);
}

//---UART Interrupt Handler------------------------------------
void SPI1_IRQHandler(void)
{
	
	//-----------------------------------------------
	//Receiver buffer is not empty -> new data 
	//-----------------------------------------------
	if(((SPI1->SR & SPI_SR_TXE_Msk) == SPI_SR_TXE_Msk) &&__HAL_SPI_GET_FLAG(&spi,SPI_FLAG_RXNE))
	{	
		uint32_t newHead = (rx_buffer_head+1) % SPI_RX_BUFFER_SIZE;
		//check if rx buffer is full
		if(newHead != rx_buffer_tail)
		{
			spi_rx_buffer[rx_buffer_head] = SPI1->DR;		
			rx_buffer_head = newHead;			
		}
			else  SPI1->DR;
		
	}
	
	//-----------------------------------------------
	//Transmitter empty -> can send new data
	//----------------------------------------------
	if(((SPI1->SR & SPI_SR_TXE_Msk) == SPI_SR_TXE_Msk) &&__HAL_SPI_GET_FLAG(&spi,SPI_FLAG_TXE) )
	{
		
	 if(((SPI1->SR & SPI_SR_BSY_Msk) == SPI_SR_BSY_Msk) && tx_buffer_tail != tx_buffer_head)
		{						
			SPI1->DR = spi_tx_buffer[tx_buffer_tail];
			tx_buffer_tail = (tx_buffer_tail+1) % (SPI_TX_BUFFER_SIZE);
		}
		else 
		{
			SPI1->DR = 0x00;
			bsp_spi1_disable_tx();
		}
	}	
	//-----------------------------------------------
 	NVIC_ClearPendingIRQ(SPI1_IRQn);
}


//-----------------------------------------------------------
