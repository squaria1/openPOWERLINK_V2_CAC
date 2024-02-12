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
    #include "csv.h"
    #include "sensor.h"

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
    tOplkError  linkPDO_in(tObdSize obdSize, 
                           const UINT16 arrayIndex, 
                           UINT16 index, 
                           UINT8 subIndex);
    tOplkError  linkPDO_out(tObdSize obdSize, 
                            const UINT16 arrayIndex, 
                            UINT16 index, 
                            UINT8 subIndex);
    tOplkError  initApp(struct LigneVannes* dataPhysicalConfigVannes,
                        struct LigneSensors* dataPhysicalConfigSensors);
    bool        initOPL(struct LigneVannes* dataPhysicalConfigVannes,
                        struct LigneSensors* dataPhysicalConfigSensors);
    bool        ExtinctOPL();
    //void        setValues_In_CN(struct LigneCSV* data, int ligne);
    int16_t*    getValues_In_CN(void);
    void        setValues_Out_CN();
    int16_t*    getValues_Out_CN(void);
    //void        setActivated_In_CN(struct LigneCSV* data, int ligne, uint8_t mode);
    void        setActivated_Out_CN(struct LigneCSV* data, 
                                    int ligne);

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
    static bool                activated_Out_CN_l[SIZE_OUT];

    //------------------------------------------------------------------------------
    // global vars
    //------------------------------------------------------------------------------
    static const uint16_t       nbValuesCN_Out = SIZE_OUT / NB_NODES - 1;
    static const uint16_t       nbValuesCN_In = SIZE_IN / NB_NODES - 1;
    static const uint16_t       nbValuesCN_Out_ByCN = (SIZE_OUT / NB_NODES) * (NODEID - 1);
    static const uint16_t       nbValuesCN_In_ByCN = (SIZE_IN / NB_NODES) * (NODEID - 1);
    static uint8_t              mode;

    //------------------------------------------------------------------------------
    // local function prototypes
    //------------------------------------------------------------------------------

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
        bool        demandeExtinctOPL();

    protected:

    private:

};

#endif // OPL_H