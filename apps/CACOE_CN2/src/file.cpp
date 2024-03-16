#include "file.h"

file::file()
{
    //constructor
}

file::~file()
{
    //destructor
}

int16_t file::initFile()
{
    int16_t res = 0;
    // recuperation du temps actuel dans la variable now
    time_t now = time(0); 
    // transformation du temps actuel en struct tm contenant les composantes temporelles d'une date : seconde,minute,heure,jour,mois,annee 
    tm* tm_NOW = localtime(&now); 

    int  year = 1900 + tm_NOW->tm_year;
    int  month = 1 + tm_NOW->tm_mon;
    int  day = tm_NOW->tm_mday;

    int  hour = tm_NOW->tm_hour;
    int  minute = tm_NOW->tm_min;
    int  second = tm_NOW->tm_sec;

    /*********************** initialisation day_month_year *************************************/

    year_month_day = year_month_day + to_string(year);
    year_month_day = year_month_day + "-";
    year_month_day = year_month_day + to_string(month);
    year_month_day = year_month_day + "-";
    year_month_day = year_month_day + to_string(day);

    /*******************************************************************************************/

    /*********************** initialisation hour_minute_seconde *************************************/

    hour_minute_second = hour_minute_second + to_string(hour);
    hour_minute_second = hour_minute_second + "-";
    hour_minute_second = hour_minute_second + to_string(minute);
    hour_minute_second = hour_minute_second + "-";
    hour_minute_second = hour_minute_second + to_string(second);

    /*******************************************************************************************/

    /******************* stockage du chemin du nouveau fichier de t�l�m�trie ******************/

    nameFiles = year_month_day + "__" + hour_minute_second + ".txt";
    cout << nameFiles << endl;
    std::string path = TELEMFILES_DIRECTORY;
    path.append(nameFiles);

    file::pathFile = path;

    res = file::openFile();

    /******************************************************************************************/

    return res;
}

int16_t file::testWriteFile()
{
    try
    {
        file::dataFile << "File write successful" << endl;
        return 0;
    }
    catch (const std::exception& e)
    {
        return 1;
    }
}

void file::writeTelem(const char* fmt_p, ...)///< ajouter uniquement : uint16_t codeSuccess 
{

    char logMsg[EVENTLOG_MAX_LENGTH];
    char msg[EVENTLOG_MAX_LENGTH];

    strncpy(msg, fmt_p, EVENTLOG_MAX_LENGTH - 1);
    msg[EVENTLOG_MAX_LENGTH - 1] = '\0';
    strncat(msg, " | code_success:0x%04X", EVENTLOG_MAX_LENGTH - strlen(msg) - 1);

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

void file::writeError(const char* fmt_p, ...) ///< ajouter uniquement : uint16_t codeError
{
    char logMsg[EVENTLOG_MAX_LENGTH];
    char msg[EVENTLOG_MAX_LENGTH];

    strncpy(msg, fmt_p, EVENTLOG_MAX_LENGTH - 1);
    msg[EVENTLOG_MAX_LENGTH - 1] = '\0';
    strncat(msg, " | code_error:0x%04X", EVENTLOG_MAX_LENGTH - strlen(msg) - 1);

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

int16_t file::openFile()
{
    // ouverture du fichier en mode ecriture avec curseur repositionne automatiquement a la fin du fichier
    try
    {
        file::dataFile.open(file::pathFile, ios::out | ios::app);
    }
    catch (const std::exception& e)
    {
        perror("telemFiles open failed");
        return 0xE001;
    }
    return 0;
}

int16_t file::closeFile()
{
    try
    {
        file::dataFile.close();
    }
    catch (const std::exception& e)
    {
        perror("telemFiles close failed");
        return 0xE0FF;
    }
    return 0;
}

