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
  * \struct dataEtats
  * \brief struct containing values taken from a general state CSV
  * such as the value, dependance and timer dependance of each valves.
  * 
  */
struct LigneCSV* dataEtats;

 /**
  * \struct dataPhysicalConfigVannes
  * \brief struct containing values taken from "physicalCONFIG_valves.csv"
  * such as the initial values, GPIO ports of each valves.
  * 
  */
struct LigneVannes* dataPhysicalConfigVannes;

 /**
  * \struct dataPhysicalConfigSensors
  * \brief struct containing values taken from "physicalCONFIG_sensors.csv"
  * such as the calibrated, min and max values of each sensors.
  * 
  */
struct LigneSensors* dataPhysicalConfigSensors;

 /**
  * \struct dataActivation
  * \brief struct containing the boolean values taken from "activation.csv"
  * 
  */
struct LigneActivation* dataActivation;

 /**
  * \struct dataActivation
  * \brief struct containing the general state code 
  * according to the name of a general state CSV file taken from "liaisonEGEtat.csv"
  * 
  */
struct LigneEG* dataEG;

/**
 * \brief function to initialize the struct containing the values
 * and read the 5 different CSV files.
 * 
 * \return statusErrDef that values errAllocDataPhysValves, errAllocDataPhysSensors,
 * errAllocDataActivation in case of a malloc allocation error
 * or noError when the function exits successfully.
 */
statusErrDef initCSV() 
{
    statusErrDef res = noError;

    refreshCSV();

    /********************Partie_PhysicalConfig_Vannes********************/

    dataPhysicalConfigVannes = (struct LigneVannes*)malloc(sizeof(struct LigneVannes));
    if (dataPhysicalConfigVannes == NULL) 
    {
        perror("Error allocating memory");
        return errAllocDataPhysValves;
    }

    memset(dataPhysicalConfigVannes, 0, sizeof(struct LigneVannes));

    res = lireFichierVannes(PHYSICAL_CONFIG_VANNES_DIRECTORY);
    if (res != noError)
        return res;

    /********************Partie_PhysicalConfig_Sensors********************/


    dataPhysicalConfigSensors = (struct LigneSensors*)malloc(sizeof(struct LigneSensors));
    if (dataPhysicalConfigSensors == NULL) 
    {
        perror("Error allocating memory");
        return errAllocDataPhysSensors;
    }

    memset(dataPhysicalConfigSensors, 0, sizeof(struct LigneSensors));

    res = lireFichierSensors(PHYSICAL_CONFIG_SENSORS_DIRECTORY);
    if (res != noError)
        return res;


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
 * \brief function to get values of the new general state CSV and 
 * "liaisonEGEtat.csv"
 * 
 * \return statusErrDef that values errAllocDataEG
 , errAllocDataEtats,
 * in case of a malloc allocation error
 * or noError when the function exits successfully.
 */
statusErrDef refreshCSV() 
{
    statusErrDef res = noError;

    free(dataEG);
    free(dataEtats);

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

    /********************Partie_CSV********************/

    dataEtats = (struct LigneCSV*)malloc(sizeof(struct LigneCSV));
    if (dataEtats == NULL) 
    {
        perror("Error allocating memory");
        return errAllocDataEtats;
    }

    memset(dataEtats, 0, sizeof(struct LigneCSV));
    res = lireFichierCSV(STATE_CSV_DIRECTORY);
    if (res != noError)
        return res;

    return res;
}

/**
 * \brief function to free the 5 structs
 * 
 * \return statusErrDef 
 * noError when the function exits successfully.
 */
statusErrDef extinctCSV() 
{
    free(dataEG);
    free(dataEtats);
    free(dataPhysicalConfigVannes);
    free(dataPhysicalConfigSensors);
    free(dataActivation);

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
 * \brief function to read a general state CSV file
 * until the end of the file.
 * 
 * \param dir location of the CSV file to read
 * \return statusErrDef 
 * errEGNotFoundInFile general code (EG) not found in liaisonEGEtat.csv linked with the CSV file name, 
 * errOpenEtatsFile when the file fails to open,
 * noError when the function exits successfully.
 */
statusErrDef lireFichierCSV(const char* dir) 
{
    const char* nameCSV = getNomFichiercsv();
    printf("nameCSV: %s\n", nameCSV);
    if (nameCSV == NULL) 
    {
        perror("Erreur code EG non trouve dans liaisonEGEtat.csv");
        return errEGNotFoundInFile;
    }

    char fileName[MAX_PATH_LENGTH];
    snprintf(fileName, sizeof(fileName), "%s%s", dir, nameCSV);
    FILE* file = fopen(fileName, "r");
    if (file == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier lireFichierCSV");
        return errOpenEtatsFile;
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) 
    {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructure(ligne, id);
        id++;
    }

    fclose(file);
    return noError;
}

/**
 * \brief function to read "physicalCONFIG_valves.csv"
 * until the end of the file.
 * 
 * \param fileName location and name of the CSV file to read
 * \return statusErrDef 
 * errOpenPhysValvesFile when the file fails to open,
 * noError when the function exits successfully.
 */
statusErrDef lireFichierVannes(const char* fileName) 
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier lireFichierVannes");
        return errOpenPhysValvesFile;
    }
    int id = 0;

    char ligne[MAX_PHYSICAL_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) 
    {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureVannesPhysicalCONFIG(ligne, id);
        id++;
    }

    fclose(file);
    return noError;
}

