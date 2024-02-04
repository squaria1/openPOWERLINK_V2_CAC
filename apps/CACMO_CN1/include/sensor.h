#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h> 
#include <signal.h>
#include <string>

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

#ifndef SENSOR_H
#define SENSOR_H
#define ADC_READ_ERROR -100000
#define MAX_ADC 8

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
        void register_sig_handler();
        void sigint_handler(int sig);
        int GetAdc_value(int index);
        int initSensor(struct LigneCSV* data);


        int opt, delay_us, adc, i;
        int adc_list[MAX_ADC];
        int val[MAX_ADC];
        int fd[MAX_ADC];
        int abort_read;

    protected:

    private:
        std::string name;
        int temperature;
        unsigned int pressure;

};

#endif // SENSOR_H
