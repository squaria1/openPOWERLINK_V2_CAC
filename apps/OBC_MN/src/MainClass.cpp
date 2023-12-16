#include "gpio.h"
#include "opl.h"
#include "file.h"
#include "stateMachine.h"
#include "sensor.h"

int main() {
    int etat=1;
    gpio gpio;
    opl opl;
    file fichier;
    stateMachine machineEtat;
    sensor capteur;
    
    while(etat<3){
        switch(etat){
            case 1: // Initialisation
                for(int i=0; i<3;i++){
                    if(gpio.initGPIO()){
                        machineEtat.writeTelem();
                        machineEtat.sendTelem();
                    }else{
                        machineEtat.writeError();
                        machineEtat.sendTelem();
                    }
                    if(opl.initOPL()){
                        machineEtat.writeTelem();
                        machineEtat.sendTelem();
                    }else{
                        machineEtat.writeError();
                        machineEtat.sendTelem();
                    }
                    if(opl.testOPL()){
                        machineEtat.writeTelem();
                        machineEtat.sendTelem();
                    }else{
                        machineEtat.writeError();
                        machineEtat.sendTelem();
                    }
                    if(fichier.testWriteFile()){
                        machineEtat.writeTelem();
                        machineEtat.sendTelem();
                    }else{
                        machineEtat.writeError();
                        machineEtat.sendTelem();
                    }
                    if(capteur.testCapteur()){
                        machineEtat.writeTelem();
                        machineEtat.sendTelem();
                    }else{
                        machineEtat.writeError();
                        machineEtat.sendTelem();
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
                    machineEtat.writeTelem();
                    machineEtat.sendTelem();
                }
                if(capteur.isPressionTropEleve()){
                    opl.demandeExtinctOPL();
                }else{
                    machineEtat.writeTelem();
                    machineEtat.sendTelem();
                }
                if(capteur.isDebitTropEleve()){
                    opl.demandeExtinctOPL();
                }else{
                    machineEtat.writeTelem();
                    machineEtat.sendTelem();
                }
                if(opl.demandeExtinctOPL()){
                    etat=3;
                }
                break;
            case 3: // Extinction
                if(opl.ExtinctOPL()){
                    machineEtat.writeTelem();
                    machineEtat.sendTelem();
                }else{
                    machineEtat.writeError();
                    machineEtat.sendTelem();
                }
                break;
        }
    }
}