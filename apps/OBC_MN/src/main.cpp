#include "opl.h"
#include "file.h"

int main() {
    int etat=1;
    opl opl;
    file file;
    char cKey = 0;
    int16_t EG = -1;
    int16_t EC1 = -1;

    while(etat<=3){
        switch(etat){
            case 1: // Initialisation
                if (initOPL()) {
                    file.writeTelem();
                    opl.sendTelem();
                }
                else {
                    file.writeError();
                }
                if(testOPL()){
                    file.writeTelem();
                    opl.sendTelem();
                }else{
                    file.writeError();
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

                    default:
                        break;
                }
        }
                #if (TARGET_SYSTEM == _WIN32_)
                                Sleep(5000);
                #else
                                sleep(5);
                #endif
                EC1 = opl.getEC1();
                EG = 555;
                opl.setEG(EG);
                processSync();

                printf("\n\n EG MN : %d \n\n", EG);

                printf("\n\n EC1 MN : %d \n\n", EC1);
                break;
            case 3: // Extinction
                if (ExtinctOPL()) {
                    file.writeTelem();
                }
                else {
                    file.writeError();
                }
                etat=4;
                break;
        }
    }

    return 0;
}
