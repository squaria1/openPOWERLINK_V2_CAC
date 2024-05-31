/**
 * \file csv.cpp
 * \brief Module to read CSV configuration files
 * \author Mael Parot, Benoit Brice
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all functions related to reading CSV files and 
 * writing the content into structures.
 */

#include "csv.h"

/**
 * \struct dataActivation
 * \brief struct containing the boolean values taken from "activation.csv"
 * 
 */
struct LigneActivation* dataActivation;

/**
 * \struct dataEG
 * \brief struct containing the general state code
 * according to the name of a general state CSV file taken from "liaisonEGEtat.csv"
 *
 */
struct LigneEG* dataEG;

/**
 * \brief function to initialize the struct containing the values
 * and read the the activation CSV file.
 * 
 * \return statusErrDef that values errAllocDataPhysValves, errAllocDataPhysSensors,
 * errAllocDataActivation in case of a malloc allocation error
 * or noError when the function exits successfully.
 */
statusErrDef initCSV() {
    statusErrDef res = noError;

    /********************Partie_PhysicalConfig_Activation********************/

    dataActivation = (struct LigneActivation*)malloc(sizeof(struct LigneActivation));
    if (dataActivation == NULL)
    {
        perror("Error allocating memory");
        return errAllocDataActivation;
    }

    memset(dataActivation, 0, sizeof(struct LigneActivation));

    res = lireFichierActivation(COMMON_PHYSICAL_CONFIG_DIRECTORY);
    if (res != noError)
        return res;

    /********************Partie_EG_Etat********************/

    dataEG = (struct LigneEG*)malloc(sizeof(struct LigneEG));
    if (dataEG == NULL)
    {
        perror("Error allocating memory");
        return errAllocDataEG;
    }

    memset(dataEG, 0, sizeof(struct LigneEG));

    res = lireFichierEG(EG_ETAT_DIRECTORY);
    if (res != noError)
        return res;

    return noError;
}

/**
 * \brief function to free the 5 structs
 * 
 * \return statusErrDef 
 * noError when the function exits successfully.
 */
statusErrDef extinctCSV()
{
    free(dataActivation);
    free(dataEG);

    return noError;
}

/**
 * \brief function to read "activation.csv"
 * until the end of the file.
 * 
 * \param fileName location and name of the CSV file to read
 * \return statusErrDef 
 * errOpenPhysSensorsFile when the file fails to open,
 * noError when the function exits successfully.
 */
statusErrDef lireFichierActivation(const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier lireFichierActivation");
        return errOpenActivationFile;
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL)
    {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureCommon(ligne, id);
        id++;
    }

    fclose(file);
    return noError;
}

/**
 * \brief function to read "liaisonEGEtat.csv"
 * until the end of the file.
 *
 * \param fileName location and name of the CSV file to read
 * \return statusErrDef
 * errOpenEGFile when the file fails to open,
 * noError when the function exits successfully.
 */
statusErrDef lireFichierEG(const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier lireFichierEG");
        return errOpenEGFile;
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL)
    {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirEG(ligne, id);
        id++;
    }

    fclose(file);

    return noError;
}

/**
 * \brief function to fill in the activation CSV structure.
 * 
 * \param ligne the CSV line to read.
 * \param id the position of the line in the CSV file.
 */
void remplirStructureCommon(char* ligne, int id)
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL)
    {
        if (colonne == 2)
            dataActivation->activation[id] = atoi(token);
        colonne++;
        token = strtok(NULL, ";");
    }
}

/**
 * \brief function to fill in the link between EG and CSV file name structure.
 *
 * \param ligne the CSV line to read.
 * \param id the position of the line in the CSV file.
 */
void remplirEG(char* ligne, int id)
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL)
    {
        switch (colonne)
        {
        case 0:
            dataEG->EG[id] = (int16_t)strtol(token, NULL, 0);
            break;
        case 1:
            removeCarriageReturn(token);
            dataEG->nom[id] = strdup(token);
            break;
        default:
            break;
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

/**
 * \brief function getter of the activation of a sensor or a valve
 * from "activation.csv"
 * \param ligne the line in the CSV file
 * \return uint8_t the activation of a sensor or a valve (1 if activated or 0 if not)
 */
uint8_t getActivation(int ligne) {
    return dataActivation->activation[ligne];
}

/**
 * \brief function getter of the general state
 * from "liaisonEGEtat.csv"
 * \param ligne the line in the CSV file
 * \return uint16_t the value of the general state of a specific line
 */
int16_t getEGcsv(int ligne)
{
    return dataEG->EG[ligne];
}

/**
 * \brief function that removes the carriage return
 * from the end a line of a general state CSV file before strdup is called.
 * \param str the line in the CSV file
 */
void removeCarriageReturn(char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\r')
        {
            str[i] = '\0'; // Replace carriage return with null terminator
            break; // Stop after the first carriage return
        }
        i++;
    }
}