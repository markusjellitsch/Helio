#include "rs485.h"
#include <QSerialPort>
#include <QDebug>
#include <QtCore>


Rs485::Rs485(QObject* parent) : QSerialPort(parent){
    mConnected = false;
}


//-----------------------------------------
// RS485 OPEN - Open a com port for rs485
//-----------------------------------------
 bool Rs485::Connect(QString const & port,size_t const baudrate, QSerialPort::Parity const parity, QSerialPort::StopBits const stopbit){

    mConnected = false;

    setBaudRate(baudrate);
    setPortName(port);
    setStopBits(stopbit);
    setParity(parity);
    setDataBits(QSerialPort::Data8);

    mConnected = open(QIODevice::ReadWrite);

    return mConnected;
 }


 //-----------------------------------------
 // RS485 CLOSE - Close the port
 //-----------------------------------------
  void Rs485::Disconnect(){

     close();

     mConnected = false;
  }

//-----------------------------------------
// RS485 Receive Handler - Callback function
//-----------------------------------------
void Rs485::SlotReceive(){
    qDebug()<< "Received!";
}

 Rs485::~Rs485(){}

//-----------------------------------------
// RS485 SEND BYTE - Send a single byte via
// the rs485 interface. This function
// used QSerial class.
//-------------------------------------
bool  Rs485::SendByte(unsigned char const byte){
    size_t const cNumBytes = 1;
    size_t const cWaitDelay = 300;

    char val = (char)byte;
   char data[cNumBytes] = {val};


    write(QByteArray::fromRawData(data,cNumBytes));


    return  waitForBytesWritten(cWaitDelay);
}

//-----------------------------------------
// SEND MULTI BYTES - Send multiple bytes via
// the rs485 interface. This function
// used QSerial class.
//-----------------------------------------
bool  Rs485::SendMultiBytes(unsigned char * data, size_t const len){
   size_t const cWaitDelay = 300;

   if (data == nullptr || len == 0) return false;


   qint64 iBytes = write(QByteArray::fromRawData((char*)data,len));
   if (iBytes != len) return false;

    return waitForBytesWritten(cWaitDelay);
}

//-----------------------------------------
// RS485 READ BYTE - Read byte from the
// interface.
//-----------------------------------------
unsigned char  Rs485::ReadByte(){

        char byte = 0;

        bool is =  getChar(&byte);
        if (is) return byte;
        else return 0;
}

//-----------------------------------------
// RS485 IS CONNECTED - Return if successfully
// connectet to a port.
//-----------------------------------------
 bool  Rs485::IsConnected() const{

     return mConnected ;
 }
