#include "include\sensors\sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <errno.h>

#define ADC_READ_ERROR -100000

#define MAX_ADC 8

int opt, delay_us, adc, i;
int adc_list[MAX_ADC];

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


int sensor::init_sensor(){

    register_sig_handler();

    if (optind == argc) {
        printf("List of ADCs is required\n");
        usage(argv[0]);
    }

    memset(adc_list, 0, sizeof(adc_list));

    for (i = 0; i < 6; i++) {
        adc = atoi(argv[i]);

        if (adc < 0 || adc > 5) {
            printf("adc %d is out of range\n", adc);
            usage(argv[0]);
        }

        if (adc_list[adc]) {
            printf("adc %d listed more then once\n", adc + 2);
            usage(argv[0]);
        }

        adc_list[adc] = 1;
    }


    int count = read_Channels(delay_us, adc_list);

}


void sensor::read_Channels(int delay_us, int *list)
{
    int ret, i, update, update_reset;
    int val[MAX_ADC];
    int fd[MAX_ADC];

    ret = 0;
    memset(fd, 0, sizeof(fd));
    memset(val, 0, sizeof(val));

    for (i = 0; i < MAX_ADC; i++) {
        if (list[i]) {
            fd[i] = open_adc(i);

            if (fd[i] < 0)
                goto loop_done;
        }
    }
    
    for (i = 0; i < MAX_ADC; i++) {
        if (!list[i])
            continue;

        val[i] = read_adc(fd[i]);

        // reset for next read
        lseek(fd[i], 0, SEEK_SET);

        if (val[i] == ADC_READ_ERROR)
            break;
    }

    if (delay_us)
        usleep(delay_us);
    

    for (i = 0; i < MAX_ADC; i++) {
        if (fd[i] > 0)
            close(fd[i]);
    }

    return count;
}

int sensor::read_adc(int fd)
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

int sensor::open_adc(int adc)
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

    bzero(&sia, sizeof sia);
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