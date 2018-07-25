// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.05.2018
// DESCRIPTION  : testing helio test interface (=Heti), a rs485 based interface
// **********************************************************************

#include <iostream>
#include <string>
#include <Heti/heti.h>

using namespace std;

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

    // get the instance
    HETI * heti = HETI::GetInstance();
    if (heti == nullptr){
        cout << "HETI couldn't be initialized!" << endl;
        return 0;
    }

    // Open Heti (= helio test interface)
    if (heti->open() != HETI_OK){
        cout << "Error opeing heti" << endl;
        return 0;
    }

    int success = 0;

    while (1){

        // Write to a single register
        if (argc ==2){
           // writing to FU
            int success = heti->writeSingleRegister(2,0);
            success = heti->writeSingleRegister(1,(uint16_t)atoi(argv[1]));
            if (success == HETI_OK){
                cout << "Write OK!" << endl;
            }
            else cout << "Write NOK!" << endl;

            return 0;
        }

        // start timer & update registers (measure elapsed time)
        heti->startTimer();
        success = heti->updateRegisters();
        cout << heti->getElapsed() << "ms" << endl;

        if (success==0){
            cout << "Reading Registers OK!" << endl;
        }
        else cout << "Reading Registers NOK!" << endl;

        // print communication statistic
        heti->printStatistic();

        // set terminal cursor back
        heti->setTerminalCursorBack(8);
  }

    return 0;
}
