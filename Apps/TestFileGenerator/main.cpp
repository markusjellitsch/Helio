

#include <QCoreApplication>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ostream>
#include <istream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <libgen.h>

#define DEFAULT_PATH ""

using namespace std;

static string outputPath = "";
static string className = "";
static string fileName = "";
static uint8_t cArgMinCount = 3;

static string  currentDate = "";

static string  const cTemplateFile = "test_template";

static string const cReplaceFileString = "PLACEHOLDER_FILE";
static string const cReplaceClassString = "PLACEHOLDER_CLASS";
static string const cReplaceDateString = "PLACEHOLDER_DATE";

void writeFile(string const & appendix);

int main(int argc, char *argv[])
{

    // check args;
    if (argc < cArgMinCount){
        cout << "usage: command <file name> <class name>" << endl;
        return 0;
    }

    // output path
    if (argc == cArgMinCount+1){
        outputPath = argv[3];
    }

    // read aruments
    string tmpFile(argv[1]);
    string tmpClass(argv[2]);
    fileName = tmpFile;
    className = tmpClass;

    // make sure to be in the right directory
    char buffer[1024];
    getcwd(buffer,1024);
    outputPath.append(buffer);
    // clear buffer
    memset(buffer,0,1024);

    // change directory
    readlink("/proc/self/exe",buffer,1024);
    string programPath = dirname(buffer);
    chdir(programPath.c_str());
    getcwd(buffer,1024);
    string currentWorkingDir(buffer);

    // print info
    cout << "Creating test class!" << endl;
    cout << "Current path:" << currentWorkingDir << endl;
    cout << "File Name:" << tmpFile << endl;
    cout << "Class Name:" << className << endl;
    cout << "Output Path:" << outputPath << endl;

    // get current data
    auto t = std::time(nullptr);
    auto time = *localtime(&t);
    stringstream stream;

    stream << put_time(&time,"%d.%m.%Y");
    currentDate = stream.str();

    // write header
    cout << "Create header (.h) file!" << endl;
    writeFile(".h");

    // write source file
    cout << "Create source (.cpp) file!" << endl;
    writeFile(".cpp");

    cout << "Finished creating class file!" << endl;


    return 0;
}



void writeFile(string const & appendix){

   // open template
   ifstream templateStream;
   templateStream.open(cTemplateFile + appendix);

   // open output file
   ofstream oStream;

   // open header file
   oStream.open(outputPath +"/"+ fileName + appendix);

   size_t const bufferSize = 255;
   char buffer[bufferSize];

   size_t count = 0;
   while (templateStream.getline(buffer,bufferSize)){

       string tmp(buffer);

       // replace class place holder
       size_t index = tmp.find(cReplaceClassString);
       if (index != string::npos){
            tmp.replace(index,strlen(cReplaceClassString.c_str()),className);
       }


       // replace file place holder
       index = tmp.find(cReplaceFileString);
       if (index != string::npos){
            tmp.replace(index,strlen(cReplaceFileString.c_str()),fileName);
       }

       // replace date place holder
       index = tmp.find(cReplaceDateString);
       if (index != string::npos){
            tmp.replace(index,strlen(cReplaceDateString.c_str()),currentDate);
       }

       // write to output file
       oStream << tmp <<endl;

       count++;
   }


    cout << count << " lines written!" << endl;

   // close template
   templateStream.close();

   // close header file
   oStream.close();
}


