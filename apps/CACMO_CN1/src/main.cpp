
#include "opl.h"
#include "file.h"
//#include "valve.h"
//#include "sensor.h"

int main() {
    int etat=1;
    opl opl;
    file file;
    char cKey = 0;
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

                    default:
                        break;
                    }
                }

                if(opl.demandeExtinctOPL()){
                    etat=3;
                }

                #if (TARGET_SYSTEM == _WIN32_)
                                Sleep(1000);
                #else
                                sleep(1);
                #endif

                EC1 = 111;
                opl.setEC1(EC1);

                processSync();

                EG = opl.getEG();

                printf("\n\n EG CN : %d \n\n", EG);

                printf("\n\n EC1 CN : %d \n\n", EC1);

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