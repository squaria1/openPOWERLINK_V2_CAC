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
void        printOutputs(void);
void        printInputs(void);
void        increaseInputs(void);
void        decreaseInputs(void);
tOplkError  processSync(void);
tOplkError  linkPDO_in(UINT varEntries, tObdSize obdSize, const UINT16 arrayIndex, UINT16 index, UINT8 subIndex);
tOplkError  linkPDO_out(UINT varEntries, tObdSize obdSize, const UINT16 arrayIndex, UINT16 index, UINT8 subIndex);
tOplkError  initApp(void);
bool        initOPL();
bool        ExtinctOPL();

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static const UINT8       aMacAddr_l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static BOOL              fGsOff_l;

/* process image */
static PI_IN*            pProcessImageIn_l;
static const PI_OUT*     pProcessImageOut_l;


/* application variables */

static int16_t             values_In_CN_l[SIZE_IN];
static int16_t             values_Out_CN_l[SIZE_OUT];
static bool              activated_In_CN_l[SIZE_IN];
static bool              activated_Out_CN_l[SIZE_OUT];

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

static const UINT8       nbValuesCN_Out = SIZE_OUT / NB_NODES;
static const UINT8       nbValuesCN_In = SIZE_IN / NB_NODES;
static const UINT8       nbValuesCN_Out_ByCN = SIZE_OUT / NB_NODES * (NODEID - 1) + 1;
static const UINT8       nbValuesCN_In_ByCN = SIZE_IN / NB_NODES * (NODEID - 1) + 1;


int16_t                  getEG();
void                     setEC1(int16_t EC1);

#ifdef __cplusplus
}
#endif

class opl
{
    public:
        opl();
        ~opl();

        void        sendTelem(int16_t statusCode);
        void        sendError(int16_t errorCode);

        void        setValues_In_CN(int16_t values_In_g[]);
        int16_t*      getValues_In_CN(void);
        void        setValues_Out_CN(int16_t values_Out_g[]);
        int16_t*      getValues_Out_CN(void);
        void        setActivated_In_CN(bool activated_In_MN_g[]);
        void        setActivated_Out_CN(bool activated_Out_MN_g[]);
        bool        demandeExtinctOPL();

    protected:

    private:

};

#endif // OPL_H