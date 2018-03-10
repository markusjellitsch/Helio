#ifndef MCP3008_H
#define MCP3008_H

#include <interface.h>
#include <slave.h>
#include <spirpi.h>
#include <vector>


#define MCP3008_MODE_POWERDOWN_OFF   (0x00 <<4)
#define MCP3008_MODE_POWERDOWN_ON    (0x03 <<4)

#define MCP3008_NUM_CHANNELS 8

#define MCP3008_CHANNEL0  0
#define MCP3008_CHANNEL1  1
#define MCP3008_CHANNEL2  2
#define MCP3008_CHANNEL3  3
#define MCP3008_CHANNEL4  4
#define MCP3008_CHANNEL5  5
#define MCP3008_CHANNEL6  6
#define MCP3008_CHANNEL7  7


typedef  uint16_t ChannelTable_t[MCP3008_NUM_CHANNELS];



class MCP3008 : public Slave
{
public:


    MCP3008()=default;

    // reading specific ADC channel
    int readChannel(uint8_t const channel);

    // reading all channels
    int readAll();

    int getRaw(uint8_t const channel);
    double getVoltage(uint8_t const channel);

    void setResolution(unsigned const int resolution);
    void setRefVoltage(unsigned const int refvolt);

    unsigned int getResolution() const;
    unsigned int getRefVoltage()const;

    double toVoltage(unsigned int const raw);
    unsigned int toRaw(double const voltage);


private:

    ChannelTable_t mChannels;

    unsigned int mRefVoltage = 4760;
    unsigned int mResolution = 4096;
};

#endif // MCP3008_H
