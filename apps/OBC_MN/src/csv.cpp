/**
 * \file csv.cpp
 * \brief Module to read CSV configuration files
 * \author Mael Parot, Benoit Brice
 * \version 1.1
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
 * \brief function getter of the activation of a sensor or a valve
 * from "activation.csv"
 * \param ligne the line in the CSV file
 * \return uint8_t the activation of a sensor or a valve (1 if activated or 0 if not)
 */
uint8_t getActivation(int ligne) {
    return dataActivation->activation[ligne];
}