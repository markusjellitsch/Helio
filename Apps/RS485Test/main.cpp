// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.11.2017
// DESCRIPTION  : Simple Clear & Write functionality for SSD1306 display
// **********************************************************************

#include <QCoreApplication>
#include <iostream>
#include <string>
#include <rs485_rpi.h>
#include <Heti/heti.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

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

    RS485Config_t config ={RS485_RPI_DEV_FILENAME,RS485_RPI_DEFAULT_BAUDRATE};
    RS485Rpi RS485;

    if (argc >= 3) config.deviceName = argv[1];
    int success = RS485.openInterface((void *)&config);
    if (success != I_OK){

        cout << "Couldn't open RS485 Inteface!" << endl;
        return 1;
    }


    string const devFile(config.deviceName);

    HETI heti(&RS485);


    cout << "Opend RS485 device:" << devFile << endl;
    cout << "Baudrate:" << config.baudRate << endl;
    cout << "Mode:";
    unsigned char buffer[RS485_RPI_BUFFERSIZE] = {0};

    ModbusRTU rtu;
    ModbusFrame_t frame = rtu.createFrame(0x11,0x02,buffer,1);

    while (1){
        if (RS485.readMulti(0,buffer,5) != I_NOK){
            for (int i = 0;i<5;i++)cout << buffer[i];
            cout << endl;
        }
        else cout << "no data read!" <<endl;


        sleep(1);
    }



   cout << endl;

 return 0;
}
