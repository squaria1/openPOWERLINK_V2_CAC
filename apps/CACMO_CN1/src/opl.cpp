#include "opl.h"


opl::opl()
{

}

opl::~opl()
{
    //destructor
}


bool opl::demandeExtinctOPL()
{
    //processSync();
    
    //printf("\n\n values_In_CN_l[0] : %d \n\n", values_In_CN_l[0]);
    
    if (values_In_CN_l[0] == 0x1FFF)
        return true;
    else
        return false;

}

void opl::sendTelem(int16_t statusCode)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN-1] = statusCode;
    processSync();

    printf("\n\n EG CN : %d \n\n", values_In_CN_l[0]);

    printf("\n\n EC1 CN : %d \n\n", values_Out_CN_l[nbValuesCN_Out_ByCN - 1]);
}

void opl::sendError(int16_t errorCode)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN-1] = errorCode;
    processSync();

    printf("\n\n EG CN : %d \n\n", values_In_CN_l[0]);

    printf("\n\n EC1 CN : %d \n\n", values_Out_CN_l[nbValuesCN_Out_ByCN - 1]);
}

void opl::setValues_In_CN(int32_t values_In_CN_g[])
{
    for (int i = 0; i < COMPUTED_PI_IN_SIZE; i++)
    {
        values_In_CN_l[i] = values_In_CN_g[i];
    }
}

int32_t* opl::getValues_In_CN()
{
    return values_In_CN_l;
}

void opl::setValues_Out_CN(int32_t values_Out_CN_g[])
{
    for (int i = 0; i < COMPUTED_PI_OUT_SIZE; i++)
    {
        values_Out_CN_l[i] = values_Out_CN_g[i];
    }
}

int32_t* opl::getValues_Out_CN()
{
    return values_Out_CN_l;
}

void opl::setActivated_In_CN(int32_t activated_In_CN_g[])
{
    for (int i = 0; i < COMPUTED_PI_IN_SIZE; i++)
    {
        activated_In_CN_l[i] = activated_In_CN_g[i];
    }
}

void opl::setActivated_Out_CN(int32_t activated_Out_CN_g[])
{
    for (int i = 0; i < COMPUTED_PI_OUT_SIZE; i++)
    {
        activated_Out_CN_l[i] = activated_Out_CN_g[i];
    }
}

int16_t opl::getEG()
{
    return values_In_CN_l[0];
}

void opl::setEC1(int16_t EC1)
{
    values_Out_CN_l[0] = EC1;
}

extern "C"
{


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

bool initOPL()
{

    tOplkError  ret = kErrorOk;
    tOptions    opts;

    strncpy(opts.devName, "eth0", 128);
    opts.nodeId = NODEID;
    opts.logFormat = kEventlogFormatReadable;
    opts.logCategory = 0xffffffff;
    opts.logLevel = 0xffffffff;

    if (system_init() != 0)
    {
        fprintf(stderr, "Error initializing system!");
        return false;
    }

    eventlog_init(opts.logFormat,
        opts.logLevel,
        opts.logCategory,
        (tEventlogOutputCb)console_printlogadd);


    initEvents(&fGsOff_l);

    printf("----------------------------------------------------\n");
    printf("openPOWERLINK console CN DEMO application\n");
    printf("Using openPOWERLINK stack: %s\n", oplk_getVersionString());
    printf("----------------------------------------------------\n");

    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "demo_cn_console: Stack version:%s Stack configuration:0x%08X",
        oplk_getVersionString(),
        oplk_getStackConfiguration());

    ret = initPowerlink(CYCLE_LEN,
                        opts.devName,
                        aMacAddr_l,
                        opts.nodeId);

    ret = initApp();
    if (ret != kErrorOk)
        return false;

    initOplThread();

    return true;
}

