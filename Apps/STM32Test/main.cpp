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


static char cmd = 0;
static int reg =1;
static int16_t val = 1;
static int frequency = 100;


/* parse arguments */
int parse_args(int argc, char *argv[]){


    if (argc >=2) cmd = argv[1][0];
    if (argc >=3) reg = atoi(argv[2]);
    if (argc >=4) val = atoi(argv[3]);
    if (argc >=5) frequency = atoi(argv[4]);

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

    // holding register
    ModbusHoldingRegister regs(val+reg);

    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

    cout << "Connecting to STM32VL-Discovery ..." << endl;

    int success = 0;
    int16_t value = 0;

    switch (cmd){

        case 'r':
            success =  stm32.readSingleRegister(reg,&value);
            if (success == I_OK)
                cout << "reading holding register: OK" << endl << "Value: "<< value <<endl;
            else
                cout << "reading holding register: NOK"<< endl;
        break;


       case 'm':
            success =  stm32.readMultiRegister(reg,val,&regs);
            if (success == I_OK)
                regs.dump();
            else
                cout << "reading holding register: NOK"<< endl;
        break;


    case 'd':
        success =  stm32.dumpRegisters(1);
        if (success == I_OK)
             cout << "reading holding register: OK"<< endl;
        else
            cout << "reading holding register: NOK"<< endl;

    break;

    case 'w':
        success =  stm32.writeSingleRegister(reg,val);
        if (success ==I_OK){
            cout << "writting holding register: OK" <<endl;
        }
        else cout << "writting holding register: NOK" <<endl;
    break;

    default:
        cout << "command not allowed!" <<endl;
    }

    cout << "Finished!" << endl;

    return 0;
}
