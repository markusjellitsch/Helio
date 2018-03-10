//------------------------------------------------------------------------
// NAME         : pca9685.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : pwm class
// -----------------------------------------------------------------------

#ifndef PCA9685_H
#define PCA9685_H

#include <interface.h>
#include <slave.h>

// Register Definitions

#define PWM_NUM_REGISTERS       255

#define MODE1 0x00              //Mode  register  1
#define MODE2 0x01              //Mode  register  2
#define SUBADR1 0x02            //I2C-bus subaddress 1
#define SUBADR2 0x03            //I2C-bus subaddress 2
#define SUBADR3 0x04            //I2C-bus subaddress 3
#define ALLCALLADR 0x05         //LED All Call I2C-bus address
#define LED0 0x6                //LED0 start register
#define LED0_ON_L 0x6           //LED0 output and brightness control byte 0
#define LED0_ON_H 0x7           //LED0 output and brightness control byte 1
#define LED0_OFF_L 0x8          //LED0 output and brightness control byte 2
#define LED0_OFF_H 0x9          //LED0 output and brightness control byte 3
#define LED_MULTIPLYER 4        // For the other 15 channels
#define ALLLED_ON_L 0xFA        //load all the LEDn_ON registers, byte 0 (turn 0-7 channels on)
#define ALLLED_ON_H 0xFB        //load all the LEDn_ON registers, byte 1 (turn 8-15 channels on)
#define ALLLED_OFF_L 0xFC       //load all the LEDn_OFF registers, byte 0 (turn 0-7 channels off)
#define ALLLED_OFF_H 0xFD       //load all the LEDn_OFF registers, byte 1 (turn 8-15 channels off)
#define PRE_SCALE 0xFE          //prescaler for output frequency
#define CLOCK_FREQ 25000000.0   //25MHz default osc clock


#define PWM_CHANNEL0        0
#define PWM_CHANNEL1        1
#define PWM_CHANNEL2        2
#define PWM_CHANNEL3        3
#define PWM_CHANNEL4        4
#define PWM_CHANNEL5        5
#define PWM_CHANNEL6        6
#define PWM_CHANNEL7        7
#define PWM_CHANNEL8        8
#define PWM_CHANNEL9        9
#define PWM_CHANNEL10       10
#define PWM_CHANNEL11       11
#define PWM_CHANNEL12       12
#define PWM_CHANNEL13       13
#define PWM_CHANNEL14       14
#define PWM_CHANNEL15       15
#define PWM_CHANNEL16       16

typedef uint8_t PwmRegisterBank_t[PWM_NUM_REGISTERS];


class PCA9685 : public Slave
{
public:

    // constructor & destructor
    PCA9685()=default;
    virtual ~PCA9685()=default;

    // set pwm freqeuncy (max is 1.5 kHz)
    void setPWMFreq(int);

    // enable pwm
    void setPWM(uint8_t const channel, int const low, int const high);
    void setPWM(uint8_t const channel, int const high);

    // read a register from register bank
    int getRegister(uint8_t const register);

    // reset the controller
    void reset();

private:
    int readRegisters();
    int writeRegister(uint8_t const reg, uint8_t const val);

    PwmRegisterBank_t mRegisters;
};

#endif // MCP4725_H
