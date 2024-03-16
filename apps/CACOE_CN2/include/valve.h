#include "csv.h"
#include "configDefine.h"
#include "statusErrorDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
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

class valve
{
public:
	valve();
	virtual ~valve();
	statusErrDef initValve();
	statusErrDef actionnementValvesInit();
	statusErrDef actionnementValve(int valveNum);
	statusErrDef verifDependanceValves();
	statusErrDef isDependanceActive(int ligne);
	int16_t startTimerDependance(int ligne);
	int16_t isTimerExeeded(int ligne);
	statusErrDef extinctValve();

protected:

private:
};

#ifdef __cplusplus
extern "C"
{
#endif
	int16_t setValvesValue();
	statusErrDef getValvesInitValue();
	int16_t getValveValue(int index);
	statusErrDef resetTimers();
#ifdef __cplusplus
}
#endif

#endif // VALVES_H