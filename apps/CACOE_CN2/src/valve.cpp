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
int16_t valve::initValve()
{
    int16_t res = 0;

    if (CHIP_PATH == "" || CHIP_PATH == " ")
    {
        perror("Error: GPIO chip path is not set.");
        return 0xE301;
    }

    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return 0xE302;
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
                return 0xE303;
            }

            // Request the line for output
            if (gpiod_line_request_output(lines[i], "opl", 0) < 0) {
                perror("Request line as output failed");
                gpiod_line_release(lines[i]);
                gpiod_chip_close(chip);
                return 0xE304;
            }
        }
    }

    res = setValvesInitValue();
    if (res != 0)
        return res;
    res = actionnementValvesInit();
    if (res != 0)
        return res;

    return 0;
}

int16_t valve::actionnementValvesInit()
{
    int16_t res = 0;

    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            // Verifiez les valeurs 
            if (values[i] < 0 || values[i] > 1)
            {
                perror("Error: Invalid input value. Must be between 0 and 1.");
                return 0xE306;
            }
            else
            {
                // Actionnez la vanne 
                res = gpiod_line_set_value(lines[i], values[i]);
                if (res < 0)
                {
                    perror("gpiod_line_set_value");
                    return 0xE307;
                }
                else
                    printf("line init %d activee a %d\n", i, gpiod_line_get_value(lines[i]));
            }
        }
    }

    return 0;
}

int16_t valve::actionnementValve(int valveNum)
{
    int16_t res = 0;
    values[valveNum] = getValeur(valveNum + nbValuesCN_In_ByCN + 2);

    // Verifiez les valeurs 
    if (values[valveNum] < 0 || values[valveNum] > 1)
    {
        perror("Error: Invalid input value. Must be 0 or 1.");
        return 0xE30B;
    }
    else
    {
        // Actionnez la vanne 
        res = gpiod_line_set_value(lines[valveNum], values[valveNum]);
        if (res < 0)
        {
            perror("gpiod_line_set_value");
            return 0xE30C;
        }
    }

    return 0;
}

int16_t valve::verifDependanceValves()
{
    int16_t res = 0;
    for (int i = 0; i < MAX_VALVES; i++)
    {
        //if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        //    printf("getActivation(%d):vrai\n", i + nbValuesCN_Out_ByCN + 2);
        //else
        //    printf("getActivation(%d):faux\n", i + nbValuesCN_Out_ByCN + 2);

        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            res = isDependanceActive(i + nbValuesCN_In_ByCN + 2);
            switch (res)
            {
            case 0:
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
                    actionnementValve(i);
                    printf("i:%d\n", i);
                    printf("AFTER : getValeur(i + nbValuesCN_In_ByCN + 2):%d , gpiod_line_get_value(lines[i]):%d\n",
                        getValeur(i + nbValuesCN_In_ByCN + 2), gpiod_line_get_value(lines[i]));
                }
                break;
            case 0x0303:
                break;
            case 0x0304:
                break;
            case 0x0305:
                break;
            default:
                return res;
                break;
            }


        }
    }

    //printf("-------------------------------------------------\n");

    return 0;
}

int16_t valve::isDependanceActive(int ligne)
{
    int16_t res = 0;
    /*printf("\n==========\n");
    printf("ligne:%d\n", ligne);
    printf("BEFORE : getValeur(ligne):%d , gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In+1)]):%d\n",
        getValeur(ligne), gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]));*/

    int* tab;
    int cmpt = 0;
    if (getDependanceVannes(ligne) != NULL)
        tab = getDependanceVannes(ligne);
    else
        return 0x0303;

    if (ligne > nbValuesCN_In_ByCN && ligne < nbValuesCN_In_ByCN + nbValuesCN_In)
    {
        res = gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]);
        if (res == -1)
            return 0xE308;
        else if (getValeur(ligne) == res)
            return 0x0304;
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
                if (nbValuesCN_In_ByCN != 0)
                {
                    res = gpiod_line_get_value(lines[(tab[i] - 1) % (nbValuesCN_In + 1)]);
                    if (res == -1)
                        return 0xE309;
                    else if (getValeur(tab[i] + 1) != res)
                        return 0x0305;
                }
            }
            else
                return 0xE30A;
        }
    }

    printf("Dependances toutes activees ! \n");
    return 0;
}

int16_t valve::startTimerDependance(int valveNum)
{
    clock_gettime(CLOCK_MONOTONIC, &beginTimer[valveNum]);
    printf("start timer!\n");
    printf("valveNum:%d\n", valveNum);
    timerStarted[valveNum] = true;

    return 0;
}

int16_t valve::isTimerExeeded(int valveNum)
{
    clock_gettime(CLOCK_MONOTONIC, &endTimer[valveNum]);
    currentTime[valveNum] = (endTimer[valveNum].tv_sec - beginTimer[valveNum].tv_sec) +
        (endTimer[valveNum].tv_nsec - beginTimer[valveNum].tv_nsec) / 1e9;
    printf("currentTime[%d]=%f\n", valveNum, currentTime[valveNum]);
    printf("getTimerVannes(%d)=%f\n", valveNum + nbValuesCN_In_ByCN + 2, getTimerVannes(valveNum + nbValuesCN_In_ByCN + 2));
    if (currentTime[valveNum] > getTimerVannes(valveNum + nbValuesCN_In_ByCN + 2))
        return 0;
    else
        return 1;
}

int16_t valve::extinctValve()
{
    try
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            if (getActivation(i + nbValuesCN_Out_ByCN + 2))
            {
                gpiod_line_release(lines[i]);
            }
        }
        gpiod_chip_close(chip);
    }
    catch (const std::exception& e)
    {
        perror("extinctValve failed");
        return 0xE3FF;
    }
    return 0;
}
#endif

int16_t setValvesValue()
{
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            values[i] = getValeur(i + nbValuesCN_In_ByCN + 1);
        }
    }

    return 0;
}

int16_t setValvesInitValue()
{
    try
    {
        for (int i = 0; i < MAX_VALVES; i++)
        {
            if (getActivation(i + nbValuesCN_Out_ByCN + 2))
            {
                values[i] = getEtatInitialVannes(i + 1);
            }
        }
    }
    catch (const std::exception& e)
    {
        perror("telemFiles writeError failed");
        return 0xE305;
    }
    return 0;
}

int16_t getValveValue(int index)
{
    int16_t valveValue = getValeur(index);
    return valveValue;
}


int16_t resetTimers() {
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            currentTime[i] = 0;
            timerStarted[i] = false;
        }
    }

    return 0;
}
