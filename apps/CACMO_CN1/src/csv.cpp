#include "csv.h"

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

void lireFichier(char* fileName, struct LigneCSV* data) {
    
    //strcat(STATE_CSV_DIRECTORY, fileName)
    FILE* file = fopen(STATE_CSV_DIRECTORY, "r");
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

bool getSensorActivated(struct LigneCSV* data, int ligne) {
    return data->sensorActivated[ligne];
}

float getTimerVannes(struct LigneCSV* data, int ligne) {
    return data->timerVannes[ligne];
}

