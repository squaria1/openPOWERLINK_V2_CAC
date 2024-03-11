#include "opl.h"
#include "file.h"
#include "csv.h"
#include "valve.h"
#include "sensor.h"
#include "configDefine.h"

int main() {
    int         etat=1;
    opl         opl;
    file        file;
    valve       valve;
    sensor      sensor;

    char        cKey = 0;
    BOOL        fExit = FALSE;
    int16_t     res = 0;
    int16_t     EC1 = -1;
    EG = 1;
    EC = 1111;

    
    while(etat<256){
        switch(etat){
            case 1: // Initialisation
                res = file.initFile();
                if (res == 0)
                    printf("TelemFile OK\n");
                else
                    printf("Error telemfiles\n");
                res = file.testWriteFile();
                if(res == 0)
                    printf("Writing in TelemFile OK\n");
                else
                    printf("Error writing in telemfiles\n");
                res = initCSV();
                if (res == 0)
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                else 
                    file.writeError("", 0xE003);
                valve.test();
                res = initOPL();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }
                else
                    file.writeError("", 0xE003);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                res = valve.initValve();
                if(res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0xE002);
                }
                res = sensor.initSensor();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }
                else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0xE002);
                }
                #endif
                res = opl.demandeExtinctOPL();
                if(res == 0)
                    etat=255;
                else
                    etat=2;
                break;
            case 2: // Acquisition et controle
                //if(sensor.isValeurCaptTropEleve()){
                //    //opl.demandeExtinctOPL();
                //}else{
                //    file.writeTelem();
                //    opl.sendTelem();
                //}
                //if(sensor.isPressionTropEleve()){
                //    //opl.demandeExtinctOPL();
                //}else{
                //    file.writeTelem();
                //    opl.sendTelem();
                //}
                //if(sensor.isDebitTropEleve()){
                //    //opl.demandeExtinctOPL();
                //}else{
                //    file.writeTelem();
                //    opl.sendTelem();
                //}

                #if (TARGET_SYSTEM == _WIN32_)
                #else
                res = valve.verifDependanceValves();
                #endif
                if (console_kbhit())
                {
                    cKey = (char)console_getch();
                    switch (cKey)
                    {
                    case 0x1B:
                        etat = 255;
                        break;
                    case 'a':
                        printf("\n\n EC1 CN : %d \n\n", EC1); 
                        affValeursOut();
                        break;
                    case 'z':
                        if (getEG() != 0)
                            EG = getEG();
                        else
                            printf("values_Out_CN_l[0] is 0\n");
                        printf("\n\n EG CN : %d \n\n", EG);
                        affValeursIn();
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
                        file.writeTelem("code_success:0x % 08X", 0x0003);
                        opl.sendTelem(0x0003);
                    }
                    else
                    {
                        file.writeError("", 0xE003);
                        opl.sendError(0x0003);
                    }
                }
                res = opl.demandeExtinctOPL();
                if(res == 0){
                    etat=255;
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

                EC1 = 111;
                setEC1(EC1);

                readChannels();
                processSync();
                system_msleep(100);
                break;
            case 3:
                break;
            case 255: // Extinction
                file.writeTelem("Shutdown:0x % 08X", 0x1FFF);
                opl.sendTelem(0x1FFF);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                res = valve.extinctValve();
                if(res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0x0003);
                }
                res = sensor.extinctSensor();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0x0003);
                }
                #endif
                res = extinctCSV();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0x0003);
                }
                res = extinctOPL();
                if (res == 0)
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                }
                res = file.closeFile();
                if (res == 0)
                    printf("TelemFile close OK\n");
                else
                    printf("Error telemfiles close\n");
                etat = 256;
                break;
            default:
                break;
        }
    }
}