#include "csv.h"
#include "configDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h> 
#include <signal.h>
#include <string>

#if (TARGET_SYSTEM == _WIN32_)
#include <getopt/getopt.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#ifndef SENSOR_H
#define SENSOR_H
#define ADC_READ_ERROR -100000

#ifdef __cplusplus
extern "C"
{
#endif

static int valSensors[MAX_SENSORS];
int getAdc_value(int index);

#ifdef __cplusplus
}
#endif

class sensor
{
    public:
        sensor();
        virtual ~sensor();

        std::string Getname() { return name; }
        void Setname(std::string val) { name = val; }
        int Gettemperature() { return temperature; }
        void Settemperature(int val) { temperature = val; }
        unsigned int Getpressure() { return pressure; }
        void Setpressure(unsigned int val) { pressure = val; }

        void readChannels(int delay_us, int* list);
        void closeAdc();
        int readAdc(int fd);
        int openAdc(int adc);
        int initSensor();


        int opt, delay_us, adc, i;
        int adc_list[MAX_SENSORS];
        int fd[MAX_SENSORS];
        int abort_read;

    protected:

    private:
        std::string name;
        int temperature;
        unsigned int pressure;

};

#endif // SENSOR_H
