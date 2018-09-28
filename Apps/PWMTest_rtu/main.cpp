//------------------------------------------------------------------------
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : main module
// -----------------------------------------------------------------------

#include <QCoreApplication>
#include <stdint.h>
#include <i2crpi.h>
#include <QDebug>
#include <string>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdlogger.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include <pca9685.h>


using namespace std;

int main(int argc, char *argv[])

{
    StdLogger logger;
    logger.setLoggingOption(false,false,true);


    char * deviceFile = I2C_RPI_DEVICE_FILE;
    uint8_t device_address = 0x40;

    // variables
    uint8_t channel = PWM_CHANNEL0;
    uint16_t const resolution = 4096;
    uint16_t duty_cycle = resolution /2;
    uint16_t frequency = 1000;

    // read pwm channel
    if (argc >1){
        channel =   atoi(argv[1]);
    }

    // read duty_cycle
    if  (argc > 2){
        duty_cycle = atoi(argv[2]);
    }

    // read frequency
    if  (argc > 3){
        frequency = atoi(argv[3]);
    }


    // i2c inteface
    I2CRPi i2c;

    // pwm controller
    PCA9685 pwm;
    pwm.setInterface(&i2c,(unsigned char*)deviceFile);
    pwm.setLogger(&logger);
    pwm.setSlaveAddress(device_address);

    // first reset pwm
    pwm.reset();

    // set the pwm frequency
    pwm.setPWMFreq(frequency);

    // set duty cycle
    pwm.setPWM(channel,duty_cycle);

    cout << "PWM Controller..."  << endl;
    cout << "Channel:" << dec << channel << endl;
    cout << "Frequency:" << frequency << "Hz" << endl;
    cout << "Duty Cycle:" << (double)duty_cycle/ (double)resolution *100  <<"%"<< endl;

    return 0;
}
