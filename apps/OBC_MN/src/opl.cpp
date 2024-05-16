/**
 * \file opl.cpp
 * \brief Module to communicate with the master board using OpenPOWERLINK
 * \author Mael Parot
 * \version 1.1
 * \date 11/04/2024
 *
 * Contains all functions related to communicating with the master board
 * using OpenPOWERLINK, transfer sensors, valves values and recieve orders from the master.
 */

#include "opl.h"

Mode                 mode = automatic;
int                  cmptEG;
const uint16_t       nbValuesCN_Out = SIZE_OUT / NB_NODES - 2;
const uint16_t       nbValuesCN_In = SIZE_IN / NB_NODES - 1;

opl::opl()
{

}

opl::~opl()
{
    //destructor
}


int16_t* getValues_In_MN()
{
    return values_In_MN_l;
}

/**
 * \brief function setter of the values
 * coming out of the MN namely, the general state order 
 * and valve values in manual mode to the CNs.
 * 
 * \param ligne the location in the PI_IN structure
 * \param valeur the manual valve value to set
 */
void setValues_Out_MN(int ligne, int16_t valeur)
{
    values_Out_MN_l[ligne] = valeur;
}

/**
 * \brief function setter to fill the activation array
 * from the CSV activation file "activation.csv"
 * 
 */
void setActivated_In_MN()
{
    for (int i = 0; i < SIZE_OUT + 1; i++)
    {
        activated_In_MN_l[i] = getActivation(i);
    }
}

/**
 * \brief function setter of the current general state (EG)
 * 
 * \param EG the current general state (EG)
 */
void setEG(int16_t EG)
{
    for (int i = 0; i < SIZE_IN; i++)
    {
        if (i % (nbValuesCN_In + 1) == 0)
            values_Out_MN_l[i] = EG;
    }
}

/**
 * \brief function to display in the console
 * the values coming from the CNs
 * 
 */
void affValeursIn()
{
    printf("\n-------------IN MN--------------\n");
    for (int i = 0; i < SIZE_OUT; i++)
    {
        printf("values_In_MN_l[%d]=%d\n", i, values_In_MN_l[i]);
    }
    printf("\n--------------------------------\n");
}

/**
 * \brief function to display in the console
 * the raw values coming from the CNs
 * 
 */
void affValeursInProcess()
{
    printf("\n-------------IN PROCESS MN--------------\n");
    for (int i = 0; i < SIZE_OUT; i++)
    {
        printf("pProcessImageOut_l->out_MN_array[%d]=%d\n", i, pProcessImageOut_l->out_MN_array[i]);
    }
    printf("\n--------------------------------\n");
}

/**
 * \brief function to display in the console
 * the raw values coming out of the MN
 * 
 */
void affValeursOutProcess()
{
    printf("\n-------------OUT PROCESS MN--------------\n");
    for (int i = 0; i < SIZE_IN; i++)
    {
        printf("pProcessImageIn_l->in_MN_array[%d]=%d\n", i, pProcessImageIn_l->in_MN_array[i]);
    }
    printf("\n--------------------------------\n");
}

/**
 * \brief function to display in the console
 * the values coming out of the MN
 * 
 */
void affValeursOut()
{
    printf("\n------------OUT MN--------------\n");
    for (int i = 0; i < SIZE_IN; i++)
    {
        printf("values_Out_MN_l[%d]=%d\n", i, values_Out_MN_l[i]);
    }
    printf("\n--------------------------------\n");
}

/**
 * \brief function to cycle through some general states to test
 * the MN and CNs programs for the early versions.
 * 
 */
void changeEG()
{
    int sel = cmptEG % 5;
    switch (sel)
    {
    case 0:
        setEG(1);
        break;
    case 1:
        setEG(2);
        break;
    case 2:
        setEG(3);
        break;
    case 3:
        setEG(4);
        break;
    case 4:
        setEG(5);
        break;
    }
    printf("\n\nEG = %d\n\n", sel+1);
    cmptEG++;
}

/**
 * \brief function to switch between 
 * automatic and manual modes.
 * 
 */
