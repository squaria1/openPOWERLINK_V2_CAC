#include "opl.h"

uint8_t              mode;

const uint16_t       nbValuesCN_Out = SIZE_OUT / NB_NODES - 1;
const uint16_t       nbValuesCN_In = SIZE_IN / NB_NODES - 1;

opl::opl()
{

}

opl::~opl()
{
    //destructor
}


int16_t opl::demandeExtinctOPL()
{
    processSync();
    
    if (values_Out_MN_l[0] == 0x1FFF)
    {
        printf("\n\n values_Out_MN_l[0] : %d \n\n", values_Out_MN_l[0]);
        printf("\n\n extinction ! \n\n");
        return 0;
    }
    else
        return 1;

}

void opl::sendTelem()
{

}

void opl::sendError()
{

}

void setValues_In_MN(int ligne, int16_t valeur)
{
        values_In_MN_l[ligne] = valeur;
}

int16_t* getValues_In_MN()
{
    return values_In_MN_l;
}

void setValues_Out_MN(int ligne, int16_t valeur)
{
        values_Out_MN_l[ligne] = valeur;
}

int16_t* getValues_Out_MN()
{
    return values_Out_MN_l;
}

void setActivated_In_MN()
{
    for (int i = 0; i < SIZE_OUT + 1; i++)
    {
        activated_In_MN_l[i] = getActivation(i);
    }
}

/*
void setActivated_Out_MN()
{
    for (int i = 0; i < SIZE_OUT; i++)
    {
        activated_Out_MN_l[i] = activated_Out_MN_g[i];
    }
}
*/

int16_t getEC1()
{
    return values_In_MN_l[0];
}

void setEG(int16_t EG)
{
    for (int i = 0; i < SIZE_IN; i++)
    {
        if (i % (nbValuesCN_In + 1) == 0)
            values_Out_MN_l[i] = EG;
    }
}

int16_t getTest()
{
    return values_Out_MN_l[1];
}

void affValeursIn()
{
    printf("\n-------------IN MN--------------\n");
    for (int i = 0; i < SIZE_OUT; i++)
    {
        printf("values_In_MN_l[%d]=%d\n", i, values_In_MN_l[i]);
    }
    printf("\n--------------------------------\n");
}

void affValeursInProcess()
{
    printf("\n-------------IN PROCESS MN--------------\n");
    for (int i = 0; i < SIZE_OUT + 2 * NB_NODES; i++)
    {
        printf("pProcessImageOut_l->out_MN_array[%d]=%d\n", i, pProcessImageOut_l->out_MN_array[i]);
    }
    printf("\n--------------------------------\n");
}

void affValeursOut()
{
    printf("\n------------OUT MN--------------\n");
    for (int i = 0; i < SIZE_IN; i++)
    {
        printf("values_Out_MN_l[%d]=%d\n", i, values_Out_MN_l[i]);
    }
    printf("\n--------------------------------\n");
}


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------


