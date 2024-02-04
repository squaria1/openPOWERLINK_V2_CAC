#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LINE_SIZE 2832
#define MAX_FIELD_SIZE 2832
#define MAX_ROWS 5
#define MAX_DEPENDANCE 2832
#define MAX_SENSORS_PER_BOARD 12
#define STATE_CSV_DIRECTORY "..\\..\\..\\include\\stateCSV\\Etat_.csv"

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
    bool sensorActivated[MAX_SENSORS_PER_BOARD];
};



void remplirStructure(char* ligne, struct LigneCSV* data, int id);
void lireFichier(char* fileName, struct LigneCSV* data);
char* getCarte(struct LigneCSV* data, int ligne);
char* getVannes(struct LigneCSV* data, int ligne);
int getValeur(struct LigneCSV* data, int ligne);
int* getDependanceVannes(struct LigneCSV* data, int ligne);
float getTimerVannes(struct LigneCSV* data, int ligne);
bool getSensorActivated(struct LigneCSV* data, int ligne);

#ifdef __cplusplus
}
#endif

#endif // CSV_H