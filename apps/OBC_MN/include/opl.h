/**
 * \file opl.h
 * \brief header file of the OpenPOWERLINK module that communicates with the master board using OpenPOWERLINK
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all functions definitions related to communicating with the master board
 * using OpenPOWERLINK, transfer sensors, valves values and recieve orders from the master.
 */

#ifndef OPL_H
#define OPL_H

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "xapOpl.h"
#include "eventOpl.h"
#include "configDefine.h"
#include "statusErrorDefine.h"
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

     /**
     * \struct tOptions
     * \brief the OpenPOWERLINK initialization parameters
     * 
     */
    typedef struct
    {
        char            cdcFile[256];       /**< The OpenPOWERLINK binary file name */
        char            fwInfoFile[256];    /**< The OpenPOWERLINK info file name */
        char*           pLogFile;           /**< the custom log file output of OpenPOWERLINK */
        tEventlogFormat logFormat;          /**< the OpenPOWERLINK log format (see OpenPOWERLINK doc) */
        UINT32          logLevel;           /**< the OpenPOWERLINK log level (see OpenPOWERLINK doc) */
        UINT32          logCategory;        /**< the OpenPOWERLINK log category (see OpenPOWERLINK doc) */
        char            devName[200];       /**< the network card device name from the DEVNAME define in 'configDefine.h' */
    } tOptions;

    /**
     * \struct tDemoNodeInfo
     * \brief the OpenPOWERLINK node information structure
     * 
     */
    typedef struct
    {
        tNmtState       nodeState[254];     /**< The current OpenPOWERLINK node state */
    } tDemoNodeInfo;

    statusErrDef            initPowerlink(UINT32 cycleLen_p,
                                            const char* cdcFileName_p,
                                            const char* devName_p,
                                            const UINT8* macAddr_p);
    statusErrDef            initOplThread(void);
    statusErrDef            initProcessImage(void);
    statusErrDef            shutdownOplImage(void);
    statusErrDef            checkStateOpl();
    void                    shutdownPowerlink(void);
    tOplkError              processSync(void);
    statusErrDef            initApp(void);
    statusErrDef            initOPL();
    statusErrDef            extinctOPL();
    int16_t*                getValues_In_MN(void);
    void                    setValues_Out_MN(int ligne, int16_t valeur);
    void                    setActivated_In_MN();
    void                    affValeursIn();
    void                    affValeursOut();
    void                    affValeursInProcess();
    void                    affValeursOutProcess();
    void                    changeEG(); 
    void                    setEGToManualMode();
    void                    manualActivation(int nodeId, int valve);

    //------------------------------------------------------------------------------
    // global vars
    //------------------------------------------------------------------------------
    extern const uint16_t       nbValuesCN_Out;
    extern const uint16_t       nbValuesCN_In;

    void        setEG(int16_t EG);

#ifdef __cplusplus
}
#endif

/**
 * \brief OpenPOWERLINK module class.
 * 
 */
class opl
{
    public:
        opl();
        ~opl();

    protected:

    private:

};

#endif // OPL_H