int16_t initOPL()
{
    
    tOplkError      ret = kErrorOk;
    tOptions        opts;
    tEventConfig    eventConfig;
    tFirmwareRet    fwRet;

    strncpy(opts.cdcFile, "mnobd.cdc", 256);
    strncpy(opts.fwInfoFile, "fw.info", 256);
    strncpy(opts.devName, "eth0", 128);
    opts.pLogFile = NULL;
    opts.logFormat = kEventlogFormatReadable;
    opts.logCategory = 0xffffffff;
    opts.logLevel = 0xffffffff;

    if (system_init() != 0)
    {
        fprintf(stderr, "Error initializing system!");
        return 1;
    }

    fwRet = firmwaremanager_init(opts.fwInfoFile);
    if (fwRet != kFwReturnOk)
    {
        fprintf(stderr, "Error initializing firmware manager!");
        return 1;
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

    ret = initPowerlink(CYCLE_LEN,
        opts.cdcFile,
        opts.devName,
        aMacAddr_l);
    if (ret != kErrorOk)
        return 1;

    ret = initApp();
    if (ret != kErrorOk)
        return 1;

    initOplThread();

    return 0;
}

//------------------------------------------------------------------------------
/**
\brief  Initialize the synchronous data application

The function initializes the synchronous data application

\return The function returns a tOplkError error code.

\ingroup module_demo_mn_console
*/
//------------------------------------------------------------------------------
tOplkError initApp(void)
{
    tOplkError  ret = kErrorOk;
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

    memset(&pProcessImageOut_l, 0, sizeof(pProcessImageOut_l));
    ret = initProcessImage();

    return ret;
}


//------------------------------------------------------------------------------
/**
\brief  Initialize the openPOWERLINK stack

The function initializes the openPOWERLINK stack.

\param[in]      cycleLen_p          Length of POWERLINK cycle.
\param[in]      cdcFileName_p       Name of the CDC file.
\param[in]      devName_p           Device name string.
\param[in]      macAddr_p           MAC address to use for POWERLINK interface.

\return The function returns a tOplkError error code.
*/
//------------------------------------------------------------------------------
tOplkError initPowerlink(UINT32 cycleLen_p,
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

    if (netselect_selectNetworkInterface(devName, sizeof(devName)) < 0)
        return kErrorIllegalInstance;

    //strncpy(devName, devName_p, 128);

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
        return ret;
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
        return ret;
    }

    ret = oplk_create(&initParam);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_create() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return ret;
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
        return ret;
    }

    return kErrorOk;
}

//------------------------------------------------------------------------------
/**
\brief  

- It creates the sync thread which is responsible for the synchronous data
  application.
*/
//------------------------------------------------------------------------------
void initOplThread(void)
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
        return;
    }


}

