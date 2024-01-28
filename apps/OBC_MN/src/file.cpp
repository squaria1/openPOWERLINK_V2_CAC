#include "file.h"

using namespace std;

//const string PATH_DIRECTORY_telemFiles = "..\\..\\..\\include\\telemFiles";

//const string PATH_DIRECTORY_LIST_TELEM_FILES = PATH_DIRECTORY_telemFiles + "\\listTelemFiles.txt";

string nameFiles;//nom du fichier a creer avec le temps actuel

static string year_month_day;

static string hour_minute_second;

file::file()
{
    //constructor
}

file::~file()
{
    //destructor
}

bool file::initFile()
{
    time_t now = time(0); // r�cup�ration du temps actuel dans la variable now
    tm* tm_NOW = localtime(&now); // transformation du temps actuel en struct tm contenant les composantes temporelles d'une date : seconde,minute,heure,jour,mois,ann�e 
    
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
  
    nameFiles = year_month_day  + "__" + hour_minute_second + ".txt";
    
    
    cout << nameFiles << endl;

    string PATH_DIRECTORY_FILE = file::PATH_DIRECTORY_telemFiles + "\\" + nameFiles;

    //ofstream dataFile(PATH_DIRECTORY_FILE);
    
    file::pathFile = PATH_DIRECTORY_FILE;

    //dataFile.close(); 
    

/******************* stockage du chemin du nouveau fichier de t�l�m�trie ******************/
    
    //dataFile.open(file::PATH_DIRECTORY_LIST_TELEM_FILES, ios::out | ios::app);// ouverture du fichier en mode �criture avec curseur repositionn� automatiquement � la fin du fichier
    //dataFile << PATH_DIRECTORY_FILE << endl;
    //dataFile.close();
    

/******************************************************************************************/
  return true;
}

bool file::testWriteFile()
{
    return true;
}

void file::writeTelem(const char* fmt_p, ...)///< ajouter uniquement : uint16_t codeSuccess 
{
  
    char logMsg[EVENTLOG_MAX_LENGTH];
    
    va_list arglist;
    va_start(arglist, fmt_p);
    eventlog_createMessageString(logMsg,
        EVENTLOG_MAX_LENGTH,
        kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "code_success:0x%02X",
        arglist);
    va_end(arglist);

    file::dataFile << string(logMsg) << endl;
    
}

void file::writeError(const char* fmt_p, ...) ///< ajouter uniquement : uint16_t codeError
{
    char logMsg[EVENTLOG_MAX_LENGTH];

    va_list arglist;
    va_start(arglist, fmt_p); 
    eventlog_createMessageString(logMsg,
        EVENTLOG_MAX_LENGTH,
        kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "code_Error:0x%02X",
        arglist);
    va_end(arglist);

    file::dataFile << string(logMsg) << endl;
            
}

bool file::openFile()
{
    file::dataFile.open(file::pathFile, ios::out | ios::app);// ouverture du fichier en mode �criture avec curseur repositionn� automatiquement � la fin du fichier
    return true;
}

bool file::closeFile()
{
    file::dataFile.close();
    return true;
}

