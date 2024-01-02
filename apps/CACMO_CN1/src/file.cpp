#include "file.h"

file::file()
{
    //constructor
}

file::~file()
{
    //destructor
}

bool file::initFile()
{
    return true;
}

bool file::testWriteFile()
{
    return true;
}

void file::writeTelem(int statusCode) 
{
    eventlog_printMessage(kEventlogLevelFatal,
        kEventlogCategoryControl,
        "obdcreate_initObd() failed with \"%s\" (0x%04x)\n",
        debugstr_getRetValStr(ret),
        ret);
}

void file::writeError() 
{

}

bool file::closeFile()
{
    return true;
}