//------------------------------------------------------------------------------
/**
\brief  Synchronous data handler

The function implements the synchronous data handler.

\return The function returns a tOplkError error code.

\ingroup module_demo_mn_console
*/
//------------------------------------------------------------------------------
tOplkError processSync(void)
{
    tOplkError  ret;

    ret = oplk_waitSyncEvent(100000);
    if (ret != kErrorOk)
        return ret;

    //memset(pProcessImageOut_l->out_MN_array, 0, sizeof(pProcessImageOut_l->out_MN_array));

    ret = oplk_exchangeProcessImageOut();
    if (ret != kErrorOk)
        return ret;

    cnt_l++;

    int a = 0;
    //for (int i = 0; i < NB_NODES; i++) {
    //    pProcessImageOut_l->out_MN_array[a] = 1;
    //    for (int j = 1; j <= i + 1; j++)
    //    {
    //        pProcessImageOut_l->out_MN_array[a + j] = 0;
    //    }
    //    for (int k = 0; k < nbValuesCN_Out; k++) {
    //        pProcessImageOut_l->out_MN_array[a + k + i + 2] = k + 1;
    //    }

    //    a = a + nbValuesCN_Out + 2 + i;
    //}
    //for (int i = 54; i < 58; i++)
    //{
    //    values_In_MN_l[i] = pProcessImageOut_l->out_MN_array[i];
    //}
    //a = 0;
    /*int16_t test1 = pProcessImageOut_l->out_MN_array[29], test2 = pProcessImageOut_l->out_MN_array[57];
    values_In_MN_l[29] = test1;
    values_In_MN_l[57] = test2;
    printf("\npProcessImageOut_l->out_MN_array[29] : %d\n", test1);
    printf("values_In_MN_l[29] : %d\n", values_In_MN_l[29]);
    printf("values_In_MN_l[57] : %d\n", values_In_MN_l[57]);
    printf("pProcessImageOut_l->out_MN_array[57] : %d\n", test2);*/
    //Process PI_OUT --> variables entrant dans le MN
    //for (int i = 0; i < NB_NODES; i++) 
    //{
    //    values_In_MN_l[(nbValuesCN_Out + 1) * i] = pProcessImageOut_l->out_MN_array[a];
    //    for (int j = 0; j < nbValuesCN_Out; j++) 
    //    {
    //        if (activated_In_MN_l[(nbValuesCN_Out + 1) * i + j + 1])
    //            values_In_MN_l[(nbValuesCN_Out + 1) * i + j + 1] = pProcessImageOut_l->out_MN_array[a + j + i + 2];
    //    }
    //    a = a + nbValuesCN_Out + 2 + i;
    //}
    for (int i = 0; i < NB_NODES; i++) {
        values_In_MN_l[(nbValuesCN_Out + 1) * i] = pProcessImageOut_l->out_MN_array[a];
        for (int j = 0; j < nbValuesCN_Out; j++) {
            if (activated_In_MN_l[(nbValuesCN_Out + 1) * i + j + 1])
                values_In_MN_l[(nbValuesCN_Out + 1) * i + j + 1] = pProcessImageOut_l->out_MN_array[a + j + i + 2];
        }
        a = a + nbValuesCN_Out + 2 + i;
    }
    values_In_MN_l[13] = pProcessImageOut_l->out_MN_array[14];
    //for (int i = 0; i < NB_NODES; i++) 
    //{
    //    values_In_MN_l[(nbValuesCN_Out + 1) * i] = tabInit[a];
    //    for (int j = 0; j < nbValuesCN_Out + 1; j++) 
    //    {
    //        values_In_MN_l[(nbValuesCN_Out + 1) * i + j + 1] = tabInit[a + j + 2];
    //    }
    //    a = a + nbValuesCN_Out + 2;
    //}
    //Process PI_IN --> variables sortant du MN
    for (int i = 0; i < SIZE_IN; i++)
    {
        if(i % (nbValuesCN_In+1) == 0)
            pProcessImageIn_l->in_MN_array[i] = values_Out_MN_l[i];
    }

    int skipSensorsOutFromIn = 1;
    switch (mode)
    {
    case 0: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel

        for (int i = 1; i < SIZE_IN; i++)
        {
            if (i % (nbValuesCN_In + 1) == 0)
                skipSensorsOutFromIn += nbValuesCN_In + 1;
            else if (i % (nbValuesCN_In + 2) == 0)
                i++;
            else
                skipSensorsOutFromIn += 1;

            if (i % (nbValuesCN_In + 1) != 0 && activated_In_MN_l[skipSensorsOutFromIn])
            {
                values_Out_MN_l[i] = values_In_MN_l[i];
                pProcessImageIn_l->in_MN_array[i] = values_Out_MN_l[i];
            }
        }
        break;
    case 1: // mode manuel : l'état des vannes proviennent directement du MN
        for (int i = 0; i < SIZE_IN; i++)
        {
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

//------------------------------------------------------------------------------
/**
\brief  Initialize process image

The function initializes the process image of the application.

\return The function returns a tOplkError error code.
*/
//------------------------------------------------------------------------------
tOplkError initProcessImage(void)
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
        return ret;

    pProcessImageIn_l = (PI_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (PI_OUT*)oplk_getProcessImageOut();

    errorIndex = obdpi_setupProcessImage();
    if (errorIndex != 0)
    {
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "Setup process image failed at index 0x%04x\n",
            errorIndex);
        ret = kErrorApiPINotAllocated;
    }

    return ret;
}

int16_t extinctOPL()
{
    shutdownOplImage();
    shutdownPowerlink();
    firmwaremanager_exit();

    return 0;
}

//------------------------------------------------------------------------------
/**
\brief  Shutdown the synchronous data application

The function shuts down the synchronous data application

\return The function returns a tOplkError error code.

\ingroup module_demo_mn_console
*/
//------------------------------------------------------------------------------
void shutdownOplImage(void)
{
    tOplkError  ret;

    ret = oplk_freeProcessImage();
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "oplk_freeProcessImage() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
    }
}

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