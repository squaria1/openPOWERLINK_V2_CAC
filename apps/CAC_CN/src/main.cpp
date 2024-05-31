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
    tOplkError      ret = kErrorOk;
    tOplkApiSocTimeInfo pTimeInfo_p;
    int valTimerSoc = 0;
    int valPrevTimerSoc = 0;

    
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


                res = opl.demandeExtinctOPL();
                if(res == noError)
                    state = shutdown;
                else
                {
                    state = controlAndAcquisition;
                    file.writeTelem("CAC is going into state control and acquisition", infoStateToControl);
                    opl.sendTelem(infoStateToControl);
                }

                system_msleep(DELAYMSINIT);
                break;
            case controlAndAcquisition: // Acquisition et controle

                #if (TARGET_SYSTEM == _WIN32_)
                res = opl.demandeExtinctOPL();
                if (res == infoStopOrderReceived)
                    state = shutdown;

                res = checkStateOpl();
                if (res != noError)
                {
                    file.writeError("OpenPOWERLINK has failed!", res);
                    opl.sendError(res);
                    if (res == errSystemSendTerminate)
                        state = shutdown;
                }

                ret = oplk_process();

                if (ret != kErrorOk)
                    printf("ret : 0x%04X\n", ret);

                system_msleep(DELAYMSWIN);

                #else

                //Get the Soc time of the OpenPOWERLINK cycle
                ret = oplk_getSocTime(&pTimeInfo_p);
                if (ret != kErrorOk)
                    printf("ret : 0x%04X\n", ret);

                valTimerSoc = pTimeInfo_p.netTime.nsec;

                //Important : the valve dependance verification and the sensor reading 
                //is called here to trigger valves for every CAC boards (every CNs) at the same time
                if (valTimerSoc != valPrevTimerSoc) {
                    valPrevTimerSoc = valTimerSoc;
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
                    if (EG != 0)
                    {
                        res = valve.verifDependanceValves();
                        //if (res == noError)
                        //{
                        //    file.writeTelem("Verification of valve dependances has succeeded", infoVerifDependSucess);
                        //    opl.sendTelem(infoVerifDependSucess);
                        //}
                        //else
                        //{
                        //    file.writeError("Verification of valve dependances has failed!", res);
                        //    opl.sendError(res);
                        //}
                        res = readChannels();
                        //if (res == noError)
                        //{
                        //    file.writeTelem("Reading sensor channels has succeeded", infoReadChannels);
                        //    opl.sendTelem(infoReadChannels);
                        //}
                        //else
                        //{
                        //    file.writeError("Reading sensor channels has failed!", res);
                        //    opl.sendError(res);
                        //}
                    }

                    res = opl.demandeExtinctOPL();
                    if (res == infoStopOrderReceived)
                        state = shutdown;

                    res = checkStateOpl();
                    if (res != noError)
                    {
                        file.writeError("OpenPOWERLINK has failed!", res);
                        opl.sendError(res);
                        if (res == errSystemSendTerminate)
                            state = shutdown;
                    }
                }

                usleep(DELAYUSLINUX);
                #endif
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