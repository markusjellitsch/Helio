#include "console.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

/* -----------------------------------------
 Constructor
-------------------------------------------*/
Console::Console(int argc,char ** argv)
{
    mArgc = argc;
    mArgv = argv;

    // clear argument list
    for (int i = 0;i<CONSOLE_NUM_OPTIONS;i++){
        mArgumentList[i].position = i;
        mArgumentList[i].valid = 0;
        mArgumentList[i].keyword = "";
        mArgumentList[i].value = "";
    }

    // read arguments
    for (int i = 0; i<  argc;i++){
        checkArgument(i);
    }
}

/* -----------------------------------------
 Check Argument
-------------------------------------------*/
int Console::checkArgument(int index){

    Argument_t argument;
    argument.position = index;
    argument.valid = false;
    argument.value = "";
    argument.keyword = "";

    if (mArgv[index] == 0){
        mArgumentList[index] = argument;
        return -1;
    }

    // read string
    string tmp(mArgv[index]);


    if (tmp[0] != '-'){
        argument.keyword = tmp;
        mArgumentList[index] = argument;
        return -2;
    }

    // valid now
    argument.valid = true;

    int pos = tmp.find("=");
    if (pos > 1){
        argument.value = tmp.substr(pos+1,tmp.size()-pos);
        argument.keyword = tmp.substr(1,pos-1);
    }
    else argument.keyword = tmp.substr(1,tmp.size()-1);

    mArgumentList[index] = argument;

    return CONSOLE_OK;
}

/* -----------------------------------------
 Return Valid Argument
-------------------------------------------*/
int Console::getValidArgs(){

    int count = 0;
    for (int i=0;i<mArgc;i++){
        if (mArgumentList[i].valid==true)count++;
    }

    return count;
}

/* -----------------------------------------
 Return Valid Options
-------------------------------------------*/
int Console::getValidOptions(){

    int count = 0;
    for (int i=0;i<mNumOtions;i++){
        if (mOptionList[i].position!=-1)count++;
    }

    return count;
}


/* -----------------------------------------
 Find Option in the Argument list using keyword
-------------------------------------------*/
int Console::findOption(std::string const & keyword){

    for (int i = 0; i<mArgc;i++){
        if (mArgumentList[i].keyword == keyword){
            return i;
        }
    }

    return -1;
}


/* -----------------------------------------
 Add an option to OptionList
-------------------------------------------*/
int Console::addOption(std::string const & keyword, std::string const &  description){

    Option_t option;
    option.position = -1;
    option.description = description;
    option.keyword = keyword;

    mOptionList[mNumOtions++]=option;

    return CONSOLE_OK;
}

/* -----------------------------------------
 Return if a Option was entered by the user
-------------------------------------------*/
bool Console::isOptionEntered(string const & keyword){

    for (int i = 0; i<mNumOtions;i++){
        if (mOptionList[i].keyword == keyword && mOptionList[i].position > 0){
            return true;
        }
    }

    return false;
}

/* -----------------------------------------
 Return the value of an Option if the user has entered
-------------------------------------------*/
std::string Console::getOptionValue(std::string const & keyword){
      for (int i=0;i<mNumOtions;i++){
          if (mOptionList[i].keyword == keyword){
              if (mOptionList[i].position > 0 && mOptionList[i].position < mArgc){
                  return mArgumentList[mOptionList[i].position].value;
              }
          }
      }

      return "";
}

/* -----------------------------------------
 Parse argument list and check if option have
 been entered. Return number of valid options
-------------------------------------------*/
int Console::parse(){

    int count = 0;
    for (int i = 0; i <mNumOtions;i++){

            int indexOption = findOption(mOptionList[i].keyword);
            if (indexOption != -1) count++;
            mOptionList[i].position = indexOption;
    }


    return count;
}

/* -----------------------------------------
 Check if option was entered (valid) and
 call the dispatchHandler.
-------------------------------------------*/
int Console::dispatch(std::string keyword, fpDispatch_t dispatchHandler){

   // check if option was entered by user
   if (isOptionEntered(keyword)!=CONSOLE_OK){
       return CONSOLE_ERROR_INVALID_OPTION;
   }

   // check nullptr
   if (dispatchHandler == nullptr){
       return CONSOLE_ERROR_NULLPTR;
   }

   // get value
   string value = getOptionValue(keyword);

   // call dispatch handler
   return dispatchHandler(value);
}

/* -----------------------------------------
 Dump all entries in the argument list
-------------------------------------------*/
int Console::dumpArgList(){

    cout << "Num Args:" << mArgc << endl;
    cout << "Valid Args:" << getValidArgs() << endl << endl;

    cout << left;
    cout << setw(5) << "Pos:" << setw(20) << "Key:" << setw(20) << "Value:" << setw(6) << "Valid:"  << endl;

    for (int i = 0; i<mArgc;i++){
        Argument_t argument = mArgumentList[i];
        cout << setw(5) << argument.position << setw(20) << argument.keyword << setw(20)  << argument.value <<setw(6)  << argument.valid << endl;
    }

    cout << right;

    return CONSOLE_OK;
}

/* -----------------------------------------
 Dump all entries in the option list
-------------------------------------------*/
int Console::dumpOptionList(){

    cout << "Num Options:" << mNumOtions << endl;
    cout << "Valid Options:" << getValidOptions() << endl << endl;

    cout << left;
    cout << setw(20) << "Keyword:" << setw(20) << "Description:" << setw(6) << "Valid:"  << endl;

    for (int i = 0; i<mNumOtions;i++){
        Option_t options = mOptionList[i];
        cout << setw(20) << options.keyword << setw(20) << options.description << setw(6) << options.position << endl;
    }

    cout << right;

    return CONSOLE_OK;
}

/* -----------------------------------------
 Show  available Options on stdout
-------------------------------------------*/
int Console::showOptions(){

    cout << endl;
    cout << mDescription << endl;
    cout << endl;
    cout << "usage: command -option=value [-option=value]" << endl;
    cout << left;
    cout << endl;
    cout << "List of options:" << endl;

    for (int i = 0; i<mNumOtions;i++){
        Option_t options = mOptionList[i];
        cout << right << setw(10) << "-" << left << setw(12) << options.keyword << left << "  --" << setw(20) << options.description << endl;
    }

    cout << right;

    return CONSOLE_OK;
}

/* -----------------------------------------
 Set a name for the application
-------------------------------------------*/
int Console::setAppDescription(string const & description){
    mDescription  = description;
    return CONSOLE_OK;
}




