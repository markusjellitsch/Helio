/* ************************************************************************** */
// NAME:    app_run.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: Application specific run function. this function is called after
//              app_init()
/* ************************************************************************** */

#include "stm32f1xx_hal.h"
#include "stm32vl_discovery.h"

#include "app_callbacks.h"
#include "app_init.h"
#include "app_run.h"
#include "modbus_control.h"

static uint8_t app_new_command_available(void);
static uint8_t app_handle_new_command(void);
static void app_set_busy(void);
static void app_unset_busy(void);
static void app_update_status_register(uint8_t const event_nr);


void app_run(void){

	uint8_t success = 0;
	success = bsp_tim2_events_register_event(TIM2_EVENT3,0,app_update_status_register,0,1000);
	ASSERT(success);

	// enable rx channel on dma to receive data
	bsp_spi1_dma_enable_rx();
	
	// BSP_LED_Init(LED_BLUE);

	DEBUG_PRINT("STM32VL-Discovery booted succesfully!\n\n");
	
	while (1){
		
		DEBUG_PRINT("Waiting for new command...\n");
		
		// wait for new command
		while (!app_new_command_available());
				
		// indicate rtu is busy
		app_set_busy();
	
		// handle command
		app_handle_new_command();
		
	  // indicate rtu is ready for new commands
	  app_unset_busy();

	  g_flag_new_command = 0;		
				
	  SPI_ENABLE_IRQ;
		
	}
	
}


static uint8_t app_new_command_available(){

	// wait unitil interrupt occurs
	//__WFI();
	
	// disable spi communication to avoid new command
	SPI_DISABLE_IRQ;
	
	uint8_t tmp = g_flag_new_command;

	
	// enable spi interrupt
	if (!tmp){	  		
		SPI_ENABLE_IRQ;
}
	
	return tmp;
}

static uint8_t app_handle_new_command(){
    return modbus_rtu_callback_handler();
}

static void app_set_busy(){
	RTU_SYS->SR |= RTU_SYS_SR_BSY_Msk;
}

static void app_unset_busy(){
	RTU_SYS->SR &= ~RTU_SYS_SR_BSY_Msk;
}

static void app_update_status_register(uint8_t const event_nr){
	 
	DEBUG_PRINT("Event triggered: %d \n",event_nr);

	BSP_LED_Toggle(LED_BLUE);
}







