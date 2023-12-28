#include "opl.h"
#include "file.h"

int main() {
    int etat=1;
    opl opl;
    file file;
    
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

                #if (TARGET_SYSTEM == _WIN32_)
                                Sleep(500);
                #else
                                sleep(0.5);
                #endif
                break;
            case 3: // Extinction
                if (ExtinctOPL()) {
                    file.writeTelem();
                    opl.sendTelem();
                }
                else {
                    file.writeError();
                    opl.sendError();
                }
                etat=4;
                break;
        }
    }

    return 0;
}
