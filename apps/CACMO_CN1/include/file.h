#ifndef FILE_H
#define FILE_H


#include "eventOpl.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>


#define EVENTLOG_MAX_LENGTH 256

using namespace std;

class file
{
public:
    file();
    ~file();

    bool initFile();
    bool testWriteFile();
    bool openFile();
    bool closeFile();
    void writeTelem(const char* fmt_p, ...); ///< ajouter uniquement : uint16_t codeSuccess
    void writeError(const char* fmt_p = "code_Error:0x%08X", ...); ///< ajouter uniquement : uint16_t codeError

protected:

private:
    const string PATH_DIRECTORY_telemFiles = "..\\..\\..\\include\\telemFiles";
    string pathFile;
    ofstream dataFile;
    string nameFiles;//nom du fichier a creer avec le temps actuel
    string year_month_day;
    string hour_minute_second;
};

#endif // FILE_H