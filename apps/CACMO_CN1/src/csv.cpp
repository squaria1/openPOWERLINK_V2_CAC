#include "csv.h"

void lireFichierCSV(char* fileName, struct LigneCSV* data) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructure(ligne, data, id);
        id++;
    }

    fclose(file);
}

void lireFichierVannes(char* fileName, struct LigneVannes* data) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureVannesPhysicalCONFIG(ligne, data, id);
        id++;
    }

    fclose(file);
}

void lireFichierSensors(char* fileName, struct LigneSensors* data) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureSensorsPhysicalCONFIG(ligne, data, id);
        id++;
    }

    fclose(file);
}

void lireFichierCommon(char* fileName, struct LigneActivation* data) {

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    int id = 0;

    char ligne[MAX_LINE_SIZE];
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        ligne[strcspn(ligne, "\n")] = 0;
        remplirStructureSensorsPhysicalCONFIG(ligne, data, id);
        id++;
    }

    fclose(file);
}

void remplirStructure(char* ligne, struct LigneCSV* data, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) {
       /* if (colonne == 0) {
            strcpy(data->carte[id], token);
        }
        else if (colonne == 1) {
            strcpy(data->vannes[id], token);
        }*/
        if (colonne == 2) {
            if (atoi(token) == 0) {
                data->valeur[id] = NULL;
            }
            else {
                data->valeur[id] = atoi(token);
            }
        }
        else if (colonne == 4) {
            char* token2 = strtok(token, "|");
            int i = 0;
            while (token2 != NULL) {
                int valeurDependance = atoi(token2);
                data->dependanceVannes[id][i] = valeurDependance;
                i++;
                token2 = strtok(NULL, "|");
            }
        }
        else if (colonne == 3) {
            data->timerVannes[id] = atof(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureVannesPhysicalCONFIG(char* ligne, struct LigneVannes* data, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;


    while (token != NULL) {
        if (colonne == 2) {
            data->etatInitial[id] = atoi(token);
        }
        else if (colonne == 3) {
            data->portGPIO[id] = atoi(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

void remplirStructureSensorsPhysicalCONFIG(char* ligne, struct LigneSensors* data, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;


    while (token != NULL) {
         if (colonne == 2) {
            data->etatInitial[id] = atoi(token);
        }
        else if (colonne == 3) {
            data->minValue[id] = atof(token);
        }
        else if (colonne == 4) {
            data->maxValue[id] = atof(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

void lireFichierCommon(char* ligne, struct LigneActivation* data, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;


    while (token != NULL) {
        if (colonne == 2) {
            data->activation[id] = atoi(token);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}

char* getCarte(struct LigneCSV* data, int ligne) {
    return data->carte[ligne];
}

char* getVannes(struct LigneCSV* data, int ligne) {
    return data->vannes[ligne];
}

int getValeur(struct LigneCSV* data, int ligne) {
    return data->valeur[ligne];
}

int* getDependanceVannes(struct LigneCSV* data, int ligne) {
    int* tableau = (int*)malloc(MAX_DEPENDANCE * sizeof(int));

    for (int i = 0; i < MAX_DEPENDANCE; i++) {
        tableau[i] = data->dependanceVannes[ligne][i];
    }
    
    return tableau;
}

float getTimerVannes(struct LigneCSV* data, int ligne) {
    return data->timerVannes[ligne];
}

uint8_t getEtatInitialVannes(struct LigneVannes* data, int ligne) {
    return data->etatInitial[ligne];
}

uint8_t getEtatInitialSensors(struct LigneSensors* data, int ligne) {
    return data->etatInitial[ligne];
}

uint8_t getPortGPIO(struct LigneVannes* data, int ligne) {
    return data->portGPIO[ligne];
}

float getMinValue(struct LigneSensors* data, int ligne) {
    return data->minValue[ligne];
}

float getMaxValue(struct LigneSensors* data, int ligne) {
    return data->maxValue[ligne];
}

bool getActivationVannes(struct LigneActivation* data, int ligne) {
    return data->activation[ligne];
}