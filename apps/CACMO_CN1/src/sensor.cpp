#include "sensor.h"


sensor::sensor()
{
    //constructor
    delay_us = 10000;
}

sensor::~sensor()
{
    //destructor
}

int getAdc_value(int index) {
    return valSensors[index];
}


int sensor::initSensor() {
    bool tabSensorActivated[MAX_SENSORS];

    for (int i = 0; i < MAX_SENSORS; i++) {
        tabSensorActivated[i] = getActivation(i);
    }

    memset(adc_list, 0, sizeof(adc_list));

    for (int i = 0; i < MAX_SENSORS; i++) { //0 taille tab de benoit
        if (tabSensorActivated[i])
            adc_list[adc] = 1;
        else
            adc_list[adc] = 0;
    }


    readChannels(delay_us, adc_list);

    return 0;
}


void sensor::readChannels(int delay_us, int *list)
{
    int ret, i;

    ret = 0;
    memset(fd, 0, sizeof(fd));
    memset(valSensors, 0, sizeof(valSensors));

    for (i = 0; i < MAX_SENSORS; i++) {
        if (list[i]) {
            fd[i] = openAdc(i);

        }
    }
    
    for (i = 0; i < MAX_SENSORS; i++) {
        if (!list[i])
            continue;

        valSensors[i] = readAdc(fd[i]);

        // reset for next read
        _lseek(fd[i], 0, SEEK_SET);

        if (valSensors[i] == ADC_READ_ERROR)
            break;
    }
    
}

void sensor::closeAdc()
{
    for (i = 0; i < MAX_SENSORS; i++) {
        if (fd[i] > 0)
            _close(fd[i]);

    }
}

int sensor::readAdc(int fd)
{
    char buff[8];
    buff[7] = 0;

    int val = ADC_READ_ERROR;

    memset(buff, 0, sizeof(buff));

    if (_read(fd, buff, 8) < 0)
        perror("read()");
    else
        val = atoi(buff);

    return val;
}

int sensor::openAdc(int adc)
{
    char path[128];
    
    sprintf(path, "%sin_voltage%d_raw", IIOSYSPATH, adc);
    

    int fd = _open(path, O_RDONLY);


    if (fd < 0) {
        perror("open()");
        printf("%s\n", path);
    }


    return fd;
}