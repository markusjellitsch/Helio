//------------------------------------------------------------------------
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : testing waveform generator
// -----------------------------------------------------------------------

#include <QCoreApplication>
#include <spirpi.h>
#include <string>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>
#include <uart_rpi.h>
#include <stm32_vldisco.h>
#include <iomanip>
#include <Modbus/modbusrtu.h>
#include <Modbus/holdingregister.h>


using namespace std;


static uint8_t channel = 1;
static int16_t period =1000;
static int16_t duty = 50;
static int16_t counter = 0;
static int16_t frequency = 100;



/* parse arguments */
int parse_args(int argc, char *argv[]){


    if (argc >=2) channel = (uint8_t)atoi(argv[1]);
    if (argc >=3) period = atoi(argv[2]);
    if (argc >=4) duty = atoi(argv[3]);
    if (argc >=5) counter = atoi(argv[4]);

    return 0;
}


int main(int argc, char *argv[])
{

    if (parse_args(argc,argv)!=0){
         cout << "Usage: command <channel>(opt) <num>(opt)" << endl;
    }

    // spi config
    unsigned int speed = (unsigned int)(frequency * 1000);
    SPIRPi spi;
    SPIConfig_t  config = {SPI_RPI_CS1_FILENAME,speed,8,0};
    spi.setCount(5000);

    // initialize stm32
    STM32_VLDISCO stm32(&spi);

    stm32.dumpRegisters();
    stm32.getRegister(RTU_SYS_CR1);


    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

    stm32.updateRegisters();
    stm32.dumpRegisters();

    cout << "Connecting to STM32VL-Discovery ..." << endl;

    stm32.setPWMCount((uint16_t)counter);
    channel = 1<<(channel-1);
    if (stm32.startPWM(channel,(uint16_t)period,(uint8_t)duty) == 0){

        cout << "PWM started!" << endl;
    }
    else cout << "PWM not started!" << endl;

    return 0;
}

