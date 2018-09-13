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

static char cmd = 0;
static int reg =1;
static int16_t val = 1;
static int frequency = 5000;


// initialize stm32
STM32_VLDISCO * stm32;


/* parse arguments */
int parse_args(int argc, char *argv[]){


    if (argc >=2) cmd = argv[1][0];
    if (argc >=3) reg = atoi(argv[2]);
    if (argc >=4) val = atoi(argv[3]);
    if (argc >=5) frequency = atoi(argv[4]);

    return 0;
}

#define TESTASSERT(x)                                                                                       \
{                                                                                                           \
    if (x != 0){                                                                                            \
        std::cout << "TESTASSERT failed in file" << __FILE__ << " in line " << __LINE__ << "." << endl;     \
        stm32->dumpRegisters(true);                                                                         \
    }                                                                                                       \
                                                                                                            \
}

typedef int8_t (*testFunction_t)(void * param);

uint16_t const cNumTests = 30;
testFunction_t testTable[cNumTests] ={0};


int8_t test1_pwm_channel_infinite(void * param);
int8_t test2_pwm_channel_one_second(void * param);
int8_t test3_pwm_channel_10_seconds(void * param);
int8_t test4_pwm_wait_for_finished(void * param);

void executeTest(int const testNum, testFunction_t test, void * param){

    if (test == nullptr) return;

    int error = test(param);

    cout << "Execute Test " << testNum << "!"<<endl;
    if (error != 0){
        cout << "Test" << testNum << "failed with error code << " << error <<"!" << endl;

    }
    else cout << "Test" << testNum << "succeeded!" << endl;

    cout << endl;
}

void testMaster(uint16_t const testNum,void * param){

    if (testNum >= cNumTests){
            cout << "Invalid test number!" << endl;
            return;
    }

    if (testTable[testNum] != 0) {
        int error = testTable[testNum](param);
        if(error !=0){
           cout << "Test" << testNum << "failed with error code << " << error <<"!" << endl;
        }
        else cout << "Test" << testNum << "succeeded!" << endl;

        cout << endl;
    }

}

int main(int argc, char *argv[])
{

    if (argc < 2){
         cout << "Usage: command <param>" << endl;
         return -1;
    }

    int testNum = 0;
    testTable[testNum++] = test1_pwm_channel_infinite;
    testTable[testNum++] = test2_pwm_channel_one_second;
    testTable[testNum++] = test3_pwm_channel_10_seconds;
    testTable[testNum++] = test4_pwm_wait_for_finished;

    StdLogger logger("Cout Logger");


    // spi config
    unsigned int speed = (unsigned int)(frequency * 1000);
    SPIRPi spi;
    SPIConfig_t  config = {SPI_RPI_CS1_FILENAME,speed,8,0};
    spi.setCount(5000);


     stm32 = new STM32_VLDISCO(&spi);

    // open spi interface
    if (spi.openInterface((void *)&config) == -1){
        cout << "Couldn't open interface!" << endl;
        return 0;
    }

    stm32->setLogger(&logger);

    int success = stm32->connect();
    if (success != RTU_OK){
        cout << "Error connecting RTU" << endl;
        return 0;
    }

    cout << "Succesfully connected to RTU!" << endl;

    if (strcmp(argv[1],"pwm")==0){
        uint8_t channel = 1;
        uint16_t period = 0;
        uint32_t count = 0;

        if (argc >= 3){
            channel = atoi(argv[2]);
        }

        if (argc >= 4){
            period = atoi(argv[3]);
        }

        if (argc >= 5){
            count = atoi(argv[4]);
        }

        stm32->setPWMCount(count);
        if (period == 0) {
            stm32->stopPWM(channel);
             cout << "stopped" << endl;
        }
        else stm32->startPWM(channel,period,50);
    }

    if (strcmp(argv[1],"cnt")==0){
        uint8_t channel = 1;
        uint16_t count = 0;

        if (argc >= 3){
            channel = atoi(argv[2]);
        }

        if (argc >= 4){
            count = atoi(argv[3]);
             cout << count << endl;
        }

        logger.setLoggingOption(false,false,false);
        if (count == 0) {
            stm32->stopCapture(channel<<4);
             cout << "Capture stopped" << endl;
        }
        else {
            stm32->setCaptureCount(channel<<4,count);
            stm32->startCapture(channel<<4);
            stm32->startTimer();
            while (!stm32->isCaptureFinished(channel<<4)){
                usleep(10000);
                logger.setLoggingOption(false,false,false);
                cout << "wait for finished..." << endl;
                stm32->setTerminalCursorBack(1);
            }

            double elapsed =  stm32->getElapsed();
            cout << "capture finished!" << endl;
            cout << "elapsed: " << elapsed << " ms" << endl;
         }
    }

    if (strcmp(argv[1],"test")==0){
        uint32_t spi_clk = 100000;

        if (argc >= 3){
            spi_clk = atoi(argv[2]);
        }

        logger.setLoggingOption(false,false,false);
        config.busSpeed = spi_clk*1000;
        spi.openInterface(&config);

        while (1){
               stm32->updateRegisters();
               stm32->printStatistic();
               stm32->setTerminalCursorBack(10);
        }
      }

        if (strcmp(argv[1],"dac")==0){
            uint16_t voltage = 1000;

            if (argc >= 3){
                voltage = atoi(argv[2]);
            }

            logger.setLoggingOption(false,false,false);

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

    }

    return 0;
}


