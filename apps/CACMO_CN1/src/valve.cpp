#include "valve.h"

#if (TARGET_SYSTEM == _WIN32_)
#else
struct gpiod_chip* chip;
struct gpiod_line* lines[MAX_VALVES];;
#endif
unsigned int offsets[MAX_VALVES];
int values[MAX_VALVES];
int err;
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

void valve::test()
{
    for (int i = 0; i < MAX_VALVES; i++) 
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            offsets[i] = getPortGPIO(i + 1);
            values[i] = getEtatInitialVannes(i + 1);
            printf("offsets[%d]:%d\n", i, offsets[i]);
            printf("values[%d]:%d\n", i, values[i]);
        }
    }
}

#if (TARGET_SYSTEM == _WIN32_)
#else
int16_t valve::initValve()
{
    if (CHIP_PATH=="" || CHIP_PATH==" ") {
        perror("Error: GPIO chip path is not set.");
        return 1;
    }

    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return 1;
    }

    for (int i = 0; i < MAX_VALVES; i++) {
        if (getActivation(i + nbValuesCN_Out_ByCN + 1))
        {
            offsets[i] = getPortGPIO(i+1);
            values[i] = getEtatInitialVannes(i+1);

            lines[i] = gpiod_chip_get_line(chip, offsets[i]);
            if (!lines[i])
            {
                perror("gpiod_chip_get_lines");
                gpiod_line_release(lines[i]);
                gpiod_chip_close(chip);
                return 1;
            }

            // Request the line for output
            if (gpiod_line_request_output(lines[i], "opl", 0) < 0) {
                perror("Request line as output failed");
                gpiod_line_release(lines[i]);
                gpiod_chip_close(chip);
                return 1;
            }
        }
    }

    setValveInitValue();
    actionnementValves();

    return 0;
}

int16_t valve::actionnementValvesInit()
{

    printf("offsets[%d]:%d\n", i, offsets[i]);
    printf("values[%d]:%d\n", i, values[i]);

    for (int i = 0; i < MAX_VALVES; i++) {
        if (getActivation(i + nbValuesCN_Out_ByCN + 1))
        {
            // Verifiez les valeurs 
            if (values[i] < 0 || values[i] > 1)
            {
                perror("Error: Invalid input value. Must be 0 or 1.");
                return 1;
            }
            else
            {
                // Actionnez la vanne 
                err = gpiod_line_set_value(lines[i], values[i]);
                if (err)
                {
                    perror("gpiod_line_set_value");
                    return 1;
                }
            }
        }
    }

    return 0;
}

int16_t valve::actionnementValve(int valveNum)
{
    setValveValue();

    // Verifiez les valeurs 
    if (values[valveNum] < 0 || values[valveNum] > 1)
    {
        perror("Error: Invalid input value. Must be 0 or 1.");
        return 1;
    }
    else
    {
        // Actionnez la vanne 
        err = gpiod_line_set_value(lines[valveNum], values[valveNum]);
        if (err)
        {
            perror("gpiod_line_set_value");
            return 1;
        }
    }

    return 0;
}

int16_t valve::verifDependanceValves()
{
    for (int i = 0; i < MAX_VALVES; i++) 
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 1) &&
            isDependanceActive(i + nbValuesCN_In_ByCN + 2) &&
            !timerStarted[i])
        {
            printf("start timer %d !\n", i);
            startTimerDependance(i);
        }
        
        if (getActivation(i + nbValuesCN_Out_ByCN + 1) &&
            isDependanceActive(i + nbValuesCN_In_ByCN + 2) &&
            isTimerExeeded(i))
        {
            printf("actionnement valve %d !\n",i);
            actionnementValve(i);
        }
    }

    return 0;
}

int16_t valve::isDependanceActive(int ligne)
{
    int* tab;
    if (getDependanceVannes(ligne) != NULL)
        tab = getDependanceVannes(ligne);
    else
        return 2;

    for (int i = 0; i < sizeof(tab) / sizeof(int); i++)
    {
        printf("tab[%d]:%d\n", i, tab[i]);
        if (tab[i] > nbValuesCN_In_ByCN && tab[i] < nbValuesCN_In_ByCN + nbValuesCN_In)
        {
            printf("getValeur(tab[i] + 2):%d , gpiod_line_get_value(lines[tab[i]]):%d\n", 
                getValeur(tab[i] + 2), gpiod_line_get_value(lines[tab[i]]));

            if (getValeur(tab[i] + 2) != gpiod_line_get_value(lines[tab[i]]))
                return 1;
        }
        else if(tab[i] % (nbValuesCN_In + 1) != 0)
        {

            printf("getValeur(tab[i] + 2):%d , getValues_In_CN(tab[i]):%d\n",
                getValeur(tab[i] + 2), getValues_In_CN(tab[i]));
            if (getValeur(tab[i] + 2) != getValues_In_CN(tab[i]))
                return 1;
        }
    }

    return 0;
}

int16_t valve::startTimerDependance(int valveNum)
{
    clock_gettime(CLOCK_MONOTONIC, &beginTimer[valveNum]);
    printf("timer!\n");
    timerStarted[valveNum] = true;

    return 0;
}

int16_t valve::isTimerExeeded(int valveNum)
{
    clock_gettime(CLOCK_MONOTONIC, &endTimer[valveNum]);
    currentTime = (endTimer[valveNum].tv_sec - beginTimer[valveNum].tv_sec) +
        (endTimer[valveNum].tv_nsec - beginTimer[valveNum].tv_nsec) / 1e9;

    if (currentTime > getTimerVannes(ligne))
    {
        timerStarted[valveNum] = false;
        return 0;
    }
    else
        return 1;
}

int16_t valve::extinctValve()
{
    for (int i = 0; i < MAX_VALVES; i++) {
        if (getActivation(i + nbValuesCN_Out_ByCN + 1))
        {
            gpiod_line_release(lines[i]);
        }
    }
    gpiod_chip_close(chip);

    return 0;
}
#endif

int16_t setValvesValue()
{
    for (int i = 0; i < MAX_VALVES; i++) 
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 1))
        {
            values[i] = getValeur(i + nbValuesCN_In_ByCN + 1);
        }
    }

    return 0;
}

int16_t setValvesInitValue()
{
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            values[i] = getEtatInitialVannes(i + 1);
        }
    }

    return 0;
}

int16_t getValveValue(int index)
{
    int16_t valveValue = getValeur(index);
    return valveValue;
}
