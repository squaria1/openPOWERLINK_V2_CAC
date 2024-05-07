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
// gpio chip of the raspberry gpio controler
struct gpiod_chip* chip;
// gpio chip lines of the raspberry gpio controler
struct gpiod_line* lines[MAX_VALVES];
#endif
// gpio port array
unsigned int offsets[MAX_VALVES];
// gpio values array
int values[MAX_VALVES];
// Declaration des variables pour le timer
struct timespec beginTimer[MAX_VALVES], endTimer[MAX_VALVES];
// current times of the timers
double currentTime[MAX_VALVES];
// values true if a timer is started, false otherwise
bool timerStarted[MAX_VALVES];

valve::valve()
{

}

valve::~valve()
{

}

#if (TARGET_SYSTEM == _WIN32_)
#else
/**
 * \brief function to initialize to valve module
 * 
 * \return statusErrDef that values errGPIOPathEmpty 
 * when the gpio chip path is not set
 * or errOpenGPIO when the gpio chip fails to open
 * or errGPIOGetLine when the gpio lines are busy
 * or errGPIORequestOutput when the gpio lines 
 * can't be set to output mode
 * or noError when the function exits successfully. 
 */
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

/**
 * \brief function to set the valves values
 * from the CSV module initial values.
 * 
 * \return statusErrDef that values errValueInitIsNotBinary 
 * when a value is not 0 or 1.
 * or errGPIOSetInitValue when the line doesn't set the value
 * or noError when the function exits successfully. 
 */
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

/**
 * \brief function to get a valve value
 * either from a general state CSV file if mode
 * is automatic or from a OpenPOWERLINK value from the MN
 * and set the valve to the corresponding gpio line.
 * 
 * \param valveNum the valve number
 * \return statusErrDef that values errValueInitIsNotBinary 
 * when a value is not 0 or 1.
 * or errGPIOSetValue when the line doesn't set the value
 * or noError when the function exits successfully.  
 */
statusErrDef valve::actionnementValve(int valveNum)
{
    statusErrDef res = noError;

    switch (mode)
    {
    case automatic:
        values[valveNum] = getValeur(valveNum + nbValuesCN_In_ByCN + 2);
        break;
    case manual:
        values[valveNum] = getValues_In_CN(valveNum + nbValuesCN_In_ByCN + 1);
        break;
    default:
        break;
    }

    // Verify if the values are either 0 or 1
    if (values[valveNum] != 0 && values[valveNum] != 1)
    {
        perror("Error: Invalid input value. Must be 0 or 1.");
        return errValueIsNotBinary;
    }
    else
    {
        // set the valve to the corresponding gpio line.
        if (gpiod_line_set_value(lines[valveNum], values[valveNum]) < 0)
        {
            perror("gpiod_line_set_value");
            return errGPIOSetValue;
        }
    }

    return res;
}

/**
 * \brief function that verifies if the valves
 * are already set, if not the timer starts 
 * and once the timer is greater than the value 
 * taken from a general state CSV file, it sets 
 * the valve value to the corresponding gpio line.
 * 
 * \return statusErrDef that values errValueInitIsNotBinary 
 * when a value is not 0 or 1.
 * or errGPIOSetValue when the line doesn't set the value
 * or noError when the function exits successfully.   
 */
