/**
 * \file csv.h
 * \brief header file of the CSV module that reads CSV configuration files
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all functions related to reading CSV files and 
 * writing the content into structures.
 */

#ifndef CSV_H
#define CSV_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <ctype.h>
#include "configDefine.h"
#include "statusErrorDefine.h"
#include "opl.h"

#if (TARGET_SYSTEM == _WIN32_)
#else
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \struct LigneCSV
 * \brief struct containing values taken from a general state CSV
 * such as the value, dependance and timer dependance of each valves.
 * 
 */
struct LigneCSV {
    int valeur[MAX_LINE_SIZE];                              /**< the set value of a valve */
    int dependanceVannes[MAX_LINE_SIZE][MAX_VALVES];        /**< the valve dependances array */
    double timerVannes[MAX_LINE_SIZE];                      /**< the timer after which the valve is triggered */
};

/**
 * \struct LigneVannes
 * \brief struct containing values taken from "physicalCONFIG_valves.csv"
 * such as the initial values, GPIO ports of each valves.
 * 
 */
struct LigneVannes {
    uint8_t etatInitial[2*MAX_VALVES];                      /**< the initial value of a valve */
    uint8_t portGPIO[2*MAX_VALVES];                         /**< the GPIO port associated with the valve */
};

/**
 * \struct LigneSensors
 * \brief struct containing values taken from "physicalCONFIG_sensors.csv"
 * such as the type and modbus parameters of each sensors.
 * 
 */
struct LigneSensors {
    uint8_t  type[2*MAX_SENSORS];                           /**< the type of sensor (1 for an MCP3008 channel and 2 for modbus) */
    uint16_t modbusAddrRemoteSlave[2*MAX_SENSORS];          /**< the connected sensor address */
    uint16_t modbusStartAddress[2*MAX_SENSORS];             /**< the read start address */
    uint32_t modbusBaudRate[2*MAX_SENSORS];                 /**< the modbus connexion baud rate */
    char     modbusParity[2*MAX_SENSORS];                   /**< the modbus parity (N for None, E for Even and O for Odd) */
    uint16_t modbusDataBits[2*MAX_SENSORS];                 /**< the modbus databits number (5,6,7 or 8) */
    uint16_t modbusStopBit[2*MAX_SENSORS];                  /**< the modbus stopbit number (1 or 2) */
};

/**
 * \struct LigneActivation
 * \brief struct containing the boolean values taken from "activation.csv"
 * 
 */
struct LigneActivation {
    uint8_t activation[MAX_LINE_SIZE];                      /**< the activation of a valve, sensor or board state (1 for activated, 0 otherwise) */
};

/**
 * \struct LigneEG
 * \brief struct containing the general state code 
 * according to the name of a general state CSV file taken from "liaisonEGEtat.csv"
 * 
 */
struct LigneEG {
        int16_t EG[MAX_LINE_SIZE];                          /**< the MN general state code */
        char* nom[MAX_LINE_SIZE];                           /**< the general state CSV file name associated with the MN general state code */
};

statusErrDef initCSV();
statusErrDef refreshCSV();
statusErrDef extinctCSV();

statusErrDef lireFichierCSV(const char* dir);
statusErrDef lireFichierVannes(const char* fileName);
statusErrDef lireFichierSensors(const char* fileName);
statusErrDef lireFichierActivation(const char* fileName);
statusErrDef lireFichierEG(const char* fileName);

void remplirStructure(char* ligne, int id);
void remplirStructureVannesPhysicalCONFIG(char* ligne, int id);
void remplirStructureSensorsPhysicalCONFIG(char* ligne, int id);
void remplirStructureCommon(char* ligne, int id);
void remplirEG(char* ligne, int id);

int getValeur(int ligne);
int* getDependanceVannes(int ligne);
double getTimerVannes(int ligne);
uint8_t getEtatInitialVannes(int ligne);
uint8_t getPortGPIO(int ligne);
uint8_t getSensorType(int ligne);
uint16_t getModbusAddrRemoteSlave(int ligne);
uint16_t getModbusStartAddress(int ligne);
uint32_t getModbusBaudRate(int ligne);
char getModbusParity(int ligne);
uint16_t getModbusDataBits(int ligne);
uint16_t getModbusStopBit(int ligne);
uint8_t getActivation(int ligne);
int16_t getEGcsv(int ligne);
const char* getNomFichiercsv();
int searchEG();
void removeCarriageReturn(char* str);

extern struct LigneCSV* dataEtats;
extern struct LigneVannes* dataPhysicalConfigVannes;
extern struct LigneSensors* dataPhysicalConfigSensors;
extern struct LigneActivation* dataActivation;
extern struct LigneEG* dataEG;

#ifdef __cplusplus
}
#endif

#endif // CSV_H