/**
 * \brief function to read "physicalCONFIG_sensors.csv"
 * until the end of the file.
 * 
 * \param fileName location and name of the CSV file to read
 * \return statusErrDef 
 * errOpenPhysSensorsFile when the file fails to open,
 * noError when the function exits successfully.
 */
statusErrDef lireFichierSensors(const char* fileName) 
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier lireFichierSensors");
        return errOpenPhysSensorsFile;
    }
    int id = 0;

    char ligne[MAX_PHYSICAL_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) 
    {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureSensorsPhysicalCONFIG(ligne, id);
        id++;
    }

    fclose(file);
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
 * \brief function to fill in the general CSV structure.
 * 
 * \param ligne the CSV line to read.
 * \param id the position of the line in the CSV file.
 */
void remplirStructure(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    char* token2 = NULL;
    int colonne = 0, i = 0, valeurDependance = 0;

    while (token != NULL) 
    {
        switch (colonne)
        {
        case 2:
            if (atoi(token) == 0)
                dataEtats->valeur[id] = 0;
            else
                dataEtats->valeur[id] = atoi(token);
            break;
        case 3:
            dataEtats->timerVannes[id] = atof(token);
            break;
        case 4:
            token2 = strtok(token, "|");
            i = 0;
            while (token2 != NULL)
            {
                valeurDependance = atoi(token2);
                dataEtats->dependanceVannes[id][i] = valeurDependance;
                i++;
                token2 = strtok(NULL, "|");
            }
            break;
        default:
            break;
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

/**
 * \brief function to fill in the physical config valves structure.
 * 
 * \param ligne the CSV line to read.
 * \param id the position of the line in the CSV file.
 */
void remplirStructureVannesPhysicalCONFIG(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) 
    {
        switch (colonne)
        {
        case 2:
            dataPhysicalConfigVannes->etatInitial[id] = atoi(token);
            break;
        case 3:
            dataPhysicalConfigVannes->portGPIO[id] = atoi(token);
            break;
        default:
            break;
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

/**
 * \brief function to fill in the physical config sensors structure.
 * 
 * \param ligne the CSV line to read.
 * \param id the position of the line in the CSV file.
 */
void remplirStructureSensorsPhysicalCONFIG(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) 
    {
        switch (colonne)
        {
        case 2:
            dataPhysicalConfigSensors->type[id] = atoi(token);
            break;
        case 3:
            dataPhysicalConfigSensors->modbusAddrRemoteSlave[id] = atoi(token);
            break;
        case 4:
            dataPhysicalConfigSensors->modbusStartAddress[id] = atoi(token);
            break;
        case 5:
            dataPhysicalConfigSensors->modbusBaudRate[id] = atoi(token);
            break;
        case 6:
            dataPhysicalConfigSensors->modbusParity[id] = token[0];
            break;
        case 7:
            dataPhysicalConfigSensors->modbusDataBits[id] = atoi(token);
            break;
        case 8:
            dataPhysicalConfigSensors->modbusStopBit[id] = atoi(token);
            break;
        default:
            break;
        }
        colonne++;
        token = strtok(NULL, ";");
    }
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
 * \brief function getter of the value of valves from a general state CSV
 * 
 * \param ligne the line in the CSV file
 * \return int the value of the valve from a specific line
 */
int getValeur(int ligne) 
{
    return dataEtats->valeur[ligne];
}

/**
 * \brief function getter of all the dependances for a valve 
 * from a general state CSV
 * \param ligne the line in the CSV file
 * \return int* an array of all the dependances for a valve
 */
int* getDependanceVannes(int ligne) 
{
    int* tab = (int*)malloc(MAX_VALVES * sizeof(int));

    if (tab != NULL)
    {
        for (int i = 0; i < MAX_VALVES; i++) 
        {
            tab[i] = dataEtats->dependanceVannes[ligne][i];
        }
    }
    else
        return NULL;
    
    return tab;
}

/**
 * \brief function getter of the timer until value is set for a valve
 * from a general state CSV
 * \param ligne the line in the CSV file
 * \return double the value of the timer
 */
double getTimerVannes(int ligne) 
{
    return dataEtats->timerVannes[ligne];
}

/**
 * \brief function getter of the initial value of a valve
 * from "physicalCONFIG_valves.csv"
 * \param ligne the line in the CSV file
 * \return uint8_t the initial value of the valve
 */
uint8_t getEtatInitialVannes(int ligne) 
{
    return dataPhysicalConfigVannes->etatInitial[ligne];
}

/**
 * \brief function getter of the GPIO port of a valve
 * from "physicalCONFIG_valves.csv"
 * \param ligne the line in the CSV file
 * \return uint8_t the GPIO port of the valve
 */
uint8_t getPortGPIO(int ligne) 
{
    return dataPhysicalConfigVannes->portGPIO[ligne];
}

uint8_t getSensorType(int ligne)
{
    return dataPhysicalConfigSensors->type[ligne];
}

uint16_t getModbusAddrRemoteSlave(int ligne)
{
    return dataPhysicalConfigSensors->modbusAddrRemoteSlave[ligne];
}

uint16_t getModbusStartAddress(int ligne)
{
    return dataPhysicalConfigSensors->modbusStartAddress[ligne];
}

uint32_t getModbusBaudRate(int ligne)
{
    return dataPhysicalConfigSensors->modbusBaudRate[ligne];
}

char getModbusParity(int ligne)
{
    return dataPhysicalConfigSensors->modbusParity[ligne];
}

uint16_t getModbusDataBits(int ligne)
{
    return dataPhysicalConfigSensors->modbusDataBits[ligne];
}

uint16_t getModbusStopBit(int ligne)
{
    return dataPhysicalConfigSensors->modbusStopBit[ligne];
}

/**
 * \brief function getter of the activation of a sensor or a valve
 * from "activation.csv"
 * \param ligne the line in the CSV file
 * \return uint8_t the activation of a sensor or a valve (1 if activated or 0 if not)
 */
uint8_t getActivation(int ligne) 
{
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
 * \brief function getter of the name of a general state CSV file
 * of the current general state (EG)
 * \return const char* the name of a general state CSV file
 */
const char* getNomFichiercsv() 
{
    int ligne = searchEG();
    if (ligne != -1)
    {
        return dataEG->nom[ligne];
    }
    else
        return NULL;
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

/**
 * \brief search the general state (EG) 
 * from the link between EG and CSV file name structure
 * 
 * \return int the general state (EG) 
 */
int searchEG()
{
    int i;
    for (i = 0; i < MAX_LINE_SIZE; i++)
    {
        if (dataEG->EG[i] == EG)
            return i;
    }
    return -1;
}