/**
 * \file sensor.cpp
 * \brief Module to read sensors values
 * \author Mael Parot, Corentin Berthon
 * \version 1
 * \date 11/04/2024
 *
 * Contains all functions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */

#include "sensor.h"

static int opt, adc, i;
static uint8_t tabSensorActivated[MAX_SENSORS];
static int fd[MAX_SENSORS];
static int abort_read;


sensor::sensor()
{
    //constructor
    memset(tabSensorActivated, 0, sizeof(tabSensorActivated));
}

sensor::~sensor()
{
    //destructor
}


statusErrDef sensor::initSensor() {
    statusErrDef res = noError;
    for (int i = 0; i < MAX_SENSORS; i++) 
    {
        tabSensorActivated[i] = getActivation(i + nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 + 2);
    }

    res = readChannels();
    if (res != 0)
        return res;

    return res;
}

statusErrDef sensor::extinctSensor() {
    statusErrDef res = noError;

    closeAdc();

    return res;
}

int16_t getAdc_value(int index) {
    return valSensors[index];
}

statusErrDef readChannels()
{
    int i;
    statusErrDef res = noError;
    memset(fd, 0, sizeof(fd));
    memset(valSensors, 0, sizeof(valSensors));

    for (i = 0; i < MAX_SENSORS; i++) {
        if (tabSensorActivated[i]) {
            fd[i] = openAdc(i);
            if (fd[i] == ADC_READ_ERROR)
            {
                res = errOpenAdc;
                break;
            }
        }
    }
    
    for (i = 0; i < MAX_SENSORS; i++) {
        if (!tabSensorActivated[i])
            continue;

        valSensors[i] = readAdc(fd[i]);

        // reset for next read
        lseek(fd[i], 0, SEEK_SET);

        if (valSensors[i] == ADC_READ_ERROR)
        {
            res = errReadAdc;
            break;
        }
    }

    closeAdc();

    return res;
}

statusErrDef closeAdc()
{
    statusErrDef res = noError;
    int ret = 0;

    for (i = 0; i < MAX_SENSORS; i++) {
        if (fd[i] > 0)
        {
            ret = close(fd[i]);
            if (ret < 0)
            {
                res = errCloseAdc;
                break;
            }
        }
    }

    return res;
}

int readAdc(int fd)
{
    char buff[8];
    buff[7] = 0;

    int val = ADC_READ_ERROR;

    memset(buff, 0, sizeof(buff));

    if (read(fd, buff, 8) < 0)
        perror("read()");
    else
        val = atoi(buff);

    return val;
}

int openAdc(int adc)
{
    char path[128];
    
    sprintf(path, "%sin_voltage%d_raw", IIOSYSPATH, adc);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open()");
        printf("%s\n", path);
        return ADC_READ_ERROR;
    }

    return fd;
}