statusErrDef valve::verifDependanceValves()
{
    statusErrDef res = noError;
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
        {
            //if mode is automatic then we compare the valve dependances 
            //with the dependances in a general state CSV file
            //if mode is manual then we compare and set the value of the gpio lines
            //with the values taken from the MN through the OpenPOWERLINK input array.
            switch (mode)
            {
            case automatic:
                res = isDependanceActive(i + nbValuesCN_In_ByCN + 2);
                switch (res)
                {
                //when all the dependances are activated
                case noError:
                    printf("\n==========\n");
                    printf("ligne:%d\n", i + nbValuesCN_In_ByCN + 2);
                    printf("BEFORE : getValeur(i + nbValuesCN_In_ByCN + 2):%d , gpiod_line_get_value(lines[i]):%d\n",
                        getValeur(i + nbValuesCN_In_ByCN + 2), gpiod_line_get_value(lines[i]));
                    //if the timer is not started then we start it
                    if (!timerStarted[i])
                    {
                        printf("start timer %d !\n", i);
                        startTimerDependance(i);
                    }
                    //if the timer has been exceeded we activate the valve
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
            case manual:
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

/**
 * \brief function to check if
 * every dependance of a valve is activated
 * according to the array of dependance from
 * a general state CSV file.
 * 
 * \param ligne the line in the general state CSV file
 * \return statusErrDef that values errGPIOGetValue 
 * when the getter of a gpio line value fails
 * or infoValveAlreadyActivated when every dependances
 * or errGPIODependValue when the getter of a gpio line
 * dependance value fails
 * or infoAllDependNotActivated when all the dependances
 * of a valve are not activated.
 * or errDependOutsideOfRange when a dependance is
 * not among the valves of the same board.
 * or infoNoDepend when the valve has no dependances.
 * or noError when the function exits successfully.    
 */
statusErrDef valve::isDependanceActive(int ligne)
{
    statusErrDef res = noError;

    int* tab = getDependanceVannes(ligne);
    int cmpt = 0;

    if (tab != NULL)
    {
        //compare the real value of a gpio line with the valve value associated with it in the general state CSV file.
        if (ligne > nbValuesCN_In_ByCN && ligne < nbValuesCN_In_ByCN + nbValuesCN_In)
        {
            //check if we can read the gpio line value before comparing it
            if (gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]) < 0)
            {
                free(tab);
                return errGPIOGetValue;
            }
            else if (getValeur(ligne) == gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]))
            {
                free(tab);
                return infoValveAlreadyActivated;
            }
        }

        //count the number of valve dependances for a valve
        do {
            cmpt++;
        } while (cmpt <= MAX_VALVES && tab[cmpt] != 0);

        if (tab[0] != 0)
        {
            //compare the real value of all gpio lines with the valve values in the general state CSV file.
            for (int i = 0; i < cmpt; i++)
            {
                //check if we can read the gpio line value before comparing it
                if (tab[i] > nbValuesCN_In_ByCN && tab[i] < nbValuesCN_In_ByCN + nbValuesCN_In)
                {
                    if (gpiod_line_get_value(lines[(tab[i] - 1) % (nbValuesCN_In + 1)]) < 0)
                    {
                        free(tab);
                        return errGPIODependValue;
                    }
                    else if (getValeur(tab[i] + 1) != gpiod_line_get_value(lines[(tab[i] - 1) % (nbValuesCN_In + 1)]))
                    {
                        free(tab);
                        return infoAllDependNotActivated;
                    }
                }
                else
                    return errDependOutsideOfRange;
            }
        }

        printf("Dependances toutes activees ! \n");
    }
    else
        res = infoNoDepend;

    free(tab);

    return res;
}

/**
 * \brief function to start the timer
 * when all the dependances of a valve are activated.
 * 
 * \param valveNum the valve number
 * \return 0 if the timer starts successfully
 */
int16_t valve::startTimerDependance(int valveNum)
{
    clock_gettime(CLOCK_REALTIME, &beginTimer[valveNum]);
    printf("start timer!\n");
    printf("valveNum:%d\n", valveNum);
    timerStarted[valveNum] = true;

    return 0;
}

/**
 * \brief function to check if the current value of
 * a timer has exceeded the timer value from a
 * general state CSV file.
 * 
 * \param valveNum the valve number
 * \return 0 if the value has been exceeded, 1 if not
 */
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

/**
 * \brief function to shutdown the valve module.
 * 
 * \return statusErrDef that values errValueInitIsNotBinary 
 * when a value is not 0 or 1
 * or errGPIOSetInitValue when the line doesn't set the value
 * or errGPIORelease when a gpio line fails to be released
 * or noError when the function exits successfully.     
 */
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

/**
 * \brief function setter of the valve values
 * from a general state CSV file.
 * 
 * \return 0 if the valves are set successfully
 */
int16_t setValvesValue()
{
    for (int i = 0; i < MAX_VALVES; i++)
    {
        if (getActivation(i + nbValuesCN_Out_ByCN + 2))
            values[i] = getValeur(i + nbValuesCN_In_ByCN + 1);
    }

    return 0;
}

/**
 * \brief function getter of the initial valve values
 * from the valve physical config CSV file.
 * 
 * \return statusErrDef that values errGetValveInitValue 
 * when the getter of the initial values fails
 * or noError when the function exits successfully.      
 */
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

/**
 * \brief function getter of the valve value
 * from a general state CSV file on Windows
 * or directly from the gpio line value on Linux.
 * 
 * \param ligne the line in a general state CSV file
 * \return the value of the valve
 */
int16_t getValveValue(int ligne)
{
    #if (TARGET_SYSTEM == _WIN32_)
        return getValeur(ligne);
    #else
        return gpiod_line_get_value(lines[(ligne - 2) % (nbValuesCN_In + 1)]);
    #endif
}

/**
 * \brief function to reset the timers
 * after the activation of all the valves.
 * 
 * \return statusErrDef that values noError 
 * when the function exits successfully.
 */
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
