#ifndef OPL_H
#define OPL_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "xapOpl.h"
#include "eventOpl.h"
#include "configOpl.h"

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

class opl
{
    public:
        opl();
        ~opl();

        bool        initOPL();
        bool        testOPL();
        bool        ExtinctOPL();
        void        sendTelem();
        void        sendError();
        tOplkError  initPowerlink(UINT32 cycleLen_p,
                                       const char* cdcFileName_p,
                                       const char* devName_p,
                                       const UINT8* macAddr_p);
        void        initOplThread(void);
        tOplkError  initProcessImage(void);
        void        shutdownOplImage(void);
        void        shutdownPowerlink(void);
        tOplkError  initApp(void);
        void        setValues_In_MN(int32_t values_In_g[]);
        int32_t*    getValues_In_MN(void);
        void        setValues_Out_MN(int32_t values_Out_g[]);
        int32_t*    getValues_Out_MN(void);
        void        setActivated_In_MN(int32_t activated_In_MN_g[]);
        void        setActivated_Out_MN(int32_t activated_Out_MN_g[]);


        typedef struct
        {
            UINT32                leds;
            UINT32                ledsOld;
            UINT32                input;
            UINT32                inputOld;
            UINT                  period;
            int                   toggle;
        } APP_NODE_VAR_T;


    protected:

    private:
        //------------------------------------------------------------------------------
        // local types
        //------------------------------------------------------------------------------

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

};

#endif // OPL_H