#include "csv.h"

struct LigneActivation* dataActivation;

int16_t initCSV() {

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

int16_t extinctCSV() {
    free(dataActivation);

    return 0;
}

void removeCarriageReturn(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\r') {
            str[i] = '\0'; // Replace carriage return with null terminator
            break; // Stop after the first carriage return
        }
        i++;
    }
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

void remplirStructureCommon(char* ligne, int id) {
    char* token = strtok(ligne, ";");
    int colonne = 0;

    while (token != NULL) {
        if (colonne == 2)
        {
            dataActivation->activation[id] = atoi(token);
            printf("dataActivation->activation[id]: %d\n", dataActivation->activation[id]);
        }
        colonne++;
        token = strtok(NULL, ";");
    }
}
uint8_t getActivation(int ligne) {
    return dataActivation->activation[ligne];
}