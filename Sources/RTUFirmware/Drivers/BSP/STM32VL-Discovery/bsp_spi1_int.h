////////////////////////////////////////////////////////////////////////
// Uart Driver, interrupt driven
// (c) Gandler Martin
// last modified: 02.05.2016
////////////////////////////////////////////////////////////////////////

#ifndef bsp_spi1_H
#define bsp_spi1_H

#include <stdbool.h>
#include <stdint.h>


#define SPI_RX_BUFFER_SIZE	100
#define SPI_TX_BUFFER_SIZE  100

// initialize spi interface
void bsp_spi1_init(uint8_t mode);

// send multi data 
void bsp_spi1_put_data(uint8_t* data, uint16_t len);

// send 1 byte
void bsp_spi1_put_char(uint8_t data);

// read byte from spi data buffer
uint8_t bsp_spi1_get_char(uint8_t* data);

// return if new rx data is available
bool bsp_spi1_data_available(void);

void bsp_spi1_disable_rx(void);
void bsp_spi1_enable_rx(void);

void bsp_spi1_enable_tx(void);
void bsp_spi1_disable_tx(void);

uint16_t bsp_spi1_get_unread_bytes(void);



#endif
