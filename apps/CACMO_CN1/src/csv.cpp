#include "csv.h"

struct LigneCSV* dataEtats;
struct LigneVannes* dataPhysicalConfigVannes;
struct LigneSensors* dataPhysicalConfigSensors;
struct LigneActivation* dataActivation;
struct LigneEG* dataEG;

int16_t initCSV() {

    refreshCSV();

    /********************Partie_PhysicalConfig_Vannes********************/

    dataPhysicalConfigVannes = (struct LigneVannes*)malloc(sizeof(struct LigneVannes));
    if (dataPhysicalConfigVannes == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    memset(dataPhysicalConfigVannes, 0, sizeof(struct LigneVannes));

    lireFichierVannes(PHYSICAL_CONFIG_VANNES_DIRECTORY);
    printf("Etat Init : %d\n", getEtatInitialVannes(3));
    printf("Port GPIO : %d\n", getPortGPIO(3));

    /********************Partie_PhysicalConfig_Sensors********************/


    dataPhysicalConfigSensors = (struct LigneSensors*)malloc(sizeof(struct LigneSensors));
    if (dataPhysicalConfigSensors == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    memset(dataPhysicalConfigSensors, 0, sizeof(struct LigneSensors));

    lireFichierSensors(PHYSICAL_CONFIG_SENSORS_DIRECTORY);
    printf("Etat Init : %d\n", getEtatInitialSensors(3));
    printf("Min Val : %f\n", getMinValue(3));
    printf("Max Val : %f\n", getMaxValue(3));


    /********************Partie_PhysicalConfig_Activation********************/


    dataActivation = (struct LigneActivation*)malloc(sizeof(struct LigneActivation));
    if (dataActivation == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    memset(dataActivation, 0, sizeof(struct LigneActivation));

    lireFichierCommon(COMMON_PHYSICAL_CONFIG_DIRECTORY);
    printf("Active? : %d\n", getActivation(2));

    return 0;
}

int16_t refreshCSV() {
    free(dataEG);
    free(dataEtats);    
    
    /********************Partie_CSV********************/

    dataEtats = (struct LigneCSV*)malloc(sizeof(struct LigneCSV));
    if (dataEtats == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    memset(dataEtats, 0, sizeof(struct LigneCSV));
    printf("STATE_CSV_DIRECTORY: %s\n", STATE_CSV_DIRECTORY);
    lireFichierCSV(STATE_CSV_DIRECTORY);

    int* monTableau = getDependanceVannes(2);

    int i = 0;
    do {
        printf("%d ", monTableau[i]);
        i++;
    } while (monTableau[i] != 0);

    printf("valeur : %d\n", getValeur(2));
    printf("timer : %f\n", getTimerVannes(2));

    /********************Partie_PhysicalConfig_EG_Etat********************/

    dataEG = (struct LigneEG*)malloc(sizeof(struct LigneEG));
    if (dataEG == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    memset(dataEG, 0, sizeof(struct LigneEG));

    lireFichierEG(EG_ETAT_DIRECTORY);
    printf("EG? : %d\n", getEGcsv(1));
    printf("NomFichierCSV? : %s\n", getNomFichiercsv(1));


    return 0;
}

int16_t extinctCSV() {
    free(dataEG);
    free(dataEtats);
    free(dataPhysicalConfigVannes);
    free(dataPhysicalConfigSensors);
    free(dataActivation);

    return 0;
}

void lireFichierCSV(const char* dir) {
    //char* nameCSV = getNomFichiercsv(EG);
    const char* nameCSV = "Etat_.csv";
    if (nameCSV == NULL) {
        perror("Erreur code EG non trouve dans liaisonEGEtat.csv");
        exit(EXIT_FAILURE);
    }

    char fileName[MAX_PATH_LENGTH];
    strcpy(fileName, dir);
    strcat(fileName, nameCSV);
    printf("fileName lireFichierCSV: %s\n", fileName);
    FILE* file = fopen((const char*)fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier lireFichierCSV");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructure(ligne, id);
        id++;
    }

    fclose(file);
}

void lireFichierVannes(const char* fileName) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier lireFichierVannes");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_PHYSICAL_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureVannesPhysicalCONFIG(ligne, id);
        id++;
    }

    fclose(file);
}

void lireFichierSensors(const char* fileName) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier lireFichierSensors");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_PHYSICAL_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureSensorsPhysicalCONFIG(ligne, id);
        id++;
    }

    fclose(file);
}

void lireFichierCommon(const char* fileName) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier lireFichierCommon");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureCommon(ligne, id);
        id++;
    }

    fclose(file);
}

