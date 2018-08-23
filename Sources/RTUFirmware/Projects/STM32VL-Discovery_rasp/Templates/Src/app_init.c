/* ************************************************************************** */
// NAME:    app_init.c
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: Application specific init function	
/* ************************************************************************** */

#include "stm32f1xx_hal.h"
#include "stm32vl_discovery.h"
#include "app_init.h"
#include "app_callbacks.h"


// system clock configuration
static void SystemClock_Config(void);
static uint8_t hal_init(void);
static uint8_t bsp_init(void);
static uint8_t mdw_init(void);

/* ********************************************************************************************************************************************** */
/* ********************************************** SYSTEN INITIALIZATION & CONFIGURATION********************************************************** */
/* *********************************** In here HAL,BSP, Middleware & app specific driver are initialized ***************************************** */
/* ********************************************************************************************************************************************** */

/* ---------------------------------------------*/
// Initialize HAL driver
/* ---------------------------------------------*/
uint8_t hal_init(void){

	// initialize low-level peripherals & driver
  HAL_Init();
	
  // configure the system clock to 24 MHz 
  SystemClock_Config();
	
	return 0;
}


/* ---------------------------------------------*/
// Initialize BSP driver
/* ---------------------------------------------*/
uint8_t bsp_init(void){


	// initialize board LEDs
	BSP_LED_Init(LED_GREEN);
	BSP_LED_Init(LED_BLUE);
	
	// initialize uart1
	//bsp_uart1_init(UART1_BAUDRATE_DEFAULT);
	
	// initialize spi1 with dma
	bsp_spi1_dma_init(spi1_dma_tx_callback,spi1_dma_rx_callback);
	bsp_spi1_dma_set_rx_size(DMA_CMD_DATALEN+1);
	
  // initialize pwm 
	bsp_tim1_pwm_init();
  bsp_tim1_pwm_set_resolution(TIM1_PWM_RESOLUTION_10US);	
	bsp_tim1_pwm_set_period(100);
	bsp_tim1_pwm_set_duty_cycle(50);
  
	// wait a little bit
  HAL_Delay(3);
	
	// initialize frequency counters
	bsp_tim4_ic_init();
	bsp_tim4_ic_set_resolution(TIM4_IC_RESOLUTION_100US);
	bsp_tim4_ic_set_limit(TIM4_IC_MASK_ALL,1000);
	
	bsp_tim2_events_init();
	
	
	return 0;
} 


/* ---------------------------------------------*/
 // Initialize Middleware driver 
/* ---------------------------------------------*/
uint8_t mdw_init(void){
	
	// initialize & config modbus protocol	
	modbus_rtu_init(modbus_rx_wrapper, modbus_tx_wrapper);
	modbus_rtu_disable_callback_handler();
	modbus_register_init();
	
	return 0;
}


/* ---------------------------------------------*/
 // Application initialization
/* ---------------------------------------------*/
uint8_t app_init(void){
	
	// initialize hal driver
	hal_init();
	
	// initialize bsp driver
	bsp_init();
	
	// initialize middlware 
	mdw_init();
		
	// initialize debug
	debug_init(bsp_uart1_put_byte);
  
	return 0;
}

/* ---------------------------*/
 // Clock Configuration
/* ---------------------------*/
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/* ---------------------------*/
 // Systick Handler 
/* ---------------------------*/
void SysTick_Handler(void)
{
  HAL_IncTick();
}




