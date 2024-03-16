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


int16_t sensor::initSensor() {
    int16_t res = 0;
    for (int i = 0; i < MAX_SENSORS; i++) 
    {
        tabSensorActivated[i] = getActivation(i + nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 + 2);
    }

    res = readChannels();
    if (res != 0)
        return res;

    return 0;
}

int16_t sensor::extinctSensor() {
    int16_t res = 0;

    res = closeAdc();

    return res;
}

int16_t getAdc_value(int index) {
    return valSensors[index];
}

int16_t readChannels()
{
    int i;
    int16_t res = 0;
    memset(fd, 0, sizeof(fd));
    memset(valSensors, 0, sizeof(valSensors));

    for (i = 0; i < MAX_SENSORS; i++) {
        if (tabSensorActivated[i]) {
            fd[i] = openAdc(i);
            if (fd[i] == ADC_READ_ERROR)
            {
                res = 0xE401;
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
            res = 0xE402;
            break;
        }
    }

    closeAdc();

    return res;
}

int16_t closeAdc()
{
    int16_t res = 0;

    for (i = 0; i < MAX_SENSORS; i++) {
        if (fd[i] > 0)
        {
            res = close(fd[i]);
            if (res < 0)
            {
                res = 0xE4FF;
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