void setEGToManualMode()
{
    if (mode == automatic)
    {
        mode = manual;
        setEG(infoStateToManualMode);
    }
    else if (mode == manual)
    {
        mode = automatic;
        setEG(1);
    }

    printf("\n\nmode : %d\n\n", mode);
}

/**
 * \brief function to set to 0 or 1 a valve value of a CN
 * when mode is set to manual mode.
 * 
 * \param nodeId the id of the CN
 * \param valve the valve number (from 0 to 11)
 */
void manualActivation(int nodeId, int valve)
{
    printf("\nOld value for valve[%d] : %d, nodeId : %d\n", valve + (nbValuesCN_In + 1) * (nodeId - 1),
        values_Out_MN_l[valve + (nbValuesCN_In + 1) * (nodeId - 1)], nodeId);

    values_Out_MN_l[valve + (nbValuesCN_In + 1) * (nodeId - 1)] = !values_Out_MN_l[valve + (nbValuesCN_In + 1) * (nodeId - 1)];

    printf("New value for valve[%d] : %d, nodeId : %d\n", valve + (nbValuesCN_In + 1) * (nodeId - 1),
        values_Out_MN_l[valve + (nbValuesCN_In + 1) * (nodeId - 1)], nodeId);
}


//------------------------------------------------------------------------------
// OpenPOWERLINK stack and module functions
//------------------------------------------------------------------------------

/**
 * \brief function to initialize the CN OpenPOWERLINK module
 * 
 * \return statusErrDef that values errOPLSystemInit
 * when OpenPOWERLINK fails to set the correct configuration
 * for the current operating system.
 * or errInitFirmwareManager when the firmware manager to
 * check dependances of the operating system fails to initialize
 * or errSelNetInterface when the selection
 * of the network interface fails when
 * in a Windows machine can be caused by the absence of WinPcap.
 * or errInitObjDictionary when the object dictionary header file (objdict.h)
 * has incorrect values or syntax.
 * or errOplkInit when the OpenPOWERLINK stack fails maybe because the stack is
 * not found by the application, check the CN .lib files.
 * or errOplkCreate when the OpenPOWERLINK stack fails to create a new instance
 * or errOplkAllocProcessImage when the allocation of the input
 * and/or output structure doesn't exist in the objdict.h file
 * or errSendNMTResetCommand when the OpenPOWERLINK reset command fails
 * or noError when the function exits successfully. 
 */
statusErrDef initOPL()
{
    statusErrDef    res = noError;
    tOptions        opts;
    tEventConfig    eventConfig;
    tFirmwareRet    fwRet;

    strncpy(opts.cdcFile, CDCFILE, 256);
    strncpy(opts.fwInfoFile, "fw.info", 256);
    strncpy(opts.devName, DEVNAME, 128);
    opts.pLogFile = NULL;
    opts.logFormat = kEventlogFormatReadable;
    opts.logCategory = 0xffffffff;
    opts.logLevel = 0xffffffff;

    if (system_init() != 0)
    {
        fprintf(stderr, "Error initializing system!");
        return errOPLSystemInit;
    }

    fwRet = firmwaremanager_init(opts.fwInfoFile);
    if (fwRet != kFwReturnOk)
    {
        fprintf(stderr, "Error initializing firmware manager!");
        return errInitFirmwareManager;
    }

    eventlog_init(opts.logFormat,
        opts.logLevel,
        opts.logCategory,
        (tEventlogOutputCb)console_printlogadd);

    memset(&eventConfig, 0, sizeof(tEventConfig));

    eventConfig.pfGsOff = &fGsOff_l;
    eventConfig.pfnFirmwareManagerCallback = firmwaremanager_processEvent;

    initEvents(&eventConfig);

    printf("----------------------------------------------------\n");
    printf("openPOWERLINK OBC MN application\n");
    printf("Using openPOWERLINK stack: %s\n", oplk_getVersionString());
    printf("----------------------------------------------------\n");

    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "demo_mn_console: Stack version:%s Stack configuration:0x%08X",
        oplk_getVersionString(),
        oplk_getStackConfiguration());

    setActivated_In_MN();

    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "Using CDC file: %s",
        opts.cdcFile);

    res = initPowerlink(CYCLE_LEN,
        opts.cdcFile,
        opts.devName,
        aMacAddr_l);
    if (res != noError)
        return res;

    res = initApp();
    if (res != noError)
        return res;

    res = initOplThread();
    if (res != noError)
        return res;

    return noError;
}

