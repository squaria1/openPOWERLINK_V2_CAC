#include "opl.h"
#include "file.h"
#include "csv.h"
#include "valve.h"
#include "sensor.h"
#include "configDefine.h"

typedef enum
{
    init,
    controlAndAcquisition,
    shutdown,
    ending,
} stateDef;

int main() {
    stateDef    state = init;
    opl         opl;
    file        file;
    valve       valve;
    sensor      sensor;
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
                if(res == 0)
                {
                    printf("Writing in TelemFile OK\n");
                    file.writeTelem("CAC initialisation is ongoing", 0x7001);
                    opl.sendTelem(0x7001);
                }
                else
                    printf("Error writing in telemfiles\n");
                res = initCSV();
                if (res == 0)
                    file.writeTelem("CSV subsystem has successfully initialized", 0x0101);
                else 
                    file.writeError("CSV subsystem initialization has failed!", res);
                res = initOPL();
                if (res == 0)
                {
                    file.writeTelem("OpenPOWERLINK has successfully initialized", 0x0201);
                    opl.sendTelem(0x0201);
                }
                else
                    file.writeError("OpenPOWERLINK initialization has failed!", res);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                res = valve.initValve();
                if(res == 0)
                {
                    file.writeTelem("Valve subsystem has successfully initialized", 0x0301);
                    opl.sendTelem(0x0301);
                }else
                {
                    file.writeError("Valve subsystem initialization has failed!", res);
                    opl.sendError(res);
                }
                res = sensor.initSensor();
                if (res == 0)
                {
                    file.writeTelem("Sensor subsystem has successfully initialized", 0x0401);
                    opl.sendTelem(0x0401);
                }
                else
                {
                    file.writeError("Sensor subsystem initialization has failed!", res);
                    opl.sendError(res);
                }
                #endif

                system_msleep(DELAYMSINIT);

                res = opl.demandeExtinctOPL();
                if(res == 0)
                    state = shutdown;
                else
                {
                    state = controlAndAcquisition;

                    file.writeTelem("CAC is going into state control and acquisition", 0x7002);
                    opl.sendTelem(0x7002);
                }
                break;
            case controlAndAcquisition: // Acquisition et controle
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
                        printf("\n\n EG CN : %d \n\n", EG);
                        affValeursIn();
                        break;
                    case 'e':
                        affValeursProcessIn();
                        break;
                    default:
                        break;
                    }
                }

                res = isEGchanged();
                if (res == 0)
                {
                    res = refreshCSV();
                    if (res == 0)
                    {
                        file.writeTelem("CSV has been changed", 0x0102);
                        opl.sendTelem(0x0102);
                    }
                    else
                    {
                        file.writeError("CSV has failed to change with the new EG!", res);
                        opl.sendError(res);
                    }
                }

                #if (TARGET_SYSTEM == _WIN32_)
                #else
                    res = valve.verifDependanceValves();
                    if (res == 0)
                    {
                        file.writeTelem("Verification of valve dependance has succeeded", 0x0302);
                        opl.sendTelem(0x0302);
                    }
                    else
                    {
                        file.writeError("Verification of valve dependance has failed!", res);
                        opl.sendError(res);
                    }
                    res = readChannels();
                    if (res == 0)
                    {
                        file.writeTelem("Reading sensor channels has succeeded", 0x0402);
                        opl.sendTelem(0x0402);
                    }
                    else
                    {
                        file.writeError("Reading sensor channels has failed!", res);
                        opl.sendError(res);
                    }
                #endif
                system_msleep(DELAYMSCONTROL);

                res = opl.demandeExtinctOPL();
                if(res == 0)
                    state = shutdown;

                if (system_getTermSignalState() != FALSE)
                {
                    fExit = TRUE;
                    printf("Received termination signal, exiting...\n");
                    eventlog_printMessage(kEventlogLevelInfo,
                        kEventlogCategoryControl,
                        "Received termination signal, exiting...");
                    file.writeError("OpenPOWERLINK has failed!", 0xE001);
                    opl.sendError(0xE001);
                }
                if (oplk_checkKernelStack() == FALSE)
                {
                    fExit = TRUE;
                    fprintf(stderr, "Kernel stack has gone! Exiting...\n");
                    eventlog_printMessage(kEventlogLevelFatal,
                        kEventlogCategoryControl,
                        "Kernel stack has gone! Exiting...");
                    file.writeError("OpenPOWERLINK has failed!", 0xE002);
                    opl.sendError(0xE002);
                }
                break;
            case shutdown: // Extinction
                file.writeTelem("CAC is going into shutdown state", 0x7FFF);
                opl.sendTelem(0x7FFF);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                res = sensor.extinctSensor();
                if (res == 0)
                {
                    file.writeTelem("Sensor subsystem has exited correctly", 0x04FF);
                    opl.sendTelem(0x04FF);
                }
                else
                {
                    file.writeError("Sensor subsystem has failed to exit!", res);
                    opl.sendError(res);
                }
                res = valve.extinctValve();
                if(res == 0)
                {
                    file.writeTelem("Valve subsystem has exited correctly", 0x03FF);
                    opl.sendTelem(0x03FF);
                }
                else
                {
                    file.writeError("Valve subsystem has failed to exit!", res);
                    opl.sendError(res);
                }
                #endif
                res = extinctOPL();
                if (res == 0)
                    file.writeTelem("OpenPOWERLINK has exited correctly", 0x02FF);
                else
                    file.writeError("OpenPOWERLINK has failed to exit!", res);
                res = extinctCSV();
                if (res == 0)
                {
                    file.writeTelem("CSV subsystem has exited correctly", 0x01FF);
                    opl.sendTelem(0x01FF);
                }
                else
                {
                    file.writeError("CSV subsystem has failed to exit!", res);
                    opl.sendError(res);
                }
                res = file.closeFile();
                if (res == 0)
                    printf("TelemFile close OK\n");
                else
                    printf("Error telemfiles close!\n");
                state = ending;
                break;
            default:
                break;
        }
    }
}