#include "sensor.h"

static int opt, delay_us, adc, i;
static uint8_t tabSensorActivated[MAX_SENSORS];
static int fd[MAX_SENSORS];
static int abort_read;


sensor::sensor()
{
    //constructor
    delay_us = 10000;
    memset(tabSensorActivated, 0, sizeof(tabSensorActivated));
}

sensor::~sensor()
{
    //destructor
}


int16_t sensor::initSensor() {
    for (int i = 0; i < MAX_SENSORS; i++) {
        tabSensorActivated[i] = getActivation(i + nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 + 2);
    }

    readChannels();

    return 0;
}

int16_t sensor::extinctSensor() {
    closeAdc();

    return 0;
}

int16_t getAdc_value(int index) {
    return valSensors[index];
}

void readChannels()
{
    int ret, i;
    ret = 0;
    memset(fd, 0, sizeof(fd));
    memset(valSensors, 0, sizeof(valSensors));

    for (i = 0; i < MAX_SENSORS; i++) {
        if (tabSensorActivated[i]) {
            fd[i] = openAdc(i);
        }
    }
    
    for (i = 0; i < MAX_SENSORS; i++) {
        if (!tabSensorActivated[i])
            continue;

        valSensors[i] = readAdc(fd[i]);

        // reset for next read
        lseek(fd[i], 0, SEEK_SET);

        if (valSensors[i] == ADC_READ_ERROR)
            break;
    }

    closeAdc();
}

void closeAdc()
{
    for (i = 0; i < MAX_SENSORS; i++) {
        if (fd[i] > 0)
            close(fd[i]);
    }
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
    }


    return fd;
}