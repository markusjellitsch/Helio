// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.11.2017
// DESCRIPTION  : Simple Clear & Write functionality for SSD1306 display
// **********************************************************************

#include <QCoreApplication>
#include <iostream>
#include <string>
#include <ssd1306.h>
#include <stdlogger.h>

using namespace std;

#define ARG_COMMAND     1
#define ARG_LINE0       2
#define ARG_LINE1       3
#define ARG_LINE2       4
#define ARG_LINE3       5

SSD1306 display;


int ReadStdin(){

   // get argumentes and convert to snptrintf
   for (int i= 0; i<4; i++){

       size_t const cBufferSize = 150;
       char buf[cBufferSize];

        cout << ">";
        cin.getline(buf,150);

        display.setFocus(i);
        display.printf("%s",buf);
   }

   display.display();
   display.closeDisplay();

   return 0;
}
/*--------------------------------------------------
 * Simple clear & write program for
 * the SSSD1306 Oled Display!
 *
 * This program takes arguments from command
 * line and runs one of following commands:
 *          => clear display
 *          => write to display
 *
 * This program can take 4 additional params. Each
 * param is the text for a line (0-3)
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    StdLogger logger;
    logger.setLoggingOption(false,false,false);

   display.setLogger(&logger);

    // initialize display
    if (!display.initDisplay()){

        cout << "Could't initialize Display!"<< endl;
        return 0;
    }

   int const cMinArgs = 2;
   if (argc < cMinArgs){
       return ReadStdin();
   }


   // check command
   char command = argv[ARG_COMMAND][0];

   switch (command) {

        // clear display
        case 'c':
           display.clearDisplay();
           break;

        // write a line
        case 'w':
           if (argc < 3){

                cout << "At least line0 must be set!" << endl;
                return 0;
           }


           // write on line0
           if (argc >=3){

               display.setFocus(SSD1306_LINE0);
               display.printf(argv[ARG_LINE0]);
           }

           // write on line1
           if (argc >=4){

               display.setFocus(SSD1306_LINE1);
               display.printf(argv[ARG_LINE1]);
           }

           // write on line2
           if (argc >=5){

               display.setFocus(SSD1306_LINE2);
               display.printf(argv[ARG_LINE2]);
           }

           // write on line3
           if (argc >=6){

               display.setFocus(SSD1306_LINE3);
               display.printf(argv[ARG_LINE3]);
           }

           // show on display
           display.display();
        break;

       default:
           break;
   }


   cout << "Closing Display!" << endl;
   cout << "Bye Bye!" << endl;

   // finally close display
   display.closeDisplay();

 return 0;
}
