#include "gpio.h"
#include "opl.h"
#include "file.h"
#include "stateMachine.h"
#include "sensor.h"

int main() {
    int etat=1;
    gpio gpio;
    opl opl;
    file file;
  
    sensor capteur;
    
    while(etat<3){
        switch(etat){
            case 1: // Initialisation
                for(int i=0; i<3;i++){
                    if(gpio.initGPIO()){
                        file.writeTelem();
                        opl.sendTelem();
                    }else{
                        file.writeError();
                        opl.sendTelem();
                    }
                    if(opl.initOPL()){
                        file.writeTelem();
                        opl.sendTelem();
                    }else{
                        file.writeError();
                        opl.sendTelem();
                    }
                    if(opl.testOPL()){
                        file.writeTelem();
                        opl.sendTelem();
                    }else{
                        file.writeError();
                        opl.sendTelem();
                    }
                    if(fichier.testWriteFile()){
                        file.writeTelem();
                        opl.sendTelem();
                    }else{
                        file.writeError();
                        opl.sendTelem();
                    }
                    if(capteur.testCapteur()){
                        file.writeTelem();
                        opl.sendTelem();
                    }else{
                        file.writeError();
                        opl.sendTelem();
                    }
                    if(opl.demandeExtinctOPL()){
                        etat=2;
                    }else{
                        etat=3;
                    }
                }
                break;
            case 2: // Acquisition et controle
                if(capteur.isValeurCaptTropEleve()){
                    opl.demandeExtinctOPL();
                }else{
                    file.writeTelem();
                    opl.sendTelem();
                }
                if(capteur.isPressionTropEleve()){
                    opl.demandeExtinctOPL();
                }else{
                    file.writeTelem();
                    opl.sendTelem();
                }
                if(capteur.isDebitTropEleve()){
                    opl.demandeExtinctOPL();
                }else{
                    file.writeTelem();
                    opl.sendTelem();
                }
                if(opl.demandeExtinctOPL()){
                    etat=3;
                }
                break;
            case 3: // Extinction
                if(opl.ExtinctOPL()){
                    file.writeTelem();
                    opl.sendTelem();
                }else{
                    file.writeError();
                    opl.sendTelem();
                }
                break;
        }
    }
}