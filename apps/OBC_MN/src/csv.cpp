#include "csv.h"

struct LigneActivation* dataActivation;

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

statusErrDef extinctCSV()
{
    free(dataActivation);

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

uint8_t getActivation(int ligne) {
    return dataActivation->activation[ligne];
}