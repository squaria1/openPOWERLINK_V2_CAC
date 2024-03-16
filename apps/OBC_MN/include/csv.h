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

#if (TARGET_SYSTEM == _WIN32_)
#else
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    struct LigneActivation {
        uint8_t activation[MAX_LINE_SIZE];
    };

    statusErrDef initCSV();
    statusErrDef extinctCSV();

    statusErrDef lireFichierActivation(const char* fileName);
    void remplirStructureCommon(char* ligne, int id);

    uint8_t getActivation(int ligne);

    extern struct LigneActivation* dataActivation;

#ifdef __cplusplus
}
#endif

#endif // CSV_H