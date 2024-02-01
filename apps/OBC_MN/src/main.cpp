#include "opl.h"
#include "file.h"

#include <iostream>
using namespace std;

int main() {
    int etat = 1;
    opl opl;
    file file;    
    tOplkError  ret = kErrorOk;
    char        cKey = 0;
    BOOL        fExit = FALSE;
    int16_t EG = -1;
    int16_t EC1 = -1;

    int16_t TEST = 30;

    while(etat<=3){
        switch(etat){
            case 1: // Initialisation
                if (initOPL()) {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem();
                }
                else {
                    file.writeError();
                }
                if(testOPL()){
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem();
                }else{
                    file.writeError("", 0xE003);
                    opl.sendError();
                }
                if(opl.demandeExtinctOPL()){
                    etat=3;
                }else{
                    etat=2;
                }
                break;
            case 2: // Sequencement des etats generaux
                if (opl.demandeExtinctOPL()) {
                    etat = 3;
                }
                if (console_kbhit())
                {
                    cKey = (char)console_getch();

                    switch (cKey)
                    {
                    case 0x1B:
                        etat = 3;
                        break;
                    case 'a':
                        EG = 555;
                        setEG(EG);
                        printf("\n\n EG MN : %d \n\n", EG);
                        break;
                    case 'z':
                        EC1 = getEC1();
                        printf("\n\n EC1 MN : %d \n\n", EC1);
                        break;
                    case 'e':
                        TEST = getTest();
                        printf("\n\n TEST : %d \n\n", TEST);
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

                #if (defined(CONFIG_USE_SYNCTHREAD) || \
                                         defined(CONFIG_KERNELSTACK_DIRECTLINK))
                                system_msleep(100);
                #else
                                processSync();
                #endif
                break;
            case 3: // Extinction
                if (ExtinctOPL()) {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                }
                else {
                    file.writeError("", 0xE003);
                }
                etat=4;
                break;
        }
    }

    return 0;
    
    
}
