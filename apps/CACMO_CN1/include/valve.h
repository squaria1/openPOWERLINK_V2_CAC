#include "csv.h"
#include "configOpl.h"
#include <stdio.h>
#include <gpio.h>
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
#include <gpiod.hpp>
#include <internal.h>



#ifndef VALVES_H
#define VALVES_H
#define MAX_VALVES 12

#ifdef __cplusplus
extern "C"
{
#endif

	int val[MAX_VALVES];
	int16_t* getValvesValue(int16_t* benoitlist);

#ifdef __cplusplus
}
#endif
struct gpiod_chip {
	char* path;
	int offset;
};

struct gpiod_line {
	int line;
};

class valves
{
public:
	valves();
	virtual ~valves();
	bool getLineValue(char chip_path, int line_offset);
	bool initValves(char* chip_path, int offset, int16_t* valveslist);

protected:

private:
	int offset = 0; //port csv
	struct gpiod_hip* chip;
	char chip_path;
};

#endif // VALVES_H