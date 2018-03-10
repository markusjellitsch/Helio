// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.11.2017
// DESCRIPTION  : Simple Clear & Write functionality for SSD1306 display
// **********************************************************************

#include <QCoreApplication>
#include <iostream>
#include <string>
#include <uart_rpi.h>
#include <string.h>
#include <stdio.h>

using namespace std;

#define ARG_COMMAND     1
#define ARG_LINE0       2
#define ARG_LINE1       3
#define ARG_LINE2       4
#define ARG_LINE3       5

/*--------------------------------------------------
 * Simple clear & write program for
 * the SSSD1306 Oled Display!
 *
 * This program takes arguments from command
 * line and runs one of following commands:
 *          => clear display
 *          => write to display
 *
 * This program can take 4 additional params. Each
 * param is the text for a line (0-3)
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UARTConfig_t config ={UART_RPI_DEV_FILENAME,UART_RPI_DEFAULT_BAUDRATE};
    UARTRpi uart;

    if (argc >= 2) config.deviceName = argv[1];
    int success = uart.openInterface((void *)&config);
    if (success != I_OK){

        cout << "Couldn't open UART Inteface!" << endl;
        return 1;
    }


    string const devFile(config.deviceName);

    cout << "Opend UART device:" << devFile << endl;
    cout << "Baudrate:" << config.baudRate << endl;
    cout << "Mode:";
    unsigned char buffer[UART_RPI_BUFFERSIZE] = {0};

    // UART WRITE
    if (argc >= 3){

        cout << "TX!" << endl;
        string message(argv[2]);
        cout << "Sending message:" <<message << endl;
        unsigned int len = strlen(argv[2]);
        if (len > UART_RPI_BUFFERSIZE-1) len =UART_RPI_BUFFERSIZE-1;

        memcpy(buffer,argv[2],len);
        buffer[len] = '\n';

        if (uart.writeMulti(0,buffer,len+1) == I_NOK){
            cout << "Couldn't send message!" << endl;
            return 2;
        }
            cout << "Sent message (" <<len << " bytes)" <<  endl;
    }

    // UART READ
    else{
        cout << "RX" << endl;
        cout << "Waiting for message..." << endl;
        int count = uart.readMulti(0,buffer,UART_RPI_BUFFERSIZE);
        if(count == I_NOK){
            cout << "Couln't receive any message" << endl;
            return 1;
        }
        else {

            cout << "Message received ("<<count << " bytes):";
            for (int i = 0; i < count;i++) cout << buffer[i];
            cout << endl;
        }
    }


   cout << endl;

 return 0;
}
