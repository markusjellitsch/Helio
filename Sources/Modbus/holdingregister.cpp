//------------------------------------------------------------------------
// NAME         : holdingregister.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Holding registers for modbusRTU
// -----------------------------------------------------------------------


#include "holdingregister.h"
#include <cassert>
#include <string>
#include <iomanip>
#include <iostream>
#include <string.h>

using namespace std;

/*---------------------------------------------------------------------------
* Constructor
*--------------------------------------------------------------------------*/
ModbusHoldingRegister::ModbusHoldingRegister(int const size){
     if (size != 0){

         mRegisters = new holding_register_t[size];
     }

     mNumRegs = size;
     assert(mRegisters != nullptr);
}


/*---------------------------------------------------------------------------
* Desctructor
*--------------------------------------------------------------------------*/
ModbusHoldingRegister::~ModbusHoldingRegister(){
    delete []mRegisters;
}

/*---------------------------------------------------------------------------
* Set Value of a holding register
*--------------------------------------------------------------------------*/
int ModbusHoldingRegister::setValue(int16_t const reg, int const value){

    if (mRegisters == nullptr) return MODBUS_ERROR_NULLPTR;
    if (reg >= mNumRegs) return MODBUS_ERROR_OUT_OF_BOUNDS;

    mRegisters[reg].value = value;

    return MODBUS_OK;
}

/*---------------------------------------------------------------------------
* Get Value of a holding register
*--------------------------------------------------------------------------*/
int ModbusHoldingRegister::getValue(int16_t const reg, int16_t * value){

    if (mRegisters == nullptr) return MODBUS_ERROR_NULLPTR;
    if (reg >= mNumRegs) return MODBUS_ERROR_OUT_OF_BOUNDS;

    *value = mRegisters[reg].value;

    return MODBUS_OK;
}

/*---------------------------------------------------------------------------
* Set Desctiption of a holding register
*--------------------------------------------------------------------------*/
int ModbusHoldingRegister::setDescription(int16_t const reg,string const & description){

    if (mRegisters == nullptr) return MODBUS_ERROR_NULLPTR;
    if (reg >= mNumRegs) return MODBUS_ERROR_OUT_OF_BOUNDS;

    mRegisters[reg].description = description;

    return MODBUS_OK;
}

/*---------------------------------------------------------------------------
*  Set Name of a register
*--------------------------------------------------------------------------*/
int ModbusHoldingRegister::setName(int16_t const reg,string const & name){

    if (mRegisters == nullptr) return MODBUS_ERROR_NULLPTR;
    if (reg >= mNumRegs) return MODBUS_ERROR_OUT_OF_BOUNDS;

    mRegisters[reg].name = name;

    return 0;
}

/*---------------------------------------------------------------------------
*  Dump register set to std::count
*--------------------------------------------------------------------------*/
void ModbusHoldingRegister::dump(){

   if (mRegisters == 0) return;

   cout << setw(100) << setfill('-')<<"-" << setfill(' ') << endl;
   cout << "|" << left << setw(4) << "NR: " << "|" << setw(10) << "NAME" << "|" << setw(6) << "VALUE" << "|" << setw(16) << "BITS" << "|"  << left <<setw(58) << "DESCRIPTION" << "|"<<endl;
   cout << setw(100) << setfill('-')<<"-" << setfill(' ') << endl;

  for (int i = 0; i < mNumRegs;i++){
         cout << "|" << left << setw(4) << i << "|" << setw(10) <<  mRegisters[i].name << "|" << setw(6) << mRegisters[i].value << "|";
         for (int e = 15; e>=0;e--){
             cout << ((mRegisters[i].value & (1<<e))== (1<<e));
         }
         cout << "|" << setw(58) << mRegisters[i].description << "|";
         cout << endl;
  }
   cout << setw(100) << setfill('-')<<"-" << setfill(' ') << endl;
}