/**
\brief  Initialize the synchronous data application

The function initializes the synchronous data application

\return statusErrDef that values errOplkAllocProcessImage
when the allocation of the input and/or output structure 
doesn't exist in the objdict.h file
or errSetupProcessImage when the input and/or output structure
are not the same with the mnobd.cdc file
or noError when the function exits successfully.

\ingroup module_demo_mn_console
*/
statusErrDef initApp(void)
{
    statusErrDef res = noError;
    int         i;

    cnt_l = 0;
    i = 0;

    for (i = 0; i < SIZE_OUT; i++)
    {
        values_In_MN_l[i] = 0;
    }

    for (i = 0; i < SIZE_IN; i++)
    {
        values_Out_MN_l[i] = 0;
    }

    mode = automatic;
    setEG(1);

    memset(&pProcessImageOut_l, 0, sizeof(pProcessImageOut_l));
    res = initProcessImage();

    return res;
}


/**
\brief  Initialize the openPOWERLINK stack

The function initializes the openPOWERLINK stack.

\param[in]      cycleLen_p          Length of POWERLINK cycle.
\param[in]      cdcFileName_p       Name of the CDC file.
\param[in]      devName_p           Device name string.
\param[in]      macAddr_p           MAC address to use for POWERLINK interface.

\return statusErrDef that values errSelNetInterface
when the selection of the network interface fails when
in a Windows machine can be caused by the absence of WinPcap.
or errInitObjDictionary when the object dictionary header file (objdict.h)
has incorrect values or syntax.
or errOplkInit when the OpenPOWERLINK stack fails maybe because the stack is
not found by the application, check the CN .lib files.
or errOplkCreate when the OpenPOWERLINK stack fails to create a new instance
or errOplkSetCDCFileName when the mnobd.cdc file is not found or is incorrect
or noError when the function exits successfully. 
*/
statusErrDef initPowerlink(UINT32 cycleLen_p,
                              const char* cdcFileName_p,
                              const char* devName_p,
                              const UINT8* macAddr_p)
{
    tOplkError          ret = kErrorOk;
    tOplkApiInitParam   initParam;
    static char         devName[128];

    printf("Initializing openPOWERLINK stack...\n");
    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryControl,
        "Initializing openPOWERLINK stack");

    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "Select the network interface");

    #if (TARGET_SYSTEM == _WIN32_)
        if (netselect_selectNetworkInterface(devName, sizeof(devName)) < 0)
            return errSelNetInterface;
    #else
        strncpy(devName, devName_p, 128);
    #endif

    printf("DEVNAME : %s \n", devName);
    memset(&initParam, 0, sizeof(initParam));
    initParam.sizeOfInitParam = sizeof(initParam);

    // pass selected device name to Edrv
    initParam.hwParam.pDevName = devName;
    initParam.nodeId = NODEID;
    initParam.ipAddress = (0xFFFFFF00 & IP_ADDR) | initParam.nodeId;

    /* write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address */
    memcpy(initParam.aMacAddress, macAddr_p, sizeof(initParam.aMacAddress));

    initParam.fAsyncOnly = FALSE;
    initParam.featureFlags = UINT_MAX;
    initParam.cycleLen = cycleLen_p;       // required for error detection
    initParam.isochrTxMaxPayload = 256;              // const
    initParam.isochrRxMaxPayload = 1490;             // const
    initParam.presMaxLatency = 50000;            // const; only required for IdentRes
    initParam.preqActPayloadLimit = 36;               // required for initialisation (+28 bytes)
    initParam.presActPayloadLimit = 36;               // required for initialisation of Pres frame (+28 bytes)
    initParam.asndMaxLatency = 150000;           // const; only required for IdentRes
    initParam.multiplCylceCnt = 0;                // required for error detection
    initParam.asyncMtu = 1500;             // required to set up max frame size
    initParam.prescaler = 2;                // required for sync
    initParam.lossOfFrameTolerance = 500000;
    initParam.asyncSlotTimeout = 3000000;
    initParam.waitSocPreq = 1000;
    initParam.deviceType = UINT_MAX;         // NMT_DeviceType_U32
    initParam.vendorId = UINT_MAX;         // NMT_IdentityObject_REC.VendorId_U32
    initParam.productCode = UINT_MAX;         // NMT_IdentityObject_REC.ProductCode_U32
    initParam.revisionNumber = UINT_MAX;         // NMT_IdentityObject_REC.RevisionNo_U32
    initParam.serialNumber = UINT_MAX;         // NMT_IdentityObject_REC.SerialNo_U32

    initParam.subnetMask = SUBNET_MASK;
    initParam.defaultGateway = DEFAULT_GATEWAY;
    sprintf((char*)initParam.sHostname, "%02x-%08x", initParam.nodeId, initParam.vendorId);
    initParam.syncNodeId = C_ADR_SYNC_ON_SOA;
    initParam.fSyncOnPrcNode = FALSE;

    // set callback functions
    initParam.pfnCbEvent = processEvents;

    initParam.pfnCbSync = processSync;

    // Initialize object dictionary
    ret = obdcreate_initObd(&initParam.obdInitParam);
    if (ret != kErrorOk)
    {
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "obdcreate_initObd() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errInitObjDictionary;
    }

    // initialize POWERLINK stack
    ret = oplk_initialize();
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_initialize() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "oplk_init() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errOplkInit;
    }

    ret = oplk_create(&initParam);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_create() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errOplkCreate;
    }

    ret = oplk_setCdcFilename(cdcFileName_p);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_setCdcFilename() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "oplk_setCdcFilename() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errOplkSetCDCFileName;
    }

    return noError;
}

