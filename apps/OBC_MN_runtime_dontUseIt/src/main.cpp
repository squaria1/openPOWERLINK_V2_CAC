/**
 * \file main.cpp
 * \brief Main program with state machine
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Main program of the MN (Master Node) 
 * that controls the CNs (the acquisition and control boards)
 *
 */

#include "opl.h"
#include "file.h"
#include "csv.h"

#include <iostream>
using namespace std;

/**
 * \brief main function of the program
 * 
 * \return int 0 if the program exits properly
 */
int main() {
    stateDef state = init;
    opl opl;
    file file;    
    char        cKey = 0;
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
                //system_msleep(DELAYMSINIT);
                state=control;
                break;
            case control: // General state sequencing through user keyboard input
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
                    case 'q':
                        manualActivation(1, 1);
                        break;
                    case 's':
                        manualActivation(1, 2);
                        break;
                    case 'd':
                        manualActivation(1, 3);
                        break;
                    case 'f':
                        manualActivation(1, 4);
                        break;
                    case 'g':
                        manualActivation(2, 1);
                        break;
                    case 'h':
                        manualActivation(2, 2);
                        break;
                    case 'j':
                        manualActivation(3, 1);
                        break;
                    case 'k':
                        manualActivation(3, 2);
                        break;
                    case 'l':
                        manualActivation(3, 3);
                        break;
                    case 'm':
                        manualActivation(3, 4);
                        break;
                    case 'i':
                        setEGToManualMode();
                        break;
                    default:
                        break;
                    }
                }
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
