#include "opl.h"
#include "file.h"
#include "csv.h"
#include "valve.h"
#include "sensor.h"
#include "configDefine.h"

int main() {
    int etat=1;
    opl opl;
    file file;
    valve valve;
    sensor sensor;

    char        cKey = 0;
    BOOL        fExit = FALSE;
    EG = 1;
    int16_t EC1 = -1;
    
    while(etat<3){
        switch(etat){
            case 1: // Initialisation      
                if (file.initFile())
                    printf("TelemFile OK\n");
                else
                    printf("Error telemfiles\n");
                if(file.testWriteFile())
                    printf("Writing in TelemFile OK\n");
                else
                    printf("Error writing in telemfiles\n");
                if (initCSV())
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                else 
                    file.writeError("", 0xE003);
                if (initOPL()) 
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }
                else
                    file.writeError("", 0xE003);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                if(valve.initValve())
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0xE002);
                }
                #endif
                if (sensor.initSensor())
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }
                else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0xE002);
                }
                if(opl.demandeExtinctOPL())
                    etat=3;
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
                if (console_kbhit())
                {
                    cKey = (char)console_getch();
                    switch (cKey)
                    {
                    case 0x1B:
                        etat = 255;
                        break;
                    case 'a':
                        EC1 = 111;
                        setEC1(EC1);
                        printf("\n\n EC1 CN : %d \n\n", EC1);
                        break;
                    case 'z':
                        EG = getEG();
                        printf("\n\n EG CN : %d \n\n", EG);
                        break;
                    default:
                        break;
                    }
                }
                if (isEGchanged())
                {
                    if (refreshCSV())
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
                if(opl.demandeExtinctOPL()){
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
#if (defined(CONFIG_USE_SYNCTHREAD) || \
                        defined(CONFIG_KERNELSTACK_DIRECTLINK))
                system_msleep(100);
#else
                processSync();
#endif

                break;
            case 3:
                break;
            case 255: // Extinction
                file.writeTelem("Shutdown:0x % 08X", 0x1FFF);
                opl.sendTelem(0x1FFF);
                #if (TARGET_SYSTEM == _WIN32_)
                #else
                if(valve.extinctValve())
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
                if (extinctCSV())
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                    opl.sendError(0x0003);
                }                
                if (extinctOPL())
                {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                }
                else
                {
                    file.writeError("", 0xE003);
                }
                break;
        }
    }
}