/**
 * \brief function to check if the OpenPOWERLINK stack
 * crashed or a termination signal has been received from
 * the OS app manager.
 * 
 * \return statusErrDef that values errSystemSendTerminate
 * if a termination signal has been received 
 * or errOplKernelStackDown if the OpenPOWERLINK stack crashed
 * or noError when the function exits successfully. 
 */
statusErrDef checkStateOpl()
{
    if (system_getTermSignalState() != FALSE)
    {
        printf("Received termination signal, exiting...\n");
        eventlog_printMessage(kEventlogLevelInfo,
            kEventlogCategoryControl,
            "Received termination signal, exiting...");
        return errSystemSendTerminate;
    }
    if (oplk_checkKernelStack() == FALSE)
    {
        fprintf(stderr, "Kernel stack has gone! Exiting...\n");
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "Kernel stack has gone! Exiting...");
        return errOplKernelStackDown;
    }
    return noError;
}

/**
\brief  

- It creates the sync thread which is responsible for the synchronous data
  application.

\return statusErrDef that values errSendNMTResetCommand
when the OpenPOWERLINK reset command fails
or noError when the function exits successfully. 
*/
statusErrDef initOplThread()
{
    tOplkError  ret = kErrorOk;

    system_startFirmwareManagerThread(firmwaremanager_thread, 5);


    // start stack processing by sending a NMT reset command
    ret = oplk_execNmtCommand(kNmtEventSwReset);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_execNmtCommand() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errSendNMTResetCommand;
    }


}

