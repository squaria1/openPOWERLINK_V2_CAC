/**
 * \file valve.cpp
 * \brief Module to control the valves of the CAC Board
 * \author Mael Parot
 * \version 1
 * \date 11/04/2024
 *
 * Contains all functions related to controling valves
 * with the gpiod library.
 */

#include "valve.h"

#if (TARGET_SYSTEM == _WIN32_)
#else
struct gpiod_chip* chip;
struct gpiod_line* lines[MAX_VALVES];;
#endif
unsigned int offsets[MAX_VALVES];
int values[MAX_VALVES];
// Declaration des variables pour le timer
struct timespec beginTimer[MAX_VALVES], endTimer[MAX_VALVES];
double currentTime[MAX_VALVES];
bool timerStarted[MAX_VALVES];

valve::valve()
{

}

valve::~valve()
{

}

#if (TARGET_SYSTEM == _WIN32_)
#else
statusErrDef valve::initValve()
{
    statusErrDef res = noError;

    if (CHIP_PATH == "" || CHIP_PATH == " ") 
    {
        perror("Error: GPIO chip path is not set.");
        return errGPIOPathEmpty;
    }

    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return errOpenGPIO;
    }

    for (int i = 0; i < MAX_VALVES; i++) 
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            offsets[i] = getPortGPIO(i + 1);
            values[i] = getEtatInitialVannes(i + 1);

            lines[i] = gpiod_chip_get_line(chip, offsets[i]);
            if (!lines[i])
            {
                perror("gpiod_chip_get_lines");
                gpiod_line_release(lines[i]);
                gpiod_chip_close(chip);
                return errGPIOGetLine;
            }

            // Request the line for output
            if (gpiod_line_request_output(lines[i], "opl", 0) < 0) {
                perror("Request line as output failed");
                gpiod_line_release(lines[i]);
                gpiod_chip_close(chip);
                return errGPIORequestOutput;
            }
        }
    }

    res = getValvesInitValue();
    if (res != noError)
        return res;
    res = actionnementValvesInit();
    if (res != noError)
        return res;

    return res;
}

statusErrDef valve::actionnementValvesInit()
{
    statusErrDef res = noError;

    for (int i = 0; i < MAX_VALVES; i++) 
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            // Verifiez les valeurs 
            if (values[i] != 0 && values[i] != 1)
            {
                perror("Error: Invalid input value. Must be between 0 and 1.");
                return errValueInitIsNotBinary;
            }
            else
            {
                // Actionnez la vanne
                if (gpiod_line_set_value(lines[i], values[i]) < 0)
                {
                    perror("gpiod_line_set_value");
                    return errGPIOSetInitValue;
                }
                else
                    printf("line init %d activee a %d\n", i, gpiod_line_get_value(lines[i]));
            }
        }
    }

    return res;
}

statusErrDef valve::actionnementValve(int valveNum)
{
    statusErrDef res = noError;

    switch (mode)
    {
    case 0:
        values[valveNum] = getValeur(valveNum + nbValuesCN_In_ByCN + 2);
        break;
    case 1:
        values[valveNum] = getValues_In_CN(valveNum + nbValuesCN_In_ByCN + 1);
        break;
    default:
        break;
    }

    // Verifiez les valeurs 
    if (values[valveNum] != 0 && values[valveNum] != 1)
    {
        perror("Error: Invalid input value. Must be 0 or 1.");
        return errValueIsNotBinary;
    }
    else
    {
        // Actionnez la vanne
        if (gpiod_line_set_value(lines[valveNum], values[valveNum]) < 0)
        {
            perror("gpiod_line_set_value");
            return errGPIOSetValue;
        }
    }

    return res;
}

statusErrDef valve::verifDependanceValves()
{
    statusErrDef res = noError;
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            switch (mode)
            {
            case 0:
                res = isDependanceActive(i + nbValuesCN_In_ByCN + 2);
                switch (res)
                {
                case noError:
                    printf("\n==========\n");
                    printf("ligne:%d\n", i + nbValuesCN_In_ByCN + 2);
                    printf("BEFORE : getValeur(i + nbValuesCN_In_ByCN + 2):%d , gpiod_line_get_value(lines[i]):%d\n",
                        getValeur(i + nbValuesCN_In_ByCN + 2), gpiod_line_get_value(lines[i]));

                    if (!timerStarted[i])
                    {
                        printf("start timer %d !\n", i);
                        startTimerDependance(i);
                    }

                    if (isTimerExeeded(i) == 0)
                    {
                        printf("actionnement valve %d !\n", i);
                        res = actionnementValve(i);
                        if (res != noError)
                            return res;
                        printf("i:%d\n", i);
                        printf("AFTER : getValeur(i + nbValuesCN_In_ByCN + 2):%d , gpiod_line_get_value(lines[i]):%d\n",
                            getValeur(i + nbValuesCN_In_ByCN + 2), gpiod_line_get_value(lines[i]));
                    }
                    break;
                case infoNoDepend:
                    break;
                case infoValveAlreadyActivated:
                    break;
                case infoAllDependNotActivated:
                    break;
                default:
                    return res;
                    break;
                }
                break;
            case 1:
                if (gpiod_line_get_value(lines[i]) < 0)
                    return errGPIOGetValue;
                else if (getValues_In_CN(i + nbValuesCN_In_ByCN + 1) == gpiod_line_get_value(lines[i]))
                    res = infoValveAlreadyActivated;
                else
                {
                    res = actionnementValve(i);
                    if (res != noError)
                        return res;
                }
                break;
            default:
                break;
            }
        }
        
    }

    return res;
}

