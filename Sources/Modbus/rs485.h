#ifndef RS485_H
#define RS485_H


#define RS485_BUFFER_SIZE 255

#include <QString>





class Rs485 : public QSerialPort
{
    Q_OBJECT
public:

    Rs485(QObject* parent = 0);
     ~Rs485();

    bool Connect(QString const & port,size_t const baudrate, QSerialPort::Parity const parity, QSerialPort::StopBits const stopbit);
    void Disconnect();

    bool SendByte(unsigned char const byte);
    bool SendMultiBytes(unsigned char * data, size_t const len);
    unsigned char ReadByte();

    bool IsConnected() const;

public slots:

    void SlotReceive();


private:
    bool mConnected;

};

#endif // RS485_H
