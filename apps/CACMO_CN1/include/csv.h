#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <configDefine.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct LigneCSV {
        char* carte[256];
        char* vannes[256];
        int valeur[256];
        int dependanceVannes[256][256];
        float timerVannes[256];
    };

    struct LigneVannes {
        uint8_t etatInitial[256];
        uint8_t portGPIO[256];
    };

    struct LigneSensors {
        uint8_t etatInitial[256];
        float minValue[256];
        float maxValue[256];
    };

    struct LigneActivation {
        bool activation[256];
    };

    void lireFichierCSV(char* fileName, struct LigneCSV* data);
    void lireFichierVannes(char* fileName, struct LigneVannes* data);
    void lireFichierSensors(char* fileName, struct LigneSensors* data);
    void lireFichierCommon(char* fileName, struct LigneActivation* data);

    void remplirStructureVannesPhysicalCONFIG(char* ligne, struct LigneVannes* data, int id);
    void remplirStructureSensorsPhysicalCONFIG(char* ligne, struct LigneSensors* data, int id);
    void remplirStructure(char* ligne, struct LigneCSV* data, int id);
    void remplirStructureCommon(char* ligne, struct LigneActivation* data, int id);

    int getValeur(struct LigneCSV* data, int ligne);
    int* getDependanceVannes(struct LigneCSV* data, int ligne);
    float getTimerVannes(struct LigneCSV* data, int ligne);
    uint8_t getEtatInitialVannes(struct LigneVannes* data, int ligne);
    uint8_t getEtatInitialSensors(struct LigneSensors* data, int ligne);
    uint8_t getPortGPIO(struct LigneVannes* data, int ligne);
    float getMinValue(struct LigneSensors* data, int ligne);
    float getMaxValue(struct LigneSensors* data, int ligne);


#ifdef __cplusplus
}
#endif

#endif // CSV_H