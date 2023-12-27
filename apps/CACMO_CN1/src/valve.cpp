#include "valve.h"
#include "opl.h"
#include "file.h"
#include <gpiod.hpp>

opl opl;
file file;

valve::valve()
{
    //constructor
}

valve::~valve()
{
    //destructor
}

bool valve::initValve()
{
    try {
        ::gpiod::chip chip("gpiochip0");
        for (int i = 0; i < MAXGPIO; i++)
        {

        }
    }
    catch (const ::gpiod::error& e) {
        return false;
    }
    return true;
}