#ifndef SENSOR_H
#define SENSOR_H

#include "csv.h"
#include "configDefine.h"
#include "statusErrorDefine.h"
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

#ifdef __cplusplus
extern "C"
{
#endif

static int16_t valSensors[MAX_SENSORS]; 
statusErrDef readChannels();
statusErrDef closeAdc();
int readAdc(int fd);
int openAdc(int adc);
int16_t getAdc_value(int index);

#ifdef __cplusplus
}
#endif

/**
 * \brief sensor module class.
 * 
 */
class sensor
{
    public:
        sensor();
        virtual ~sensor();
        statusErrDef initSensor();
        statusErrDef extinctSensor();

    protected:

    private:

};

#endif // SENSOR_H
