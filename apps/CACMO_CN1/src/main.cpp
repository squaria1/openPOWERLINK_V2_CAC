
#include "opl.h"
#include "file.h"
//#include "valve.h"
//#include "sensor.h"

int main() {
    int etat=1;
    opl opl;
    file file;
  

    /*valve valve;
    sensor sensor;*/
    
    while(etat<3){
        switch(etat){
            case 1: // Initialisation
                if (initOPL()) {
                    file.writeTelem();
                    opl.sendTelem();
                }
                else {
                    file.writeError();
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
                if(opl.demandeExtinctOPL()){
                    etat=3;
                }
                break;
            case 3: // Extinction
                if(ExtinctOPL()){
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