statusErrDef valve::isDependanceActive(int ligne)
{
    statusErrDef res = noError;

    int* tab;
    int cmpt = 0;
    if (getDependanceVannes(ligne) != NULL)
        tab = getDependanceVannes(ligne);
    else
        return infoNoDepend;

    if (ligne > nbValuesCN_In_ByCN && ligne < nbValuesCN_In_ByCN + nbValuesCN_In)
    {
        if (gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]) < 0)
            return errGPIOGetValue;
        else if (getValeur(ligne) == gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]))
            return infoValveAlreadyActivated;
    }

    do {
        cmpt++;
    } while (tab[cmpt] != 0);
    if (tab[0] != 0)
    {
        for (int i = 0; i < cmpt; i++)
        {
            if (tab[i] > nbValuesCN_In_ByCN && tab[i] < nbValuesCN_In_ByCN + nbValuesCN_In)
            {
                if (gpiod_line_get_value(lines[(tab[i] - 1) % (nbValuesCN_In + 1)]) < 0)
                    return errGPIODependValue;
                else if (getValeur(tab[i] + 1) != gpiod_line_get_value(lines[(tab[i] - 1) % (nbValuesCN_In + 1)]))
                    return infoAllDependNotActivated;
            }
            else
                return errDependOutsideOfRange;
        }
    }

    printf("Dependances toutes activees ! \n");
    return res;
}

int16_t valve::startTimerDependance(int valveNum)
{
    clock_gettime(CLOCK_REALTIME, &beginTimer[valveNum]);
    printf("start timer!\n");
    printf("valveNum:%d\n", valveNum);
    timerStarted[valveNum] = true;

    return 0;
}

int16_t valve::isTimerExeeded(int valveNum)
{
    clock_gettime(CLOCK_REALTIME, &endTimer[valveNum]);
    currentTime[valveNum] = (endTimer[valveNum].tv_sec - beginTimer[valveNum].tv_sec) +
        (endTimer[valveNum].tv_nsec - beginTimer[valveNum].tv_nsec) / 1e9;
    printf("currentTime[%d]=%f\n", valveNum, currentTime[valveNum]);
    printf("getTimerVannes(%d)=%f\n", valveNum + nbValuesCN_In_ByCN + 2, getTimerVannes(valveNum + nbValuesCN_In_ByCN + 2));
    if (currentTime[valveNum] > getTimerVannes(valveNum + nbValuesCN_In_ByCN + 2))
        return 0;
    else
        return 1;
}

statusErrDef valve::extinctValve()
{
    statusErrDef res = noError;
    try
    {
        res = getValvesInitValue();
        if (res != noError)
            return res;
        res = actionnementValvesInit();
        if (res != noError)
            return res;

        for (int i = 0; i < MAX_VALVES; i++)
        {
            if (getActivation(i + nbValuesCN_Out_ByCN + 2))
                gpiod_line_release(lines[i]);
        }
        gpiod_chip_close(chip);
    }
    catch (const std::exception& e)
    {
        perror("extinctValve failed");
        return errGPIORelease;
    }
    return res;
}
#endif

int16_t setValvesValue()
{
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
            values[i] = getValeur(i + nbValuesCN_In_ByCN + 1);
    }

    return 0;
}

statusErrDef getValvesInitValue()
{
    try
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            if (getActivation(i + nbValuesCN_Out_ByCN + 2))
                values[i] = getEtatInitialVannes(i + 1);
        }
    }
    catch (const std::exception& e)
    {
        perror("set valves init value failed");
        return errGetValveInitValue;
    }
    return noError;
}

int16_t getValveValue(int index)
{
    int16_t valveValue = getValeur(index);
    return valveValue;
}


statusErrDef resetTimers() {
    for(int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            currentTime[i] = 0;
            timerStarted[i] = false;
        }
    }

    return noError;
}
