#include "opl.h"
#include "file.h"
#include "csv.h"

#include <iostream>
using namespace std;

typedef enum
{
    init,
    control,
    shutdown,
    ending,
} stateDef;

int main() {
    stateDef state = init;
    opl opl;
    file file;    
    tOplkError  ret = kErrorOk;
    char        cKey = 0;
    BOOL        fExit = FALSE;
    int16_t     res = 0;


    while(state != ending){
        switch(state){
            case init: // Initialisation
                res = file.initFile();
                if (res == 0)
                    printf("TelemFile OK\n");
                else
                    printf("Error telemfiles\n");
                res = file.testWriteFile();
                if (res == 0)
                    printf("Writing in TelemFile OK\n");
                else
                    printf("Error writing in telemfiles\n");
                res = initCSV();
                if (res == 0)
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                else
                    file.writeError("", 0xE003);
                res = initOPL();
                if (res == 0) 
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                else 
                    file.writeError();
                state=control;
                break;
            case control: // Sequencement des etats generaux
                if (console_kbhit())
                {
                    cKey = (char)console_getch();

                    switch (cKey)
                    {
                    case 0x1B:
                        state = shutdown;
                        break;
                    case 'a':
                        affValeursOut();
                        break;
                    case 'z':
                        affValeursIn();
                        break;
                    case 'e':
                        affValeursInProcess();
                        break;
                    case 'r':
                        affValeursOutProcess();
                        break;
                    case 't':
                        changeEG();
                        break;
                    default:
                        break;
                    }
                }

                if (system_getTermSignalState() != FALSE)
                {
                    fExit = TRUE;
                    printf("Received termination signal, exiting...\n");
                    eventlog_printMessage(kEventlogLevelInfo,
                        kEventlogCategoryControl,
                        "Received termination signal, exiting...");
                }

                if (oplk_checkKernelStack() == FALSE)
                {
                    fExit = TRUE;
                    fprintf(stderr, "Kernel stack has gone! Exiting...\n");
                    eventlog_printMessage(kEventlogLevelFatal,
                        kEventlogCategoryControl,
                        "Kernel stack has gone! Exiting...");
                }
                processSync();
				system_msleep(1000);
                break;
            case shutdown: // Extinction
                file.writeTelem("Shutdown:0x % 08X", 0x1FFF);
                res = extinctCSV();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                }
                res = extinctOPL();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                }
                else {
                    file.writeError("", 0xE003);
                }
                res = file.closeFile();
                if (res == 0)
                    printf("TelemFile close OK\n");
                else
                    printf("Error telemfiles close\n");
                state = ending;
                break;
            default:
                break;
        }
    }

    return 0;
    
    
}
