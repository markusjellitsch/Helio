/* ************************************************************************** */
// NAME:    		bsp_spi_dma.c
// AUTHOR:  		Markus Jellitsch
// DATE:    		09.1.2018
// DESCRIPTION: This module provides a SLAVE driver for spi interface with DMA.
//              All data to be transmitted & received is handled by DMA controller.						
/* ************************************************************************** */

#include "bsp_spi1_dma.h"
#include <stm32f1xx_hal.h>
#include <string.h>
#include <stm32f1xx_hal_dma.h>


// DMA transmit buffer  (tx)
static uint8_t m_spi_tx_buffer [SPI_TX_BUFFER_SIZE] = {0};
static uint32_t m_write_pointer = 1;

// DMA receive buffer (rx)
static uint8_t m_spi_rx_buffer [SPI_RX_BUFFER_SIZE] = {0};
static uint32_t m_read_pointer = SPI_RX_BUFFER_SIZE;
static uint16_t m_rx_size = SPI_RX_BUFFER_SIZE;

// callback function pointers for send & receive
static spi_dma_tx_callback_t m_fp_tx_complete =0;
static spi_dma_tx_callback_t m_fp_rx_complete = 0;



/* ------------------------------------------------------------
// Initialize SPI interface with DMA.
// After transmission or receive is completed, callback
// functions are called, if not ZERO.
// ----------------------------------------------------------*/
void bsp_spi1_dma_init(spi_dma_tx_callback_t const fp_tx, spi_dma_rx_callback_t const fp_rx)
{
	// set function pointers
	m_fp_tx_complete = fp_tx;
	m_fp_rx_complete = fp_rx;
	
	DMA_HandleTypeDef hdma_tx;
  DMA_HandleTypeDef hdma_rx;
	
	SPI_HandleTypeDef spi = { .Instance = SPI1 };
  
	// enable clocks 
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	// config spi1
	spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	spi.Init.Direction = SPI_DIRECTION_2LINES;
	spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi.Init.DataSize = SPI_DATASIZE_8BIT;
	spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi.Init.NSS = SPI_NSS_SOFT;
	spi.Init.TIMode = SPI_TIMODE_DISABLE;
	spi.Init.Mode = SPI_MODE_SLAVE; 
	if (HAL_SPI_Init(&spi) != HAL_OK)return;

	// config gpio pins CLK,MISO & MOSI
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // config CS pin. Connect to EXTI to trigger event on rising edge
	GPIO_InitStruct.Pin  = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	// config data ready pin
	GPIO_InitStruct.Pin       = GPIO_PIN_1;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // enable spi1
	__HAL_SPI_ENABLE(&spi);

  // config dma channel 3 for tx
  hdma_tx.Instance                 = DMA1_Channel3;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma_tx);

  // config dma channel 2 for tx
	hdma_rx.Instance                 = DMA1_Channel2;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_rx);
  
	
	NVIC_SetPriority(SPI1_IRQn,1);
	 
	// think if necessary
	__HAL_LINKDMA(&spi, hdmarx, hdma_tx);
	__HAL_LINKDMA(&spi, hdmatx, hdma_tx);
  
 // enable interrupt for tx
	NVIC_SetPriority(DMA1_Channel3_IRQn, 1);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    
  // enable interrupt for rx
  NVIC_SetPriority(DMA1_Channel2_IRQn, 1);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn); 
	
	// enable interrupt for event on CS
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI2_IRQn,1);
	
	// reset DATREADY pin
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);

  // disable software chipselect management
	CLEAR_BIT(SPI1->CR1,SPI_CR1_SSM);
	
	// enable 
	EXTI->IMR |= EXTI_IMR_MR2_Msk;
}


/* ------------------------------------------------------------
// Write one byte to the transmission buffer. 
// ----------------------------------------------------------*/
 uint8_t bsp_spi1_dma_put_byte(uint8_t  const data)
{	
	if  (m_write_pointer >= SPI_TX_BUFFER_SIZE) return 0;	
	m_spi_tx_buffer[m_write_pointer] = data;
	m_write_pointer = (m_write_pointer+1);
	
	return 1;
}


/* ------------------------------------------------------------
// Write multi bytes to the transmission buffer
// ----------------------------------------------------------*/
uint8_t bsp_spi1_dma_put_data(uint8_t* data,uint16_t len)
{	
	
	if (data == 0) return 0;
	
	while(len--)
	{
		if(!bsp_spi1_dma_put_byte(*data)){
			return 0;
		}
		data++;
	}
	
	return 1;
}


/* ------------------------------------------------------------
// Read a byte from the receive buffer
// ----------------------------------------------------------*/
uint8_t bsp_spi1_dma_get_byte(uint8_t* data)
{	
	if(data == NULL)
	{
		return 0;
	}
	
	if ((m_read_pointer < m_rx_size) && (m_read_pointer < SPI_RX_BUFFER_SIZE)){
		*data = m_spi_rx_buffer[m_read_pointer++];
		return true;
	}
	
	return 0;
}


/* ------------------------------------------------------------
// Read multi bytes from the receive buffer
// ----------------------------------------------------------*/
uint8_t bsp_spi1_dma_get_data(uint8_t* data,uint16_t const bytes){
		
	for (uint16_t i = 0; i< bytes;i++){
		
		if (!bsp_spi1_dma_get_byte(data++))
		{
			return false;
		}
	}
	
	return true;
 
}