//------------------------------------------------------------------------------
/**
\brief  Initialize the synchronous data application

The function initializes the synchronous data application

\return The function returns a tOplkError error code.

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
tOplkError initApp(void)
{
    tOplkError  ret;

    ret = initProcessImage();

    return ret;
}


//------------------------------------------------------------------------------
/**
\brief  Initialize the openPOWERLINK stack

The function initializes the openPOWERLINK stack.

\param[in]      cycleLen_p          Length of POWERLINK cycle.
\param[in]      devName_p           Device name string.
\param[in]      macAddr_p           MAC address to use for POWERLINK interface.
\param[in]      nodeId_p            POWERLINK node ID.

\return The function returns a tOplkError error code.
*/
//------------------------------------------------------------------------------
tOplkError initPowerlink(UINT32 cycleLen_p,
                                const char* devName_p,
                                const UINT8* macAddr_p,
                                UINT32 nodeId_p)
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

    memset(&initParam, 0, sizeof(initParam));
    initParam.sizeOfInitParam = sizeof(initParam);

    // pass selected device name to Edrv
    initParam.hwParam.pDevName = devName;
    initParam.nodeId = nodeId_p;
    initParam.ipAddress = (0xFFFFFF00 & IP_ADDR) | initParam.nodeId;

    /* write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address */
    memcpy(initParam.aMacAddress, macAddr_p, sizeof(initParam.aMacAddress));

    initParam.fAsyncOnly = FALSE;
    initParam.featureFlags = UINT_MAX;
    initParam.cycleLen = cycleLen_p;             // required for error detection
    initParam.isochrTxMaxPayload = C_DLL_ISOCHR_MAX_PAYL;  // const
    initParam.isochrRxMaxPayload = C_DLL_ISOCHR_MAX_PAYL;  // const
    initParam.presMaxLatency = 50000;                  // const; only required for IdentRes
    initParam.preqActPayloadLimit = 36;                     // required for initialization (+28 bytes)
    initParam.presActPayloadLimit = 36;                     // required for initialization of Pres frame (+28 bytes)
    initParam.asndMaxLatency = 150000;                 // const; only required for IdentRes
    initParam.multiplCylceCnt = 0;                      // required for error detection
    initParam.asyncMtu = 1500;                   // required to set up max frame size
    initParam.prescaler = 2;                      // required for sync
    initParam.lossOfFrameTolerance = 500000;
    initParam.asyncSlotTimeout = 3000000;
    initParam.waitSocPreq = 1000;
    initParam.deviceType = UINT_MAX;               // NMT_DeviceType_U32
    initParam.vendorId = UINT_MAX;               // NMT_IdentityObject_REC.VendorId_U32
    initParam.productCode = UINT_MAX;               // NMT_IdentityObject_REC.ProductCode_U32
    initParam.revisionNumber = UINT_MAX;               // NMT_IdentityObject_REC.RevisionNo_U32
    initParam.serialNumber = UINT_MAX;               // NMT_IdentityObject_REC.SerialNo_U32
    initParam.applicationSwDate = 0;
    initParam.applicationSwTime = 0;
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
        fprintf(stderr,
            "obdcreate_initObd() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
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
            "oplk_initialize() failed with \"%s\" (0x%04x)\n",
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
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "oplk_create() failed with \"%s\" (0x%04x)\n",
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

    setupInputs();

    processSync();
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
    tOplkError  ret = kErrorOk;

    if (oplk_waitSyncEvent(100000) != kErrorOk)
        return ret;


    ret = oplk_exchangeProcessImageOut();
    if (ret != kErrorOk)
        return ret;

    /* read input image - digital outputs */

    pProcessImageOut_l->out_CN_array[0] = values_Out_CN_l[0];

    /*
    activated_Out_CN_l[nbValuesCN_Out_ByCN - 1] = true;
    for (int i = nbValuesCN_Out_ByCN; i < nbValuesCN_Out_ByCN + nbValuesCN_Out; i++)
    {
        if (activated_Out_CN_l[i])
            pProcessImageOut_l->out_CN_array[i] = values_Out_CN_l[i];
    }
    */

    values_In_CN_l[0] = pProcessImageIn_l->in_CN_array[0];

    /* setup output image - digital inputs */

    /*
    activated_In_CN_l[0] = true;
    // Example : CN3 and 3 CNs --> from nbValuesCN_Out_ByCN = 75 / 3 * (3 - 1) = 50 to nbValuesCN_Out_ByCN + nbValuesCN_Out = 50 + 25 = 75
    for (int i = nbValuesCN_In_ByCN; i < nbValuesCN_In_ByCN + nbValuesCN_In; i++)
    {
        if (activated_In_CN_l[i])
            values_In_CN_l[i] = pProcessImageIn_l->in_CN_array[i];
    }

    ret = oplk_exchangeProcessImageIn();
    if (ret != kErrorOk)
        return ret;
    */

    ret = oplk_exchangeProcessImageIn();

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Setup inputs

The function initializes the digital input port.

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
void setupInputs(void)
{
    memset(&values_In_CN_l, 0, sizeof(values_In_CN_l));
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
    UINT        varEntries;
    tObdSize    obdSize;

    /* Allocate process image */
    printf("Initializing process image...\n");
    printf("Size of process image: Input = %lu Output = %lu \n",
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

    pProcessImageIn_l = (const PI_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (PI_OUT*)oplk_getProcessImageOut();

    /* link process variables used by CN to object dictionary */
    fprintf(stderr, "Linking process image vars:\n");

    varEntries = 1;

    //Link image EC of the correct NODEID
    obdSize = 2;
    ret = linkPDO_out(varEntries, obdSize, nbValuesCN_Out_ByCN-1, 0x6511, NODEID);
    if (ret != kErrorOk)
    {
        return ret;
    }
    /*
    // Init process image output
    // Example : CN3 and 3 CNs --> from nbValuesCN_Out_ByCN = 75 / 3 * (3 - 1) = 50 to nbValuesCN_Out_ByCN + nbValuesCN_Out = 50 + 25 = 75
    for (int i = nbValuesCN_Out_ByCN; i < nbValuesCN_Out_ByCN + nbValuesCN_Out; i++)
    {
        if (activated_Out_CN_l[i])
        {
            //Link valves images
            if (i > nbValuesCN_Out_ByCN && i <= nbValuesCN_Out_ByCN + nbValuesCN_Out / 2)
            {
                ret = linkPDO_out(varEntries, obdSize, i, 0x6510, 0x01 + i % (nbValuesCN_Out / 2));
            }
            //Link sensors images
            else if (i > nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 && i <= nbValuesCN_Out_ByCN + nbValuesCN_Out)
            {
                ret = linkPDO_out(varEntries, obdSize, i, 0x6512, 0x01 + i % (nbValuesCN_Out / 2));
            }
            if (ret != kErrorOk)
            {
                return ret;
            }
        }
    }
    // Init process image input
    for (int i = nbValuesCN_In_ByCN; i < nbValuesCN_In_ByCN + nbValuesCN_In / 2; i++)
    {
        //Link valves images in from MN
        if (activated_In_CN_l[i])
        {
            ret = linkPDO_in(varEntries, obdSize, i, 0x6500, 0x01 + i % (nbValuesCN_In / 2));
            if (ret != kErrorOk)
            {
                return ret;
            }
        }
    }
    */

    varEntries = 1;
    // Link image input EG
    ret = linkPDO_in(varEntries, obdSize, 0, 0x6501, 0xF0);
    if (ret != kErrorOk)
    {
        return ret;
    }

    fprintf(stderr, "Linking process vars... ok\n\n");

    return kErrorOk;
}


tOplkError linkPDO_in(UINT varEntries, tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex) {
    tOplkError  ret = kErrorOk;

    //ret = oplk_linkProcessImageObject(index,
    //    subIndex,
    //    offsetof(PI_IN, in_CN_array[0]) + sizeof(INT16) * arrayIndex,
    //    FALSE,
    //    obdSize,
    //    &varEntries);
    ret = oplk_linkProcessImageObject(index,
        subIndex,
        offsetof(PI_IN, in_CN_array[arrayIndex]),
        FALSE,
        obdSize,
        &varEntries);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "Linking process vars failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
    }

    return ret;
}

tOplkError linkPDO_out(UINT varEntries, tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex) {
    tOplkError  ret = kErrorOk;

    //ret = oplk_linkProcessImageObject(index,
    //    subIndex,
    //    offsetof(PI_OUT, out_CN_array[0]) + sizeof(INT16) * arrayIndex,
    //    FALSE,
    //    obdSize,
    //    &varEntries);
    ret = oplk_linkProcessImageObject(index,
        subIndex,
        offsetof(PI_OUT, out_CN_array[arrayIndex]),
        FALSE,
        obdSize,
        &varEntries);
    if (ret != kErrorOk)
    {
        fprintf(stderr,
            "Linking process vars failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
    }

    return ret;
}

bool ExtinctOPL()
{
    shutdownOplImage();
    shutdownPowerlink();
    system_exit();

    return true;
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

//------------------------------------------------------------------------------
/**
\brief  Shutdown the demo application

The function shuts down the demo application.
*/
//------------------------------------------------------------------------------
void shutdownPowerlink(void)
{
    UINT    i;

    fGsOff_l = FALSE;

    // halt the NMT state machine so the processing of POWERLINK frames stops
    oplk_execNmtCommand(kNmtEventSwitchOff);

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

}