void lireFichierEG(const char* fileName) {

    printf("fileName lireFichierEG: %s\n", fileName);
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier lireFichierEG");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirEG(ligne, id);
        id++;
    }

    fclose(file);
}

void remplirStructure(char* ligne, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) {
        if (colonne == 2) {
            if (atoi(token) == 0) {
                dataEtats->valeur[id] = NULL;
            }
            else {
                dataEtats->valeur[id] = atoi(token);
            }
        }
        else if (colonne == 4) {
            char* token2 = strtok(token, "|");
            int i = 0;
            while (token2 != NULL) {
                int valeurDependance = atoi(token2);
                dataEtats->dependanceVannes[id][i] = valeurDependance;
                i++;
                token2 = strtok(NULL, "|");
            }
        }
        else if (colonne == 3) {
            dataEtats->timerVannes[id] = atof(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureVannesPhysicalCONFIG(char* ligne, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;


    while (token != NULL) {
        if (colonne == 2) {
            dataPhysicalConfigVannes->etatInitial[id] = atoi(token);
        }
        else if (colonne == 3) {
            dataPhysicalConfigVannes->portGPIO[id] = atoi(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureSensorsPhysicalCONFIG(char* ligne, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) {
        if (colonne == 2) {
            dataPhysicalConfigSensors->etatInitial[id] = atoi(token);
        }
        else if (colonne == 3) {
            dataPhysicalConfigSensors->minValue[id] = atof(token);
        }
        else if (colonne == 4) {
            dataPhysicalConfigSensors->maxValue[id] = atof(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureCommon(char* ligne, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) {
        if (colonne == 2)
            dataActivation->activation[id] = atoi(token);
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirEG(char* ligne, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) {
        if (colonne == 0) {
            dataEG->EG[id] = (int16_t)strtol(token, NULL, 0);
        }
        else if (colonne == 1) {
            dataEG->nom[id] = strdup(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

int getValeur(int ligne) {
    return dataEtats->valeur[ligne];
}

int* getDependanceVannes(int ligne) {
    int* tableau = (int*)malloc(MAX_DEPENDANCE * sizeof(int));

    for (int i = 0; i < MAX_DEPENDANCE; i++) {
        tableau[i] = dataEtats->dependanceVannes[ligne][i];
    }
    
    return tableau;
}

float getTimerVannes(int ligne) {
    return dataEtats->timerVannes[ligne];
}

uint8_t getEtatInitialVannes(int ligne) {
    return dataPhysicalConfigVannes->etatInitial[ligne];
}

uint8_t getEtatInitialSensors(int ligne) {
    return dataPhysicalConfigSensors->etatInitial[ligne];
}

uint8_t getPortGPIO(int ligne) {
    return dataPhysicalConfigVannes->portGPIO[ligne];
}

float getMinValue(int ligne) {
    return dataPhysicalConfigSensors->minValue[ligne];
}

float getMaxValue(int ligne) {
    return dataPhysicalConfigSensors->maxValue[ligne];
}

bool getActivation(int ligne) {
    return dataActivation->activation[ligne];
}

int16_t getEGcsv(int ligne) {
    return dataEG->EG[ligne];
}

char* getNomFichiercsv(int16_t EG) {
    int ligne = searchEG(EG);
    if (ligne != -1)
        return dataEG->nom[ligne];
    else
        return "ERROR_CSV_FILE_NAME";
}

int searchEG(int16_t EG)
{
    int i;
    for (i = 0; i < MAX_LINE_SIZE; i++)
        if (dataEG->EG[i] == EG)
            return i;

    return -1;
}