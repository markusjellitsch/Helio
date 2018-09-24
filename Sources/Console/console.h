
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <stdint.h>
#include <vector>

#define CONSOLE_OK                      0
#define CONSOLE_ERROR_NO_ARGS           -1
#define CONSOLE_ERROR_INVALID_OPTION    -2
#define CONSOLE_ERROR_NULLPTR           -3

#define CONSOLE_NUM_ARGUMENTS           20
#define CONSOLE_NUM_OPTIONS             20


#define CONSOLE_RETURN_ON_SUCCESS(x)    \
{                                       \
    if (x == CONSOLE_OK) return 0;      \
}


// struct of an Argument entered by the user
struct Argument_t{
    int position;
    std::string keyword;
    std::string value;
    bool valid;
};

// struct of an Option defined by the programmer
struct Option_t{
   int position;
   std::string keyword;
   std::string description;
};

// callback function for dispatching an entered option
typedef int (*fpDispatch_t)(std::string argValue);

class Console
{
public:
    Console(int argc=1,char ** argv=nullptr);
    ~Console()=default;

    int setAppDescription(std::string const & description);
    // add an option
    int addOption(std::string const & keyword, std::string const & desctiption);

    // check options - return option value
    int isOptionEntered(std::string const & keyword);
    std::string getOptionValue(std::string const & keyword);

    // parse arguments
    int parse();

    // call dispatch handler when option was entered. The dispatch handler includes the option value.
    int dispatch(std::string keyword, fpDispatch_t dispatchHandler);

    // print Options to the screen (for user information)
    int showOptions();

    // dump arguments or option (use for debugging)
    int dumpArgList();
    int dumpOptionList();

private:

    // count valid arguments
    int getValidArgs();

    // count valid arguments
    int getValidOptions();

    int checkArgument(int index);
    int findOption(std::string const & keyword);

    int mArgc = 0;
    char ** mArgv=nullptr;

    std::string mDescription="Just another Application!";

    Argument_t mArgumentList[CONSOLE_NUM_ARGUMENTS];
    Option_t mOptionList[CONSOLE_NUM_OPTIONS];

    int mNumOtions = 0;

};

#endif // CONSOLE_H
