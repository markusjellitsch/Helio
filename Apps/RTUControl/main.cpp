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
#include <Logger/stdlogger.h>

using namespace std;

static int frequency = 5000;

// static argument variables
char ** argumentList = 0;
int argumentCnt = 0;

// initialize stm32
static STM32_VLDISCO * stm32;
static SPIRPi spi;

// command handlers
int handleHelpCommand();
int handlePWMCommand();
int handleCNTCommand();
int handleDACCommand();
int handleDumpCommand();
int handleTestCommand();

int main(int argc, char *argv[])
{

    if (argc < 2){
         cout << "Usage: command <param>" << endl;
         cout << "NOTE #1: use command help for more info!" << endl;
         return -1;
    }

    argumentCnt = argc;
    argumentList = argv;

    StdLogger logger("Cout Logger");
    logger.setLoggingOption(false,false,false);

    if (argc >=2 && strcmp(argv[argc-1],"-v")==0){
        logger.setLoggingOption(true,true,true);
    }
    // spi config
    unsigned int speed = (unsigned int)(frequency * 1000);

    SPIConfig_t  config = {SPI_RPI_CS1_FILENAME,speed,8,0};
    spi.setCount(5000);

    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

    stm32 = new STM32_VLDISCO(&spi);
    stm32->setLogger(&logger);

    int success = stm32->connect();
    if (success != RTU_OK){
        cout << "Error connecting RTU" << endl;
        return 0;
    }

    cout << "RTU connection established!" << endl;
    cout << "SPI Bus Frequency (KHz):" << speed / 1000 << endl;
    cout << endl;

    if (strcmp(argv[1],"pwm")==0){
        success = handlePWMCommand();
    }

    else if (strcmp(argv[1],"cnt")==0){
       success = handleCNTCommand();
    }

    else if (strcmp(argv[1],"test")==0){
        success = handleTestCommand();
     }


    else if (strcmp(argv[1],"dac")==0){
        success = handleDACCommand();
    }

    else if (strcmp(argv[1],"dump")==0){
        success = handleDumpCommand();
    }

    else{
        success = handleHelpCommand();
    }

    if (success == 0){
        cout << "Command succesfully exectued!" << endl;
    }
    else cout << "Command error!" << endl;

    return 0;
}


// handle command PWM
int handlePWMCommand(){

    uint8_t channel = 1;
    uint16_t period = 0;
    uint32_t count = 0;

    if (argumentCnt == 2){
        cout << "usage command pwm: <channel> <period> <count>" << endl;
        cout << "NOTE #1: When period=0, the pwm channel will be stopped!" << endl;
        cout << "NOTE #2: Possible channels: 1,2,3,4" << endl;
        return -1;
    }

    // check channel
    if (argumentCnt >= 3){
        channel = atoi(argumentList[2]);
    }

    // check period
    if (argumentCnt >= 4){
        period = atoi(argumentList[3]);
    }

    // check count
    if (argumentCnt >= 5){
        count = atoi(argumentList[4]);
    }

    // check channel
    channel = 1 << (channel -1);
    if (channel > 8) channel = 8;

    // enable pwm or disbale
    stm32->setPWMCount(count);
    if (period == 0) {
        stm32->stopPWM(channel);
        cout << "stopped" << endl;
    }

    else stm32->startPWM(channel,period,50);

    return 0;
}

// handle command CNT
int handleCNTCommand(){

    uint8_t channel = 1;
    uint16_t count = 0;

    // sanity check
    if (argumentCnt == 2){
        cout << "usage command cnt: <channel> <count>" << endl;
        cout << "NOTE #1: Possible channels: 1,2,3,4" << endl;
        return -1;
    }

    // check  channel
    if (argumentCnt >= 3){
        channel = atoi(argumentList[2]);
    }

    // check count
    if (argumentCnt >= 4){
        count = atoi(argumentList[3]);
        cout << count << endl;
    }

    channel = 1 << (channel-1);
    channel = channel << 4;

    // enable or disable freq channel
    if (count == 0) {
        stm32->stopCapture(channel);
        cout << "Capture stopped" << endl;
    }
    else {
        stm32->setCaptureCount(channel,count);
        stm32->startCapture(channel);
        stm32->startTimer();
        while (!stm32->isCaptureFinished(channel)){
            usleep(10000);
            cout << "wait for finished..." << endl;
            stm32->setTerminalCursorBack(1);
        }

        double elapsed =  stm32->getElapsed();
        cout << "capture finished!" << endl;
        cout << "elapsed: " << elapsed << " ms" << endl;
     }

    return 0;
}

// handle command DAC
int handleDACCommand(){
    uint16_t voltage = 1000;

    if (argumentCnt == 2){
      cout << "usage command dac: <volatage>"<< endl;
      return -1;
    }

    if (argumentCnt >= 3){
        voltage = atoi(argumentList[2]);
    }

    if (voltage > 3300){
        voltage = 3300;
        cout << "DAC voltage trunacated to " << voltage <<" mV" << endl;
    }

   cout << "Set DAC voltage: " << voltage << "mV"<< endl;
   int success =  stm32->setDACVoltage(voltage);
   if (success == RTU_OK){
       cout << "Setting voltage for DAC succesful!"<<endl;
   }
   else cout << "Couldn't set voltage for DAC!"<<endl;

   return 0;
}

// handle command test
int handleTestCommand(){
    uint32_t spi_clk = 100000;

    if (argumentCnt >= 3){
        spi_clk = atoi(argumentList[2]);
    }

    SPIConfig_t  config = {SPI_RPI_CS1_FILENAME,0,8,0};
    config.busSpeed = spi_clk*1000;
    spi.openInterface(&config);

    while (1){
           stm32->updateRegisters();
           stm32->printStatistic();
           stm32->setTerminalCursorBack(10);
    }

    return 0;
}

// handle command dump
int handleDumpCommand(){

    stm32->dumpRegisters(true);
    return 0;
}

// handle Command help;
int handleHelpCommand(){

    cout << "Possible Commands:" << endl;
    cout << "   * pwm <channel> <period> <count>" << endl;
    cout << "   * cnt <channel> <count>" << endl;
    cout << "   * dump" <<endl;
    cout << "   * test" << endl;
    cout << "   * help" << endl;

    return 0;
}





