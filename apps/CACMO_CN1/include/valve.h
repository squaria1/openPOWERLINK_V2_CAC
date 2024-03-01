#include "csv.h"
#include "configDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h> 
#include <signal.h>
#include <sensor.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#if (TARGET_SYSTEM == _WIN32_)
#else
#include <gpiod.h>
#include <unistd.h>
#endif

#ifndef VALVES_H
#define VALVES_H

struct gpiod_chip* chip;
struct gpiod_line* line;

unsigned int offsets[MAX_VALVES];
int values[MAX_VALVES];
int err;

class valve
{
public:
	valve();
	virtual ~valve();
	bool initValve();
	bool extinctValve();

protected:

private:
};

#ifdef __cplusplus
extern "C"
{
#endif
	int16_t getValveValue(int index);

#ifdef __cplusplus
}
#endif

#endif // VALVES_H