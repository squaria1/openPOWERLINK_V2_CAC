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
#include <gpiod.hpp>
#include <internal.h>
#endif

#ifndef VALVES_H
#define VALVES_H

#ifdef __cplusplus
extern "C"
{
#endif
	int16_t getValveValue(int index);

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
	bool getLineValue(struct LigneVannes* dataPhysicalConfigVannes, int line);
	bool initValve(struct LigneVannes* dataPhysicalConfigVannes);

protected:

private:
};

#endif // VALVES_H