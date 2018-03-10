#include <QCoreApplication>
#include <stdint.h>
#include <i2crpi.h>
#include <QDebug>
#include <string>
#include <iostream>
#include <iomanip>
#include <mcp4725.h>
#include <unistd.h>
#include <stdlogger.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include <ssd1306.h>


// Instantiate the display
#define MCP425_SLAVE_0 0x62
#define MCP425_SLAVE_1 0x63

using namespace std;

int main(int argc, char *argv[])

{
    StdLogger logger;
    logger.setLoggingOption(false,false,true);

    QCoreApplication a(argc, argv);

    uint8_t device_address = MCP425_SLAVE_0;
    char command =0;

    if (argc >2){
        device_address = atoi(argv[2]);
        command = argv[1][0];
    }
    else {
       cout << "To less arguments!" << endl;
       return  0;
    }

    I2CRPi *i2c;
    i2c =  new I2CRPi;

    if (device_address>0)   device_address = MCP425_SLAVE_1;
    else  device_address = MCP425_SLAVE_0;

    MCP4725 dac;
    dac.setSlaveAddress(device_address);
    dac.setInterface(i2c,(char*)I2C_RPI_DEVICE_FILE);
    dac.setLogger(&logger);

    unsigned int current_val = 0;
    int error = 0;
    int inc_val = 0;
    int set_val = 0;
    int address = dac.getSlaveAddress();


    switch (command) {

        case 'r':
            cout << "Reading ADC..." << endl;
            cout << "Address: 0x0" <<hex << address << dec << endl;
            current_val = dac.readDAC();
            cout << "Raw:" << current_val << endl;
            cout << "Voltage:" << dac.toVoltage(current_val) << " mV"<< endl;
            break;

        case 'w':
            if (argc < 4){
                cout << "A Value must be set as second argument!" << endl;
                return 0;
            }

            set_val = atoi(argv[3]);
            cout << "Writing DAC..." <<  endl;
            cout << "Address: 0x"  << hex << address <<endl;
            error = dac.writeVoltage(set_val);
            cout << "Status:" << error << endl;
            current_val = dac.readDAC();
            cout << "New Volt.:" << dac.toVoltage(current_val) << " mV" <<endl;
            break;

        case 'i':
            if (argc < 4){
                cout << "A Value must be set as second argument!" << endl;
                return 0;
            }

            inc_val = atoi(argv[3]);
            if (inc_val >0){
                cout << "Increase DAC..." <<endl;
            }
            else {
                cout << "Decrease DAC..." <<endl;
            }

            cout << "Address: 0x" <<hex << address <<endl;
            error = dac.increaseVoltage(inc_val);
            cout << "Status:" << error << endl;
            current_val = dac.readDAC();
            cout << "New Volt:" << dac.toVoltage(current_val) << " mV"<< endl;
            break;

    default:
        break;
    }


    sleep(1);

    delete i2c;

    return 0;

    return a.exec();
}
