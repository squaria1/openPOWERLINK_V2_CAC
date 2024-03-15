#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <ctype.h>
#include "configDefine.h"
#include "opl.h"

#if (TARGET_SYSTEM == _WIN32_)
#else
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    struct LigneCSV {
        int valeur[MAX_LINE_SIZE];
        int dependanceVannes[MAX_LINE_SIZE][MAX_LINE_SIZE];
        double timerVannes[MAX_LINE_SIZE];
    };

    struct LigneVannes {
        uint8_t etatInitial[MAX_PHYSICAL_LINE_SIZE];
        uint8_t portGPIO[MAX_PHYSICAL_LINE_SIZE];
    };

    struct LigneSensors {
        uint8_t etatInitial[MAX_PHYSICAL_LINE_SIZE];
        float minValue[MAX_PHYSICAL_LINE_SIZE];
        float maxValue[MAX_PHYSICAL_LINE_SIZE];
    };

    struct LigneActivation {
        uint8_t activation[MAX_LINE_SIZE];
    };

    struct LigneEG {
         int16_t EG[MAX_LINE_SIZE];
         char* nom[MAX_LINE_SIZE];
    };

    int16_t initCSV();
    int16_t refreshCSV();
    int16_t extinctCSV();

    void lireFichierCSV(const char* dir);
    void lireFichierVannes(const char* fileName);
    void lireFichierSensors(const char* fileName);
    void lireFichierCommon(const char* fileName);
    void lireFichierEG(const char* fileName);

    void remplirStructure(char* ligne, int id);
    void remplirStructureVannesPhysicalCONFIG(char* ligne, int id);
    void remplirStructureSensorsPhysicalCONFIG(char* ligne, int id);
    void remplirStructureCommon(char* ligne, int id);
    void remplirEG(char* ligne, int id);

    int getValeur(int ligne);
    int* getDependanceVannes(int ligne);
    double getTimerVannes(int ligne);
    uint8_t getEtatInitialVannes(int ligne);
    uint8_t getEtatInitialSensors(int ligne);
    uint8_t getPortGPIO(int ligne);
    float getMinValue(int ligne);
    float getMaxValue(int ligne);
    uint8_t getActivation(int ligne);
    int16_t getEGcsv(int ligne);
    const char* getNomFichiercsv();
    int searchEG();

    extern struct LigneCSV* dataEtats;
    extern struct LigneVannes* dataPhysicalConfigVannes;
    extern struct LigneSensors* dataPhysicalConfigSensors;
    extern struct LigneActivation* dataActivation;
    extern struct LigneEG* dataEG;

#ifdef __cplusplus
}
#endif

#endif // CSV_H