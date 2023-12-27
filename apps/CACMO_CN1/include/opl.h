#ifndef OPL_H
#define OPL_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#include "xapOpl.h"
#include "eventOpl.h"
#include "configOpl.h"

#include <oplk/oplk.h>
#include <oplk/debugstr.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#include <system/system.h>
#include <obdcreate/obdcreate.h>

#if (TARGET_SYSTEM == _WIN32_)
#include <getopt/getopt.h>
#else
#include <unistd.h>
#endif

#include <console/console.h>
#include <eventlog/eventlog.h>
#include <netselect/netselect.h>

#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct
{
    UINT32          nodeId;
    tEventlogFormat logFormat;
    UINT32          logLevel;
    UINT32          logCategory;
    char            devName[128];
} tOptions;

tOplkError  initPowerlink(UINT32 cycleLen_p,
                          const char* devName_p,
                          const UINT8* macAddr_p,
                          UINT32 nodeId_p);
void        initOplThread(void);
tOplkError  initProcessImage(void);
void        shutdownOplImage(void);
void        shutdownPowerlink(void);
void        setupInputs(void);
tOplkError  processSync(void);
tOplkError  linkPDO_in(UINT varEntries, tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex);
tOplkError  linkPDO_out(UINT varEntries, tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex);
tOplkError  initApp(void);
bool        initOPL();
bool        ExtinctOPL();

#ifdef __cplusplus
}
#endif

class opl
{
    public:
        opl();
        ~opl();

        void        sendTelem();
        void        sendError();

        void        setValues_In_CN(int32_t values_In_g[]);
        int32_t*    getValues_In_CN(void);
        void        setValues_Out_CN(int32_t values_Out_g[]);
        int32_t*    getValues_Out_CN(void);
        void        setActivated_In_CN(int32_t activated_In_MN_g[]);
        void        setActivated_Out_CN(int32_t activated_Out_MN_g[]);
        bool        demandeExtinctOPL();

    protected:

    private:

};

#endif // OPL_H