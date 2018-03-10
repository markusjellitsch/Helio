#include "mcp4725.h"
#include <interface.h>
#include <slave.h>


int MCP4725::writeVoltage(unsigned const char lowByte, unsigned const char highByte){

    SlaveData_t data;
    data.push_back(highByte);
    data.push_back(lowByte);

    return writeToSlave(data,data.size());
}

int MCP4725::writeVoltage(unsigned int voltage){

    uint16_t value = 0;
    if (voltage >= mRefVoltage) value =mResolution;
    else value = (uint16_t) (voltage * mResolution / mRefVoltage);

    unsigned char highByte = mPowerDownMode;
    unsigned char lowByte = value & 0xFF;
    unsigned char valHigh = ((uint8_t) (value >> 8)) & 0x0F;

    highByte = highByte | valHigh;

    SlaveData_t data;

    data.push_back(highByte);
    data.push_back(lowByte);

    return writeToSlave(data,data.size());
}


 int MCP4725::readDAC(){

   SlaveData_t data;

   int error = readFromSlave(&data,3);
   if (error != I_OK) return error;

   uint16_t  low =data[2] >> 4;
   uint16_t high = data[1] << 4;
   uint16_t value = high | low;

   return value;
}


 double MCP4725::readVoltage(){

    // read DAC Value from mcp4725
    int value = readDAC();
    if (value < 0) return value;

    return toVoltage(value);
}


double MCP4725::toVoltage(unsigned int const raw){
    return ((double)raw / (double)mResolution * (double)mRefVoltage);
}
unsigned int MCP4725::toRaw(double const voltage){
    return  (uint16_t) (voltage * (double)mResolution / (double)mRefVoltage);
}

void MCP4725::setMode(unsigned char const mode){
   mPowerDownMode= mode;
}

void MCP4725::setResolution(unsigned const int resolution){
    mResolution = resolution;

}
void MCP4725::setRefVoltage(unsigned const int refvolt){
    mRefVoltage = refvolt;
}

unsigned char MCP4725::getMode()const{
    return  mPowerDownMode;
}

unsigned int MCP4725::getResolution() const{
    return mResolution;
}
unsigned int MCP4725::getRefVoltage()const{
   return mRefVoltage;
}

void MCP4725::powerDown(){
    setMode(MCP4725_MODE_POWERDOWN_ON);
    writeVoltage(0); // write dummy
    setMode(MCP4725_MODE_POWERDOWN_OFF);
}

void MCP4725::wakeUp(){
    setMode(MCP4725_MODE_POWERDOWN_OFF);
    writeVoltage(mRefVoltage/2); // write dummy
}

int MCP4725::increaseVoltage(int const stepSize){
    double currentVolt = readVoltage();
    currentVolt+=stepSize;

    return writeVoltage((unsigned int)currentVolt);
}


