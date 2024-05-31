/**
 * \file file.h
 * \brief header file of the file module that writes telemetry into a file
 * \author Mael Parot, Julien Mindze
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all functions definitions related to writing the telemetry
 * into a file named after the current date and time.
 */

#ifndef FILE_H
#define FILE_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "eventOpl.h"
#include "configDefine.h"
#include "statusErrorDefine.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

/**
 * \brief file module class.
 *
 */
class file
{
public:
    file();
    ~file();

    statusErrDef initFile();
    statusErrDef testWriteFile();
    statusErrDef openFile();
    statusErrDef closeFile();
    void writeTelem(const char* fmt_p, ...);
    void writeError(const char* fmt_p = "code_Error:0x%08X", ...);

protected:

private:
    /**
     * \brief path to the telemetry file
     */
    string pathFile;
    /**
     * \brief telemetry file object
     */
    ofstream dataFile;
    /**
     * \brief name of the telemetry file
     */
    string nameFiles;
    /**
     * \brief current year, month and day
     */
    string year_month_day;
    /**
     * \brief current hour, minute and second
     */
    string hour_minute_second;
};

#endif // FILE_H