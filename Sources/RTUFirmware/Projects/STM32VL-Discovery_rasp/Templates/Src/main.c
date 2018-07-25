/*************************************************************************** */
// NAME:    main.h
// AUTHOR:  Markus Jellitsch
// DATE:    09.1.2018
// DESCRIPTION: main 
/* ************************************************************************** */

#include "main.h"
#include "bsp_dac.h"
int main(void)
{
	
	/* initialize & configure system */
	app_init();
	
	bsp_dac_init();
	

	volatile uint16_t volt = bsp_dac_get_voltage();
	
	if (volt){
		bsp_dac_set_voltage(2000);
	}
		
		
	while (1)
  {
	  
		for (uint16_t i = 0; i< 30;i++){
				bsp_dac_set_voltage(i*100);
				HAL_Delay(1);
		}
		
		for (uint16_t i = 0; i<30;i++){
				bsp_dac_set_voltage(3000-i*100);
				HAL_Delay(1);
		}
		
		}

	/* run application */
	app_run();
	
	/* this loop is never reached! */

}

