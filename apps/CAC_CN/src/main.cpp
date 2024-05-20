/**
 * \file main.cpp
 * \brief Main program with state machine
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Main program of the acquisition and control boards
 *
 */

#include "opl.h"
#include "file.h"
#include "csv.h"
#include "valve.h"
#include "sensor.h"
#include "configDefine.h"
#include "statusErrorDefine.h"

/**
 * \brief main function of the program
 * 
 * \return 0 if the program exits properly
 */
int main() {
    stateDef        state = init;
    opl             opl;
    file            file;
    valve           valve;
    sensor          sensor;
    char            cKey = 0;
    statusErrDef    res = noError;

    
    while(state != ending){
        switch(state){
            case init: // Initialisation
                res = file.initFile();
                if (res == noError)
                    printf("TelemFile OK\n");
                else
                    printf("Error telemfiles\n");
                res = file.testWriteFile();
                if(res == noError)
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
                    opl.sendTelem(infoInitOPL);

                    file.writeTelem("CAC initialisation is ongoing", infoStateToInit);
                    opl.sendTelem(infoStateToInit);
                }
                else
                    file.writeError("OpenPOWERLINK initialization has failed!", res);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                res = valve.initValve();
                if(res == noError)
                {
                    file.writeTelem("Valve subsystem has successfully initialized", infoInitValve);
                    opl.sendTelem(infoInitValve);
                }else
                {
                    file.writeError("Valve subsystem initialization has failed!", res);
                    opl.sendError(res);
                }
                res = sensor.initSensor();
                if (res == noError)
                {
                    file.writeTelem("Sensor subsystem has successfully initialized", infoInitSensor);
                    opl.sendTelem(infoInitSensor);
                }
                else
                {
                    file.writeError("Sensor subsystem initialization has failed!", res);
                    opl.sendError(res);
                }
                #endif

                system_msleep(DELAYMSINIT);

                res = opl.demandeExtinctOPL();
                if(res == noError)
                    state = shutdown;
                else
                {
                    state = controlAndAcquisition;

                    file.writeTelem("CAC is going into state control and acquisition", infoStateToControl);
                    opl.sendTelem(infoStateToControl);
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
                if (res == noError)
                {
                    res = refreshCSV();
                    if (res == noError)
                    {
                        file.writeTelem("CSV has been changed", infoCSVChanged);
                        opl.sendTelem(infoCSVChanged);
                    }
                    else
                    {
                        file.writeError("CSV has failed to change with the new EG!", res);
                        opl.sendError(res);
                    }
                }
                else if (res == infoModeSetToManual)
                {
                    printf("Mode set to manual valve activation from MN.\n");
                    file.writeTelem("Mode set to manual valve activation from MN", res);
                    opl.sendTelem(res);
                }
                else if(res == infoEGsetToZero)
                {
                    
                }

                res = opl.demandeExtinctOPL();
                if(res == infoStopOrderReceived)
                    state = shutdown;
                
                res = checkStateOpl();
                if (res != noError)
                {
                    file.writeError("OpenPOWERLINK has failed!", res);
                    opl.sendError(res);
                    if (res == errSystemSendTerminate)
                        state = shutdown;
                }

                system_msleep(DELAYMSCONTROL);
                break;
            case shutdown: // Extinction
                file.writeTelem("CAC is going into shutdown state", infoStateToShutdown);
                opl.sendTelem(infoStateToShutdown);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                
                res = sensor.extinctSensor();
                if (res == noError)
                {
                    file.writeTelem("Sensor subsystem has exited correctly", infoShutdownSensor);
                    opl.sendTelem(infoShutdownSensor);
                }
                else
                {
                    file.writeError("Sensor subsystem has failed to exit!", res);
                    opl.sendError(res);
                }
                res = valve.extinctValve();
                if(res == noError)
                {
                    file.writeTelem("Valve subsystem has exited correctly", infoShutdownValve);
                    opl.sendTelem(infoShutdownValve);
                }
                else
                {
                    file.writeError("Valve subsystem has failed to exit!", res);
                    opl.sendError(res);
                }
                #endif
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
}