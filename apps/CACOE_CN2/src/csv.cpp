#include "csv.h"

struct LigneCSV* dataEtats;
struct LigneVannes* dataPhysicalConfigVannes;
struct LigneSensors* dataPhysicalConfigSensors;
struct LigneActivation* dataActivation;
struct LigneEG* dataEG;

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

    lireFichierVannes(PHYSICAL_CONFIG_VANNES_DIRECTORY);

    /********************Partie_PhysicalConfig_Sensors********************/


    dataPhysicalConfigSensors = (struct LigneSensors*)malloc(sizeof(struct LigneSensors));
    if (dataPhysicalConfigSensors == NULL) 
    {
        perror("Error allocating memory");
        return errAllocDataPhysSensors;
    }

    memset(dataPhysicalConfigSensors, 0, sizeof(struct LigneSensors));

    lireFichierSensors(PHYSICAL_CONFIG_SENSORS_DIRECTORY);


    /********************Partie_PhysicalConfig_Activation********************/


    dataActivation = (struct LigneActivation*)malloc(sizeof(struct LigneActivation));
    if (dataActivation == NULL) 
    {
        perror("Error allocating memory");
        return errAllocDataActivation;
    }

    memset(dataActivation, 0, sizeof(struct LigneActivation));

    lireFichierActivation(COMMON_PHYSICAL_CONFIG_DIRECTORY);

    return noError;
}

statusErrDef refreshCSV() 
{
    statusErrDef res = noError;

    free(dataEG);
    free(dataEtats);

    /********************Partie_PhysicalConfig_EG_Etat********************/

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

statusErrDef extinctCSV() 
{
    free(dataEG);
    free(dataEtats);
    free(dataPhysicalConfigVannes);
    free(dataPhysicalConfigSensors);
    free(dataActivation);

    return noError;
}

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

statusErrDef lireFichierCSV(const char* dir) 
{
    const char* nameCSV = getNomFichiercsv();
    char cwd[MAX_PATH_LENGTH];
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

void remplirStructure(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) 
    {
        if (colonne == 2) 
        {
            if (atoi(token) == 0)
                dataEtats->valeur[id] = NULL;
            else
                dataEtats->valeur[id] = atoi(token);
        }
        else if (colonne == 4) 
        {
            char* token2 = strtok(token, "|");
            int i = 0;
            while (token2 != NULL) 
            {
                int valeurDependance = atoi(token2);
                dataEtats->dependanceVannes[id][i] = valeurDependance;
                i++;
                token2 = strtok(NULL, "|");
            }
        }
        else if (colonne == 3) 
            dataEtats->timerVannes[id] = atof(token);
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureVannesPhysicalCONFIG(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) 
    {
        if (colonne == 2)
            dataPhysicalConfigVannes->etatInitial[id] = atoi(token);
        else if (colonne == 3)
            dataPhysicalConfigVannes->portGPIO[id] = atoi(token);
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureSensorsPhysicalCONFIG(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) 
    {
        if (colonne == 2) 
            dataPhysicalConfigSensors->etatInitial[id] = atoi(token);
        else if (colonne == 3) 
            dataPhysicalConfigSensors->minValue[id] = atof(token);
        else if (colonne == 4) 
            dataPhysicalConfigSensors->maxValue[id] = atof(token);
        colonne++;
        token = strtok(NULL, ";");
    }
}

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

void remplirEG(char* ligne, int id) 
{
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) 
    {
        if (colonne == 0) 
            dataEG->EG[id] = (int16_t)strtol(token, NULL, 0);
        else if (colonne == 1) 
        {
            removeCarriageReturn(token);
            dataEG->nom[id] = strdup(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

int getValeur(int ligne) 
{
    return dataEtats->valeur[ligne];
}

int* getDependanceVannes(int ligne) 
{
    int* tab = (int*)malloc(MAX_DEPENDANCE * sizeof(int));

    if (tab != NULL)
    {
        for (int i = 0; i < MAX_DEPENDANCE; i++) 
        {
            tab[i] = dataEtats->dependanceVannes[ligne][i];
        }
    }
    else
        return NULL;
    
    return tab;
}

double getTimerVannes(int ligne) 
{
    return dataEtats->timerVannes[ligne];
}

uint8_t getEtatInitialVannes(int ligne) 
{
    return dataPhysicalConfigVannes->etatInitial[ligne];
}

uint8_t getEtatInitialSensors(int ligne) 
{
    return dataPhysicalConfigSensors->etatInitial[ligne];
}

uint8_t getPortGPIO(int ligne) 
{
    return dataPhysicalConfigVannes->portGPIO[ligne];
}

float getMinValue(int ligne) 
{
    return dataPhysicalConfigSensors->minValue[ligne];
}

float getMaxValue(int ligne) 
{
    return dataPhysicalConfigSensors->maxValue[ligne];
}

uint8_t getActivation(int ligne) 
{
    return dataActivation->activation[ligne];
}

int16_t getEGcsv(int ligne) 
{
    return dataEG->EG[ligne];
}

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