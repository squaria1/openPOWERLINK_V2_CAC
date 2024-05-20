/**
 * \file file.cpp
 * \brief Module to write telemetry into a file
 * \author Mael Parot, Julien Mindze
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all functions related to writing the telemetry
 * into a file named after the current date and time.
 */

#include "file.h"

file::file()
    : pathFile(""),
      dataFile(),
      nameFiles(""),
      year_month_day(""),
      hour_minute_second("") 
{
    //constructor
}

file::~file()
{
    //destructor
}

/**
 * \brief function to create the telemetry file
 * and name it after the current date and time
 * 
 * \return statusErrDef that values errOpenTelemFile
 * when the file creation and opening fails.
 * or noError when the function exits successfully.
 */
statusErrDef file::initFile()
{
    statusErrDef res = noError;
    // we get the current system time
    time_t now = time(0); 
    // transform the system time into year, month, day, hour, minute and second
    tm* tm_NOW = localtime(&now); 

    int  year = 1900 + tm_NOW->tm_year;
    int  month = 1 + tm_NOW->tm_mon;
    int  day = tm_NOW->tm_mday;

    int  hour = tm_NOW->tm_hour;
    int  minute = tm_NOW->tm_min;
    int  second = tm_NOW->tm_sec;

    // initialisation of the year, month and day
    year_month_day = year_month_day + to_string(year);
    year_month_day = year_month_day + "-";
    year_month_day = year_month_day + to_string(month);
    year_month_day = year_month_day + "-";
    year_month_day = year_month_day + to_string(day);

    // initialisation of the hour, minute and second
    hour_minute_second = hour_minute_second + to_string(hour);
    hour_minute_second = hour_minute_second + "-";
    hour_minute_second = hour_minute_second + to_string(minute);
    hour_minute_second = hour_minute_second + "-";
    hour_minute_second = hour_minute_second + to_string(second);

    //we store the telemetry file path
    nameFiles = year_month_day + "__" + hour_minute_second + ".txt";
    cout << nameFiles << endl;
    std::string path = TELEMFILES_DIRECTORY;
    path.append(nameFiles);

    file::pathFile = path;

    res = file::openFile();

    return res;
}

/**
 * \brief function to test if we can write in the telemetry file
 * 
 * \return statusErrDef that values errTestWriteFile
 * when the file write fails.
 * or noError when the function exits successfully.
 */
statusErrDef file::testWriteFile()
{
    statusErrDef res = noError;
    try
    {
        file::dataFile << "File write successful" << endl;
        return res;
    }
    catch (const std::exception& e)
    {
        return errTestWriteFile;
    }
}

/**
 * \brief function to write status telemetry into the
 * telemetry file.
 * 
 * \param fmt_p formated string about the status
 * \param ... other parameters such as the success code
 */
void file::writeTelem(const char* fmt_p, ...)
{
    char logMsg[EVENTLOG_MAX_LENGTH];
    char msg[EVENTLOG_MAX_LENGTH];

    //we copy the message from the "..." parameter and transform it to show the status code at the end
    strncpy(msg, fmt_p, EVENTLOG_MAX_LENGTH - 1);
    msg[EVENTLOG_MAX_LENGTH - 1] = '\0';
    strncat(msg, " | code_success:0x%04X", EVENTLOG_MAX_LENGTH - strlen(msg) - 1);

    //we take advantage of the OpenPOWERLINK event module to have a good log syntax
    va_list arglist;
    va_start(arglist, fmt_p);
    eventlog_createMessageString(logMsg,
        EVENTLOG_MAX_LENGTH,
        kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        msg,
        arglist);
    va_end(arglist);

    try
    {
        file::dataFile << string(logMsg) << endl;
    }
    catch (const std::exception& e)
    {
        perror("telemFiles writeTelem failed");
    }
}

/**
 * \brief function to write error telemetry into the
 * telemetry file.
 * 
 * \param fmt_p formated string about the error
 * \param ... other parameters such as the error code
 */
void file::writeError(const char* fmt_p, ...)
{
    char logMsg[EVENTLOG_MAX_LENGTH];
    char msg[EVENTLOG_MAX_LENGTH];

    //we copy the message from the "..." parameter and transform it to show the error code at the end
    strncpy(msg, fmt_p, EVENTLOG_MAX_LENGTH - 1);
    msg[EVENTLOG_MAX_LENGTH - 1] = '\0';
    strncat(msg, " | code_error:0x%04X", EVENTLOG_MAX_LENGTH - strlen(msg) - 1);

    //we take advantage of the OpenPOWERLINK event module to have a good log syntax
    va_list arglist;
    va_start(arglist, fmt_p);
    eventlog_createMessageString(logMsg,
        EVENTLOG_MAX_LENGTH,
        kEventlogLevelError,
        kEventlogCategoryGeneric,
        msg,
        arglist);
    va_end(arglist);

    try
    {
        file::dataFile << string(logMsg) << endl;
    }
    catch (const std::exception& e)
    {
        perror("telemFiles writeError failed");
    }
}

/**
 * \brief function to open the telemetry file
 * 
 * \return statusErrDef that values errOpenTelemFile
 * when the file open fails.
 * or noError when the function exits successfully.
 */
statusErrDef file::openFile()
{
    statusErrDef res = noError;
    // file open with write permission and position the file cursor at the end of the file
    try
    {
        file::dataFile.open(file::pathFile, ios::out | ios::app);
    }
    catch (const std::exception& e)
    {
        perror("telemFiles open failed");
        return errOpenTelemFile;
    }
    return res;
}

/**
 * \brief function to close the telemetry file
 * 
 * \return statusErrDef that values errCloseTelemFile
 * when the file close fails.
 * or noError when the function exits successfully.
 */
statusErrDef file::closeFile()
{
    statusErrDef res = noError;
    try
    {
        file::dataFile.close();
    }
    catch (const std::exception& e)
    {
        perror("telemFiles close failed");
        return errCloseTelemFile;
    }
    return res;
}

