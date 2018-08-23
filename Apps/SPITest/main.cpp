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



using namespace std;


static int mode = 0;
static int frequency =1;


/* parse arguments */
int parse_args(int argc, char *argv[]){

    if (argc >=2) frequency = atoi(argv[1]);
    if (argc >=3) mode = atoi(argv[2]);

    return 0;
}



int main(int argc, char *argv[])
{

    if (parse_args(argc,argv)!=0){
         cout << "Usage: command <channel>(opt) <num>(opt)" << endl;
    }

    // spi config
    unsigned int speed = (unsigned int)(100000.0 * 400.0/250.0);
    SPIRPi spi;
    SPIConfig_t  config = {SPI_RPI_CS1_FILENAME,speed,8,0};

    spi.setCount(5000);

    UARTConfig_t uartConfig;
    uartConfig.baudRate = UART_RPI_BAUD115200;
    uartConfig.deviceName = UART_RPI_DEV_FILENAME;
    UARTRpi uart;

    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

    int success = 0;
    while (1){

           uint8_t spi_data_rx[100] ={0};
           uint8_t uart_data_rx[100] ={0};
           uint8_t spi_data_tx[100] ={0};
           uint8_t uart_data_tx[100] ={0};


        // receive data on uart
        int num_bytes = uart.readMulti(0,uart_data_rx,100);
        if (num_bytes == -1) break;

        // write to uart and wait for data
        num_bytes =     spi.startTransaction(uart_data_rx,num_bytes,spi_data_rx,20);

        if(num_bytes != I_NOK && num_bytes == 7){
            success = uart.writeMulti(0,spi_data_rx,num_bytes);

        }
        else {

            uart.writeMulti(0,(unsigned char*)"invalid!",7);
        }

    }



    if (success == -1){
        cout << "Error executing SPI!" << endl;
        return 0;
    }

    cout << "Success!" << endl;

    return 0;
}
