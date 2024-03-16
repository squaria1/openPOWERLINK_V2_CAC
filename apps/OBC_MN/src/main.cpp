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
    statusErrDef     res = noError;


    while(state != ending){
        switch(state){
            case init: // Initialisation
                res = file.initFile();
                if (res == noError)
                    printf("TelemFile OK\n");
                else
                    printf("Error telemfiles\n");
                res = file.testWriteFile();
                if (res == noError)
                    printf("Writing in TelemFile OK\n");
                else
                    printf("Error writing in telemfiles\n");
                res = initCSV();
                if (res == noError)
                    file.writeTelem("CSV subsystem has successfully initialized", infoInitCSV);
                else
                    file.writeError("CSV subsystem initialization has failed!", res);
                res = initOPL();
                if (res == noError)
                {
                    file.writeTelem("OpenPOWERLINK has successfully initialized", infoInitOPL);
                    file.writeTelem("CAC initialisation is ongoing", infoStateToInit);
                }
                else
                    file.writeError("OpenPOWERLINK initialization has failed!", res);
                system_msleep(DELAYMSINIT);
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

                processSync();

                res = checkStateOpl();
                if (res != noError)
                {
                    file.writeError("OpenPOWERLINK has failed!", res);
                    if (res == errSystemSendTerminate)
                        state = shutdown;
                }

				system_msleep(DELAYMSCONTROL);
                break;
            case shutdown: // Extinction
                file.writeTelem("CAC is going into shutdown state", infoStateToShutdown);
                res = extinctOPL();
                if (res == noError)
                    file.writeTelem("OpenPOWERLINK has exited correctly", infoShutdownOPL);
                else
                    file.writeError("OpenPOWERLINK has failed to exit!", res);
                res = extinctCSV();
                if (res == noError)
                    file.writeTelem("CSV subsystem has exited correctly", infoShutdownCSV);
                else
                    file.writeError("CSV subsystem has failed to exit!", res);
                res = file.closeFile();
                if (res == noError)
                    printf("TelemFile close OK\n");
                else
                    printf("Error telemfiles close!\n");
                state = ending;
                break;
            default:
                break;
        }
    }

    return 0;
    
    
}
