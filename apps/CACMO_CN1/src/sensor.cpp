#include "sensor.h"
#include "csv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h> 

#if (TARGET_SYSTEM == _WIN32_)
#include <getopt/getopt.h>
#else
#include <unistd.h>
#endif
#if (TARGET_SYSTEM == _WIN32_)
#include <io.h>
#else 
#include <sys/ioctl.h>
#endif


char iiosyspath[] = "/sys/bus/iio/devices/iio:device0/";


sensor::sensor()
{
    //constructor
    delay_us = 10000;
}

sensor::~sensor()
{
    //destructor
}

int sensor::GetAdc_value(int index) {
    return val[index];
}


int sensor::initSensor(struct LigneCSV* data) {

    register_sig_handler();
    bool tabSensorActivated[MAX_SENSORS_PER_BOARD];

    for (int i = 0; i < MAX_SENSORS_PER_BOARD; i++) {
        tabSensorActivated[i] = getSensorActivated(data, i);
    }

    memset(adc_list, 0, sizeof(adc_list));

    for (int i = 0; i < MAX_ADC; i++) { //0 taille tab de benoit
        if (tabSensorActivated[i])
            adc_list[adc] = 1;
        else
            adc_list[adc] = 0;
    }


    readChannels(delay_us, adc_list);

}


void sensor::readChannels(int delay_us, int *list)
{
    int count, ret, i, update, update_reset;

    ret = 0;
    memset(fd, 0, sizeof(fd));
    memset(val, 0, sizeof(val));

    for (i = 0; i < MAX_ADC; i++) {
        if (list[i]) {
            fd[i] = openAdc(i);

        }
    }
    
    for (i = 0; i < MAX_ADC; i++) {
        if (!list[i])
            continue;

        val[i] = readAdc(fd[i]);

        // reset for next read
        lseek(fd[i], 0, SEEK_SET);

        if (val[i] == ADC_READ_ERROR)
            break;
    }
    
}

void sensor::closeAdc()
{
    for (i = 0; i < MAX_ADC; i++) {
        if (fd[i] > 0)
            close(fd[i]);

    }
}

int sensor::readAdc(int fd)
{
    char buff[8];

    int val = ADC_READ_ERROR;

    memset(buff, 0, sizeof(buff));

    if (read(fd, buff, 8) < 0)
        perror("read()");
    else
        val = atoi(buff);

    return val;
}

int sensor::openAdc(int adc)
{
    char path[128];
    
    sprintf(path, "%sin_voltage%d_raw", iiosyspath, adc);
    

    int fd = open(path, O_RDONLY);


    if (fd < 0) {
        perror("open()");
        printf("%s\n", path);
    }


    return fd;
}

void sensor::register_sig_handler()
{
    struct sigaction sia;

    memset(sia,0, sizeof(sia));
    sia.sa_handler = sigint_handler;

    
    if (sigaction(SIGINT, &sia, NULL) < 0) {
        perror("sigaction(SIGINT)");
        exit(1);
    }
    
}

void sensor::sigint_handler(int sig)
{
    abort_read = 1;
}