/**
\brief  Synchronous data handler

The function implements the synchronous data handler.

\return The function returns a tOplkError error code.

\ingroup module_demo_mn_console
*/
tOplkError processSync(void)
{
    tOplkError  ret;

    ret = oplk_waitSyncEvent(100000);
    if (ret != kErrorOk)
        return ret;

    ret = oplk_exchangeProcessImageOut();
    if (ret != kErrorOk)
        return ret;

    cnt_l++;

    //Process PI_OUT --> variables entrant dans le MN
    for (int i = 0; i < SIZE_OUT; i++)
    {
        values_In_MN_l[i] = pProcessImageOut_l->out_MN_array[i];
    }

    //Process PI_IN --> variables sortant du MN
    for (int i = 0; i < SIZE_IN; i++)
    {
        if(i % (nbValuesCN_In+1) == 0)
            pProcessImageIn_l->in_MN_array[i] = values_Out_MN_l[i];
    }

    switch (mode)
    {
    case automatic: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel
        break;
    case manual: // mode manuel : l'état des vannes proviennent directement du MN
        for (int i = 1; i < SIZE_IN; i++) {
            if (i % (nbValuesCN_In + 1) != 0)
                pProcessImageIn_l->in_MN_array[i] = values_Out_MN_l[i];
        }
        break;
    default:
        break;
    }
    
    ret = oplk_exchangeProcessImageIn();

    return ret;
}

/**
\brief  Initialize process image

The function initializes the process image of the application.

\return statusErrDef that values errOplkAllocProcessImage
when the allocation of the input and/or output structure 
doesn't exist in the objdict.h file
or errSetupProcessImage when the input and/or output structure
are not the same with what is inside the mnobd.cdc file
or noError when the function exits successfully.
*/
statusErrDef initProcessImage(void)
{
    tOplkError  ret = kErrorOk;
    UINT        errorIndex = 0;

    printf("Initializing process image...\n");
    printf("Size of process image: Input = %lu Output = %lu\n",
        (ULONG)sizeof(PI_IN),
        (ULONG)sizeof(PI_OUT));
    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "Allocating process image: Input:%lu Output:%lu",
        (ULONG)sizeof(PI_IN),
        (ULONG)sizeof(PI_OUT));

    ret = oplk_allocProcessImage(sizeof(PI_IN), sizeof(PI_OUT));
    if (ret != kErrorOk)
        return errOplkAllocProcessImage;

    pProcessImageIn_l = (PI_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (PI_OUT*)oplk_getProcessImageOut();

    errorIndex = obdpi_setupProcessImage();
    if (errorIndex != 0)
    {
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "Setup process image failed at index 0x%04x\n",
            errorIndex);
        return errSetupProcessImage;
    }

    return noError;
}

/**
 * \brief function to shutdown the OpenPOWERLINK module.
 * 
 * \return statusErrDef that values errOplkFreeProcessImage
 * when the freeing of memory of the OpenPOWERLINK fails.
 * or noError when the function exits successfully.
 */
statusErrDef extinctOPL()
{
    statusErrDef res = noError;

    res = shutdownOplImage();
    shutdownPowerlink();
    firmwaremanager_exit();

    return res;
}

/**
\brief  Shutdown the synchronous data application

The function shuts down the synchronous data application

\return statusErrDef that values errOplkFreeProcessImage
when the freeing of memory of the OpenPOWERLINK fails.
or noError when the function exits successfully.

\ingroup module_demo_mn_console
*/
statusErrDef shutdownOplImage()
{
    tOplkError  ret;

    ret = oplk_freeProcessImage();
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_freeProcessImage() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errOplkFreeProcessImage;
    }

    return noError;
}

/**
\brief  Shutdown the demo application

The function shuts down the demo application.
*/
void shutdownPowerlink(void)
{
    UINT        i;
    tOplkError  ret = kErrorOk;

    // NMT_GS_OFF state has not yet been reached
    fGsOff_l = FALSE;

    system_stopFirmwareManagerThread();

    // halt the NMT state machine so the processing of POWERLINK frames stops
    ret = oplk_execNmtCommand(kNmtEventSwitchOff);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_execNmtCommand() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
    }

    // small loop to implement timeout waiting for thread to terminate
    for (i = 0; i < 1000; i++)
    {
        if (fGsOff_l)
            break;
    }

    printf("Stack is in state off ... Shutdown\n");
    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryControl,
        "Stack is in state off ... Shutdown openPOWERLINK");

    oplk_destroy();
    oplk_exit();
}