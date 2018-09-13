// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.05.2018
// DESCRIPTION  : Control for Webex
// **********************************************************************

#include <iostream>
#include <string>
#include <Slaves/webex.h>
#include <unistd.h>
#include <string.h>

using namespace std;

// static argument variables
char ** argumentList = 0;
int argumentCnt = 0;

// dispatch handlers
int handleRGBColor();
int handlePWM();
int handleCNT();
int handleRead();
int handleDIP();
int handleAnalog();
int handleHelp();

WebEx webex;

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

  argumentList = argv;
  argumentCnt = argc;

  if (argc < 2){
      handleHelp();
      return 0;
  }

   if (webex.connect()!= WEBEX_OK){
        cout << "Couldn't connect to WebEx!" << endl;
        return -1;
   }

   cout << "Connect succefully to WebEx!" << endl;


   // handle command
   if (strcmp(argv[1],"rgb")==0){
        handleRGBColor();
   }
   else if (strcmp(argv[1],"pwm")==0){
       handlePWM();
   }
   else if (strcmp(argv[1],"cnt")==0){
       handleCNT();
   }
   else if (strcmp(argv[1],"analog")==0){
       handleAnalog();
   }
   else if (strcmp(argv[1],"dip")==0){
       handleDIP();
   }
   else if (strcmp(argv[1],"dmp")==0){
       webex.dumpRegisters();
   }
   else if (strcmp(argv[1],"read")==0){
       handleRead();
   }
   else if (strcmp(argv[1],"help")==0){
       handleHelp();
   }
   else{
       cout << "invalid command!" << endl;
       handleHelp();
   }

    return 0;
}

// -------------------------
// RGB Handler
// -------------------------
int handleRGBColor(){

    if (argumentCnt <= 2) {

        cout << "No RGB value specified!" << endl;

        return -1;
    }
    uint8_t rgbColor = 0;
    rgbColor = atoi(argumentList[2]);

    int success = webex.setRGBColor(rgbColor);
    if (success != WEBEX_OK){
       cout << "Couldn't set RGB Color" << endl;
       return -2;
    }

    cout << "RGB Color set successfully" << endl;

    return 0;
}

// -------------------------
// PWM Handler
// -------------------------
int handlePWM(){

    int success = 0;

    if (argumentCnt <= 2){

        success = webex.stopPWM();
        if (success != WEBEX_OK){
           cout << "Couldn't stop PWM" << endl;
           return -2;
        }

        cout << "Pwm stopped succesfully!" << endl;
        return 0;
    }


    uint8_t duty = 0;
    duty = atoi(argumentList[2]);

    uint16_t duration = 0;
    if (argumentCnt >= 4){
        duration = atoi(argumentList[3]);
    }

    success = webex.startPWM(duty,duration);
    if (success != WEBEX_OK){
       cout << "Couldn't start PWM" << endl;
       return -2;
    }

    cout << "PWM started successfully" << endl;

    return 0;
}

// -------------------------
// CNT Handler
// -------------------------
int handleCNT(){

    int success = 0;

    if (argumentCnt <= 2){

        success = webex.stopCounter();
        if (success != WEBEX_OK){
           cout << "Couldn't stop Counter" << endl;
           return -2;
        }

        cout << "Counter stopped succesfully!" << endl;
        return 0;
    }


    uint8_t id = 0;
    id = atoi(argumentList[2]);

    uint16_t value = 1000;
    if (argumentCnt >= 4){
        value = atoi(argumentList[3]);
    }

    success = webex.startCounter(id,value);
    if (success != WEBEX_OK){
       cout << "Couldn't start Counter" << endl;
       return -2;
    }

    cout << "Counter started successfully" << endl;

    return 0;
}

// -------------------------
// READ Handler
// -------------------------
int handleRead(){

    int success = 0;

    if (argumentCnt <= 2){
        cout << "No register for read specified!" << endl;
        return -2;
    }

    int16_t reg = atoi(argumentList[2]);
    int16_t value = 0;
    success = webex.readRegister(reg,&value);
    if (success != WEBEX_OK){
       cout << "Couldn't read Register!" << endl;
       return -2;
    }

    cout << "Value of reg " << reg << " is " << value << endl;

    return 0;

}

// -------------------------
// DIP Handler
// -------------------------
int handleDIP(){

    int success = 0;

    uint16_t value = 0;
    success = webex.readDip(&value);
    if (success != WEBEX_OK){
       cout << "Couldn't read DIP-Register!" << endl;
       return -2;
    }

    cout << "Value of dip is 0x" << hex << value << endl;

    return 0;

}

// -------------------------
// Analog Handler
// -------------------------
int handleAnalog(){

    int success = 0;

    uint16_t value = 0;
    success = webex.readADC(&value);
    if (success != WEBEX_OK){
       cout << "Couldn't read Analog-Register!" << endl;
       return -2;
    }

    cout << "Value of Analog Pin is " << value << " mV"<< endl;

    return 0;

}

// -------------------------
// Hep Handler
// -------------------------
int handleHelp(){
    cout << "Command List:" << endl;
    cout << "     " << "* rgb <value>"<< endl;
    cout << "     " << "* pwm <duty> <duration(opt.)>"<< endl;
    cout << "     " << "* cnt <nr> <count_val(opt.)>"<< endl;
    cout << "     " << "* dip "<< endl;
    cout << "     " << "* analog "<< endl;
    cout << "     " << "* read <register>"<< endl;
    cout << "     " << "* dmp "<< endl;
    cout << "     " << "* help "<< endl;


    return 0;
}