/* ------------------------------------------------------------
// Prepare DMA rx transmission.
// ----------------------------------------------------------*/
uint8_t bsp_spi1_dma_set_rx_size(uint16_t const size){

	if (size > SPI_RX_BUFFER_SIZE)return 0;
	
	m_rx_size = size;
	
	return 1;
}

/* ------------------------------------------------------------
// Set the Data ready pin 
// ----------------------------------------------------------*/
uint8_t bsp_spi1_dma_set_ready(){


	return 1;
}
	


/* ------------------------------------------------------------
// Prepare DMA tx transmission.
// ----------------------------------------------------------*/
void bsp_spi1_dma_enable_tx(){
	
	if (m_write_pointer <=1) {
		return;
	}
	
	// set the data ready pin
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);

	// make sure SPI has dma enabled
	SET_BIT(SPI1->CR2, SPI_CR2_TXDMAEN);
	CLEAR_BIT(SPI1->CR2, SPI_CR2_RXDMAEN);	
	CLEAR_BIT(SPI1->CR1,SPI_CR1_RXONLY);
	
	// clear CCR first to disable dma.
	DMA1_Channel3->CCR &= ~DMA_CCR_EN_Msk;
  
	// write data len on the very first byte
	m_spi_tx_buffer[0] = (uint8_t)m_write_pointer-1;
	
	
	// set source & destination
	DMA1_Channel3->CMAR = (uint32_t)m_spi_tx_buffer;
	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;
	
	// set num bytes 
	DMA1_Channel3->CNDTR = m_write_pointer;	
	
	// enable transmission complete interrupt;
	DMA1->ISR = DMA_ISR_GIF3_Msk |DMA_ISR_TCIF3_Msk ;
	
	// clear interrupt flags
	DMA1->IFCR =DMA_IFCR_CGIF3_Msk | DMA_IFCR_CTCIF3_Msk | DMA_IFCR_CHTIF3_Msk;

  // clear status flags
	SPI1->SR;
	
	DMA1_Channel3->CCR |= DMA_CCR_EN_Msk | DMA_CCR_TCIE_Msk;
 
}

/* ------------------------------------------------------------
// Prepare DMA rx transmission.
// ----------------------------------------------------------*/
void bsp_spi1_dma_enable_rx(){
	
	if (m_rx_size == 0) {
		return;
	} 
	
	// reset pin
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	
	// disable tx
	CLEAR_BIT(SPI1->CR2, SPI_CR2_TXDMAEN);
	DMA1_Channel3->CCR &= ~DMA_CCR_EN_Msk;
	
	// make sure SPI has dma enabled
	SET_BIT(SPI1->CR2, SPI_CR2_RXDMAEN);	
	
	// clear CCR first to disable dma.
	DMA1_Channel2->CCR &= ~DMA_CCR_EN_Msk;
  
	// set source & destination
	DMA1_Channel2->CMAR = (uint32_t)m_spi_rx_buffer;
	DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR;
	
	// set num bytes 
	DMA1_Channel2->CNDTR = m_rx_size;	
	
	// enable transmission complete interrupt;
	DMA1->ISR = DMA_ISR_GIF2_Msk | DMA_ISR_TCIF2_Msk;
	
	// clear interrupt flags
	DMA1->IFCR =DMA_IFCR_CGIF2_Msk | DMA_IFCR_CTCIF2_Msk | DMA_IFCR_CHTIF2_Msk; 
  
	// clear status register & data register
	SPI1->DR;
	SPI1->SR;
	SPI1->DR = 0x00;
	
	// enable dma & interrupt
	DMA1_Channel2->CCR |= DMA_CCR_EN_Msk | DMA_CCR_TCIE_Msk;
}

/* ------------------------------------------------------------
// Write one byte to the transmission buffer. 
// ----------------------------------------------------------*/
void DMA1_Channel2_IRQHandler(void)
{
	// clear pending bits
	NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
	DMA1->IFCR =DMA_IFCR_CGIF2_Msk | DMA_IFCR_CHTIF2_Msk | DMA_IFCR_CHTIF2_Msk;
	
	// reset read pointer to start reading from beginning
	m_read_pointer = 0;
	
	// rx callback
	if (m_fp_rx_complete != 0) {
		if(m_fp_rx_complete()){
				bsp_spi1_dma_enable_tx();
		}
	}
}

/* ------------------------------------------------------------
// Write one byte to the transmission buffer. 
// ----------------------------------------------------------*/
void DMA1_Channel3_IRQHandler(void)
{
  // clear pending
	NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
	DMA1->IFCR =DMA_IFCR_CGIF3_Msk | DMA_IFCR_CHTIF3_Msk | DMA_IFCR_CHTIF3_Msk;
	
	m_write_pointer = 1;
	
	// callback tx 
	if (m_fp_tx_complete != 0){
			 
		  // retransmit when necessary
		  if(m_fp_tx_complete()){
				bsp_spi1_dma_enable_tx();
			}
			else {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			}
  }
	
}


/* ------------------------------------------------------------------
// Interrup handling for rising edge event on CS pin (Slave released)
//-----------------------------------------------------------------*/
void EXTI2_IRQHandler(){
	
	// clear pending
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	EXTI->PR |= EXTI_PR_PR2_Msk;
	// enable 
	EXTI->IMR |= EXTI_IMR_MR2_Msk;
 	
	// always restart in rx mode
	bsp_spi1_dma_enable_rx();
}
