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
#include <Logger/stdlogger.h>
#include <ad9833.h>

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
    unsigned int speed = (unsigned int)(1000000.0 * 400.0/250.0);
    SPIRPi spi;
    SPIConfig_t  config = {SPI_RPI_CS0_FILENAME,speed,8,0};

    // logger
    StdLogger logger;

    // waveform generator
    AD9833 wave;
    wave.setLogger(&logger);
    wave.setInterface(&spi,(void *)&config);


    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

int success = 0;
    while (1){
    string c = "hallo welt!";

    success = spi.writeMulti(0,(uint8_t *)c.c_str() ,c.length()+1);

    uint8_t rx_bytes[100];
    success = spi.readMulti()
    sleep(1);

    }

    success = wave.setFrequency(frequency);
    success = wave.setOutputMode((Output_Mode_t)mode);

    sleep(1);


    if (success == -1){
        cout << "Error executing SPI!" << endl;
        return 0;
    }

    cout << "Success!" << endl;

    return 0;
}
