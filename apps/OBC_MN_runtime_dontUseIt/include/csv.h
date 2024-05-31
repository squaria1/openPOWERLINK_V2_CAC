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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <ctype.h>
#include "configDefine.h"
#include "statusErrorDefine.h"
#include "opl.h"

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#if (TARGET_SYSTEM == _WIN32_)
#else
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

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
statusErrDef extinctCSV();

statusErrDef lireFichierActivation(const char* fileName);
statusErrDef lireFichierEG(const char* fileName);
void remplirStructureCommon(char* ligne, int id);
void remplirEG(char* ligne, int id);

uint8_t getActivation(int ligne);
int16_t getEGcsv(int ligne);
void removeCarriageReturn(char* str);

extern struct LigneActivation* dataActivation;
extern struct LigneEG* dataEG;

#ifdef __cplusplus
}
#endif

#endif // CSV_H