int8_t test1_pwm_channel_infinite(void * param){

    stm32->resetRegisters();
    uint16_t channel = *((uint8_t *)(param));

    stm32->setPWMCount(0);
    stm32->startPWM(channel,1000,50);
    TESTASSERT(stm32->compareSet(RTU_SYS_EN,channel));
    TESTASSERT(stm32->compareRegisterValue(RTU_PWM_CNT,1000,1));
    TESTASSERT(stm32->compareRegisterValue(RTU_PWM_DUT,50,1));

    stm32->dumpRegisters(true);
    sleep(1);
    stm32->stopPWM(channel);
    TESTASSERT(stm32->compareZero(RTU_PWM_CR,channel));
    stm32->dumpRegisters(true);

    return 0;
}

int8_t test2_pwm_channel_one_second(void * param){

    stm32->resetRegisters();

    uint16_t channel = *((uint8_t *)(param));
    stm32->setPWMCount(1000);
    stm32->startPWM(channel,1000,50);
    TESTASSERT(stm32->compareSet(RTU_SYS_EN,channel));
    TESTASSERT(stm32->compareRegisterValue(RTU_PWM_CNT,1000,1));
    TESTASSERT(stm32->compareRegisterValue(RTU_PWM_DUT,50,1));

    stm32->dumpRegisters(true);
    sleep(2);
    TESTASSERT(stm32->compareSet(RTU_PWM_SR,channel));

    return 0;
}

int8_t test3_pwm_channel_10_seconds(void * param){

    stm32->resetRegisters();
    stm32->dumpRegisters();

    uint16_t channel = *((uint8_t *)(param));
    stm32->setPWMCount(10000);
    stm32->startPWM(channel,1000,40);
    TESTASSERT(stm32->compareSet(RTU_SYS_EN,channel));
    TESTASSERT(stm32->compareRegisterValue(RTU_PWM_CNT,10000,1));
    TESTASSERT(stm32->compareRegisterValue(RTU_PWM_DUT,40,1));

    stm32->dumpRegisters(true);
    sleep(10);
    TESTASSERT(stm32->compareSet(RTU_PWM_SR,channel));

    return 0;
}


int8_t test4_pwm_wait_for_finished(void * param){

    stm32->resetRegisters();

    uint16_t channel = *((uint8_t *)(param));
    stm32->setPWMCount(30000);
    stm32->startPWM(channel,1000,40);

    cout << "Waiting for PWM to finish ..." << endl;

    while (stm32->compareSet(RTU_PWM_SR,channel) != RTU_OK){
     sleep(1);
    }

    cout << "PWM stopped!" << endl;

    TESTASSERT(stm32->compareSet(RTU_PWM_SR,channel));

    return 0;
}


