#include <QCoreApplication>
#include "console.h"
#include <string>
#include <iostream>
using namespace std;

int dispatchDump(string value);
int dispatchVerbose(string value);



int main(int argc, char *argv[])
{

    Console console(argc,argv);
    console.setAppDescription("Testing Console Class!");
    console.addOption("dump","Dump all registers");
    console.addOption("verbose","Enable Verbose Mode");

    // parse argument list
    int success = console.parse();
    if (success < 0){
        console.showOptions();
        return 0;
    }

    CONSOLE_RETURN_ON_SUCCESS(console.dispatch("dump",dispatchDump));
    CONSOLE_RETURN_ON_SUCCESS(console.dispatch("verbose",dispatchVerbose));

    return 1;
}


int dispatchDump(string value){

    cout << "Dispatch Dump Handler called. Value:" << value << endl;

    return CONSOLE_OK;
}

int dispatchVerbose(string value){

    cout << "Dispatch Verbose Handler called. Value:" << value << endl;

    return CONSOLE_OK;
}
