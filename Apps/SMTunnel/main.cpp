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
#include <Modbus/modbusholdingregister.h>


using namespace std;


static int mode = 0;
static int frequency =1;


/* parse arguments */
int parse_args(int argc, char *argv[]){

    if (argc >=2) frequency = atoi(argv[1]);
    if (argc >=3) mode = atoi(argv[2]);

    return 0;
}


void spi_uart_bridge(){

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

    STM32_VLDISCO stm32(&spi);

    ModbusHoldingRegister regs;


    UARTConfig_t uartConfig;
    uartConfig.baudRate = UART_RPI_BAUD115200;
    uartConfig.deviceName = UART_RPI_DEV_FILENAME;
    UARTRpi uart;

    // open uart interface
    if(uart.openInterface((void*)&uartConfig)==-1){
        cout << "Couldn't open UART!" << endl;
        return 0;
    }


    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

    cout << "Start listening on UART (" << uartConfig.deviceName << ") ..." << endl;

    int success = 0;
    size_t const cBufferSize = 1024;

    uint8_t finish_frame[8] ={0x11,0x06,0x00,0x0A,0x00,0x01,0x6A,0x98};
    string finish_string;
    finish_string.append((char *)finish_frame,0,8);
    while (true){
         cout << endl;
        uint8_t spi_data[cBufferSize] ={0};
        uint8_t uart_data[cBufferSize] ={0};

        // receive data on uart
        int num_bytes = uart.readMulti(0,uart_data,cBufferSize);
        if (num_bytes == -1) break;

        string uart_rx_str;
        uart_rx_str.append((char*)uart_data,0,num_bytes);


        //if (uart_rx_str == finish_string) break;
        cout << "MODBUS (UART) received!  Payload:" << hex;
        for (int i = 0; i< num_bytes;i++){
            cout << setw(2)<< setfill('0')<< (int) uart_data[i] <<  " ";
        }
        cout << endl << "(" <<num_bytes<< " Bytes)"<<  endl;


        // write to uart and wait for data
        num_bytes =     stm32.transaction(uart_data,num_bytes,spi_data);
         cout << "STM32 (SPI) received!  Payload:" << hex;
         for (int i = 0; i< num_bytes;i++){
             cout << setw(2)<< setfill('0')<< (int) spi_data[i] <<  " ";
         }
         cout << endl << "(" <<num_bytes<< " Bytes)"<<  endl;

         ModbusRTU frame;
         frame.setPayloadSize(num_bytes-4);
         uint8_t i = frame.parseData(spi_data,num_bytes);

          cout << endl;

         if (i){
            ModbusFrame_t resp;
           frame.getFrame(&resp);

           if (resp.functionCode == 0x06){
               WriteHoldingRegisterResponse_t write = *(WriteHoldingRegisterResponse_t *)resp.data;
               SWAP16(write.regAddress);

               SWAP16(write.regValue);
               regs.write(write.regAddress,write.regValue);
           }
          }

            regs.dump();
            success = uart.writeMulti(0,spi_data,num_bytes);
}


    cout << "Finished!" << endl;

    return 0;
}
