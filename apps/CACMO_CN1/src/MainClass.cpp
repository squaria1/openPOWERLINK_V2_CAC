#include "valve.h"
#include "opl.h"
#include "file.h"
#include "sensor.h"

int main() {
    int etat=1;
    valve valve;
    opl opl;
    file file;
  
    sensor sensor;
    
    while(etat<3){
        switch(etat){
            case 1: // Initialisation
                for(int i=0; i<3;i++){
                    if (opl.initOPL()) {
                        file.writeTelem();
                        opl.sendTelem();
                    }
                    else {
                        file.writeError();
                    }
                    if(valve.initValve()){
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
                    }
                    if(opl.demandeExtinctOPL()){
                        etat=2;
                    }else{
                        etat=3;
                    }
                }
                break;
            case 2: // Acquisition et controle
                if(sensor.isValeurCaptTropEleve()){
                    //opl.demandeExtinctOPL();
                }else{
                    file.writeTelem();
                    opl.sendTelem();
                }
                if(sensor.isPressionTropEleve()){
                    //opl.demandeExtinctOPL();
                }else{
                    file.writeTelem();
                    opl.sendTelem();
                }
                if(sensor.isDebitTropEleve()){
                    //opl.demandeExtinctOPL();
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
                    opl.sendError();
                }
                break;
        }
    }
}