#ifndef OPL_H
#define OPL_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "xapOpl.h"
#include "eventOpl.h"
#include "configDefine.h"
#include "csv.h"

#include <oplk/oplk.h>
#include <oplk/debugstr.h>

#include <obdpi.h>
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
#include <firmwaremanager/firmwaremanager.h>
#include <netselect/netselect.h>

#include <stdio.h>
#include <limits.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        char            cdcFile[256];
        char            fwInfoFile[256];
        char*           pLogFile;
        tEventlogFormat logFormat;
        UINT32          logLevel;
        UINT32          logCategory;
        char            devName[128];
    } tOptions;

    typedef struct
    {
        tNmtState       nodeState[254];
    } tDemoNodeInfo;

    tOplkError  initPowerlink(UINT32 cycleLen_p,
                                const char* cdcFileName_p,
                                const char* devName_p,
                                const UINT8* macAddr_p);
    void        initOplThread(void);
    tOplkError  initProcessImage(void);
    void        shutdownOplImage(void);
    void        shutdownPowerlink(void);
    tOplkError  processSync(void);
    tOplkError  initApp(void);
    int16_t     initOPL();
    int16_t     extinctOPL();
    void        setValues_In_MN(int ligne, int16_t valeur);
    int16_t*    getValues_In_MN(void);
    void        setValues_Out_MN(int ligne, int16_t valeur);
    int16_t*    getValues_Out_MN(void);
    void        setActivated_In_MN();
    void        affValeursIn();
    void        affValeursOut();
    void        affValeursInProcess();
    void        affValeursOutProcess();


    //------------------------------------------------------------------------------
    // local vars
    //------------------------------------------------------------------------------
    static const UINT8              aMacAddr_l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    static BOOL                     fGsOff_l;

    static UINT                     cnt_l;
    static PI_IN*                   pProcessImageIn_l;
    static PI_OUT*                  pProcessImageOut_l;

    static int16_t                  values_In_MN_l[SIZE_OUT + 2*NB_NODES];
    static int16_t                  values_Out_MN_l[SIZE_IN];
    static bool                     activated_In_MN_l[SIZE_OUT+1];
    //static bool                   activated_Out_MN_l[SIZE_OUT];

    //------------------------------------------------------------------------------
    // global vars
    //------------------------------------------------------------------------------
    extern uint8_t              mode;
    extern const uint16_t       nbValuesCN_In;


    int16_t     getTest();

    int16_t     getEC1();
    void        setEG(int16_t EG);

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
        int16_t     demandeExtinctOPL();

    protected:

    private:

};

#endif // OPL_H