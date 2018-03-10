#include "mcp3008.h"
#include <interface.h>
#include <slave.h>
#include <vector>

using namespace  std;


 int MCP3008::readChannel(unsigned char const channel){

   if (channel >= MCP3008_NUM_CHANNELS) return SLAVE_NOK;

   SlaveData_t data;
   data.push_back(1);
   data.push_back(0b10000000 |( ((channel & 7) << 4)));
   data.push_back(0);

   int error = writeToSlave(data,3);
   if (error != I_OK) return error;

   data.clear();

   error = readFromSlave(&data,3);
   if (error != I_OK) return error;

   uint16_t tmp = data[1] & 0x03; // get high byte
   uint16_t raw  = (uint16_t) ( tmp<<8) | data[2];

   mChannels[channel] = raw;

   return SLAVE_OK;
}

 int MCP3008::readAll(){

  int error = -1;

  for (int i = 0; i< MCP3008_NUM_CHANNELS;i++){
    error =  readChannel(i);
  }


  return error;
}




 int MCP3008::getRaw(uint8_t const channel){

    if (channel >= MCP3008_NUM_CHANNELS)return -1;
    return mChannels[channel];
}

 double MCP3008::getVoltage(uint8_t const channel){

     if (channel >= MCP3008_NUM_CHANNELS)return -1;
     return toVoltage(mChannels[channel]);
}


double MCP3008::toVoltage(unsigned int const raw){
    return ((double)raw / (double)mResolution * (double)mRefVoltage);
}
unsigned int MCP3008::toRaw(double const voltage){
    return  (uint16_t) (voltage * (double)mResolution / (double)mRefVoltage);
}


void MCP3008::setResolution(unsigned const int resolution){
    mResolution = resolution;

}
void MCP3008::setRefVoltage(unsigned const int refvolt){
    mRefVoltage = refvolt;
}


unsigned int MCP3008::getResolution() const{
    return mResolution;
}
unsigned int MCP3008::getRefVoltage()const{
   return mRefVoltage;
}

