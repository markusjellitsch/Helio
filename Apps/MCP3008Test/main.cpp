// ------------------------------------------------------
//


#include <QCoreApplication>
#include <spirpi.h>
#include <string>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <mcp3008.h>
#include <Logger/stdlogger.h>

using namespace std;


static int channel = 0;
static int num =1;


/* parse arguments */
int parse_args(int argc, char *argv[]){

    if (argc >=2) channel = atoi(argv[1]);
    if (argc >=3) num = atoi(argv[2]);

    return 0;
}



int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    if (parse_args(argc,argv)!=0){
         cout << "Usage: command <channel>(opt) <num>(opt)" << endl;
    }

    unsigned int speed = (unsigned int)(1000000.0 * 400.0/250.0);
    SPIRPi spi;
    SPIConfig_t  config = {SPI_RPI_CS0_FILENAME,speed,8};


    MCP3008 adc;
    adc.setInterface(&spi,&config);
    StdLogger logger;

    double const c_ref_voltage = 4760;
    double const c_resolution = 1024;

    adc.setRefVoltage(c_ref_voltage);
    adc.setResolution(c_resolution);
    adc.setSlaveAddress(SPI_RPI_CHIPSELECT_0);

    adc.setLogger(&logger);
    logger.setLoggingOption(false,false,false);

    int ret = 0;

   while (num--){


     if (channel == MCP3008_NUM_CHANNELS){

        adc.readAll();

        for (int i = 0; i < MCP3008_NUM_CHANNELS;i++){

            unsigned int raw =adc.getRaw(i);
            double voltage = adc.getVoltage(i);

            cout << "Reading ADC..." << endl;
            cout << "Channel:" << i << endl;
            cout << "RAW:" << raw << endl;
            cout << "Voltage:"  << voltage << "mV" <<endl;
            cout << endl;
          }
    }
    else{

            if (adc.readChannel(channel) != SLAVE_OK){

                cout << "Wrong channel!" <<endl;
                return 0;
            }

            unsigned int raw =adc.getRaw(channel);
            double voltage = adc.getVoltage(channel);

            cout << "Reading ADC..." << endl;
            cout << "Channel:" << channel << endl;
            cout << "RAW:" << raw << endl;
            cout << "Voltage:"  << voltage << "mV" <<endl;
     }
}

 return ret;
}
