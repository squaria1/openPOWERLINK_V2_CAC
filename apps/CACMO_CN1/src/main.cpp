
#include "opl.h"
#include "file.h"
//#include "valve.h"
//#include "sensor.h"

int main() {
    int etat=1;
    opl opl;
    file file;
    tOplkError  ret;
    char        cKey = 0;
    BOOL        fExit = FALSE;
    int16_t EG = -1;
    int16_t EC1 = -1;
  

    /*valve valve;
    sensor sensor;*/
    
    while(etat<3){
        switch(etat){
            case 1: // Initialisation
                if (initOPL()) {
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                    opl.sendTelem(0x0002);
                }
                else {
                    file.writeError("", 0xE003);
                }
                /*if(valve.initValve()){
                    file.writeTelem();
                    opl.sendTelem();
                }else{
                    file.writeError();
                    opl.sendError();
                }
                if(opl.testOPL()){
                    file.writeTelem();
                    opl.sendTelem();
                }else{
                    file.writeError();
                    opl.sendError();
                }
                if(file.testWriteFile()){
                    file.writeTelem();
                    opl.sendTelem();
                }else{
                    opl.sendError();
                }
                if(sensor.testSensor()){
                    file.writeTelem();
                    opl.sendTelem();
                }else{
                    file.writeError();
                    opl.sendError();
                }*/
                if(opl.demandeExtinctOPL()){
                    etat=3;
                }else{
                    etat=2;
                }
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
                        etat = 3;
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
                if(opl.demandeExtinctOPL()){
                    etat=3;
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
                opl.sendTelem(0x1FFF);
                if(ExtinctOPL()){
                    file.writeTelem("code_success:0x % 08X", 0x0003);
                }else{
                    file.writeError("", 0xE003);
                }
                break;
        }
    }
}