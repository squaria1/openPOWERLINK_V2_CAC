#ifndef OPL_H
#define OPL_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "xapOpl.h"
#include "eventOpl.h"
#include "configDefine.h"
#include "nodeId.h"
#include "statusErrorDefine.h"
#include "csv.h"
#include "sensor.h"
#include "valve.h"

#include <oplk/oplk.h>
#include <oplk/debugstr.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

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
#include <string>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        UINT32          nodeId;
        tEventlogFormat logFormat;
        UINT32          logLevel;
        UINT32          logCategory;
        char            devName[128];
    } tOptions;

    statusErrDef        initPowerlink(UINT32 cycleLen_p,
                                      const char* devName_p,
                                      const UINT8* macAddr_p,
                                      UINT32 nodeId_p);
    statusErrDef        initOplThread(void);
    statusErrDef        initProcessImage(void);
    statusErrDef        shutdownOplImage(void);
    statusErrDef        checkStateOpl(void);
    void                shutdownPowerlink(void);
    void                setupInputs(void);
    tOplkError          processSync(void);
    tOplkError          linkPDO_in(tObdSize obdSize, 
                                   const UINT16 arrayIndex, 
                                   UINT16 index, 
                                   UINT8 subIndex);
    tOplkError          linkPDO_out(tObdSize obdSize, 
                                    const UINT16 arrayIndex, 
                                    UINT16 index, 
                                    UINT8 subIndex);
    statusErrDef        initOPL();
    statusErrDef        extinctOPL();
    //void              setValues_In_CN(int ligne);
    int16_t             getValues_In_CN(int ligne);
    void                setValues_Out_CN();
    int16_t             getValues_Out_CN(int ligne);
    void                setActivated_Out_CN();
    void                affValeursIn();
    void                affValeursProcessIn();
    void                affValeursOut();

    //------------------------------------------------------------------------------
    // global vars
    //------------------------------------------------------------------------------
    extern int16_t              EG;
    extern int16_t              EC;
    
    /* Xap segmentation variables */
    extern const uint16_t       nbValuesCN_Out;
    extern const uint16_t       nbValuesCN_In;
    extern const uint16_t       nbValuesCN_Out_ByCN;
    extern const uint16_t       nbValuesCN_In_ByCN;

    //------------------------------------------------------------------------------
    // local function prototypes
    //------------------------------------------------------------------------------

    int16_t                  getEG();
    statusErrDef             isEGchanged();
    void                     setEC1(int16_t EC1);

#ifdef __cplusplus
}
#endif

class opl
{
    public:
        opl();
        ~opl();

        void            sendTelem(int16_t statusCode);
        void            sendError(int16_t errorCode);
        statusErrDef    demandeExtinctOPL();

    protected:

    private:

};

#endif // OPL_H