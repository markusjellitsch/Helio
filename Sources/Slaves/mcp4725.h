#ifndef MCP4725_H
#define MCP4725_H

#include <interface.h>
#include <slave.h>

#define MCP4725_MODE_POWERDOWN_OFF   (0x00 <<4)
#define MCP4725_MODE_POWERDOWN_ON    (0x03 <<4)
class MCP4725 : public Slave
{
public:


    MCP4725()=default;

    int writeVoltage(unsigned const char lowByte, unsigned const char highByte);
    int writeVoltage(unsigned int voltage);

    double readVoltage();
    int readDAC();

    void setResolution(unsigned const int resolution);
    void setRefVoltage(unsigned const int refvolt);

    unsigned char getMode()const;
    unsigned int getResolution() const;
    unsigned int getRefVoltage()const;

    double toVoltage(unsigned int const raw);
    unsigned int toRaw(double const voltage);

    void powerDown();
    void wakeUp();

    int increaseVoltage(int const stepSize);

private:
    void setMode(unsigned char const mode);
    unsigned int mRefVoltage = 4760;
    unsigned int mResolution = 4096;
    unsigned char mPowerDownMode=0x00;

};

#endif // MCP4725_H
