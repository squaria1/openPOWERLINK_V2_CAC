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

extern "C"
{
    int getAdc_value(int index) {
        return val[index];
    }
}


int sensor::initSensor(struct LigneCSV* data) {
    bool tabSensorActivated[MAX_SENSORS];

    for (int i = 0; i < MAX_SENSORS; i++) {
        tabSensorActivated[i] = getActivation(data, i);
    }

    memset(adc_list, 0, sizeof(adc_list));

    for (int i = 0; i < MAX_SENSORS; i++) { //0 taille tab de benoit
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

    for (i = 0; i < MAX_SENSORS; i++) {
        if (list[i]) {
            fd[i] = openAdc(i);

        }
    }
    
    for (i = 0; i < MAX_SENSORS; i++) {
        if (!list[i])
            continue;

        val[i] = readAdc(fd[i]);

        // reset for next read
        _lseek(fd[i], 0, SEEK_SET);

        if (val[i] == ADC_READ_ERROR)
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
    
    sprintf(path, "%sin_voltage%d_raw", iiosyspath, adc);
    

    int fd = _open(path, O_RDONLY);


    if (fd < 0) {
        perror("open()");
        printf("%s\n", path);
    }


    return fd;
}