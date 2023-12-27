#include "opl.h"

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static const UINT8              aMacAddr_l[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static BOOL                     fGsOff_l;

static int                      aUsedNodeIds_l[] = { 1, 2, 3, 0 };
static UINT                     cnt_l;
static APP_NODE_VAR_T           aNodeVar_l[MAX_NODES];
static UNION_OUT*               pProcessImageOut_l;
static const UNION_IN*          pProcessImageIn_l;

static int32_t                  values_In_MN_l[MAX_VALUES];
static int32_t                  values_Out_MN_l[MAX_VALUES];
static bool                     activated_In_MN_l[COMPUTED_PI_IN_SIZE];
static bool                     activated_Out_MN_l[COMPUTED_PI_OUT_SIZE];
static int test;


opl::opl()
{

}

opl::~opl()
{
    //destructor
}


bool opl::demandeExtinctOPL()
{

    printf("\n\n values_Out_MN_l[0] : %d \n\n", values_Out_MN_l[0]);

    printf("\n\n test : %d \n\n", test);
    #if (TARGET_SYSTEM == _WIN32_)
    Sleep(500);
    #else
    sleep(500);
    #endif
    
    if (values_Out_MN_l[0] == 0x1FFF)
        return true;
    else
        return false;

}

void opl::sendTelem()
{

}

void opl::sendError()
{

}

void opl::setValues_In_MN(int32_t values_In_g[])
{
    for (int i = 0; i < COMPUTED_PI_IN_SIZE; i++)
    {
        values_In_MN_l[i] = values_In_g[i];
    }
}

int32_t* opl::getValues_In_MN()
{
    return values_In_MN_l;
}

void opl::setValues_Out_MN(int32_t values_Out_g[])
{
    for (int i = 0; i < COMPUTED_PI_OUT_SIZE; i++)
    {
        values_Out_MN_l[i] = values_Out_g[i];
    }
}

int32_t* opl::getValues_Out_MN()
{
    return values_Out_MN_l;
}

void opl::setActivated_In_MN(int32_t activated_In_MN_g[])
{
    for (int i = 0; i < COMPUTED_PI_IN_SIZE; i++)
    {
        activated_In_MN_l[i] = activated_In_MN_g[i];
    }
}

void opl::setActivated_Out_MN(int32_t activated_Out_MN_g[])
{
    for (int i = 0; i < COMPUTED_PI_OUT_SIZE; i++)
    {
        activated_Out_MN_l[i] = activated_Out_MN_g[i];
    }
}

extern "C"
{


//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static tOplkError       processSync(void);

bool initOPL()
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
        return false;
    }

    fwRet = firmwaremanager_init(opts.fwInfoFile);
    if (fwRet != kFwReturnOk)
    {
        fprintf(stderr, "Error initializing firmware manager!");
        return false;
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

    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "Using CDC file: %s",
        opts.cdcFile);

    ret = initPowerlink(CYCLE_LEN,
        opts.cdcFile,
        opts.devName,
        aMacAddr_l);
    if (ret != kErrorOk)
        return false;

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

\ingroup module_demo_mn_console
*/
//------------------------------------------------------------------------------
tOplkError initApp(void)
{
    tOplkError  ret = kErrorOk;
    int         i;

    cnt_l = 0;

    for (i = 0; (i < MAX_NODES) && (aUsedNodeIds_l[i] != 0); i++)
    {
        aNodeVar_l[i].leds = 0;
        aNodeVar_l[i].ledsOld = 0;
        aNodeVar_l[i].input = 0;
        aNodeVar_l[i].inputOld = 0;
        aNodeVar_l[i].toggle = 0;
        aNodeVar_l[i].period = 0;
    }
    i = 0;

    for (i = 0; i < MAX_VALUES; i++)
    {
        values_In_MN_l[i] = 0;
        values_Out_MN_l[i] = 0;
    }
    memset(&pProcessImageOut_l, 0, sizeof(pProcessImageOut_l));
    memset(&pProcessImageIn_l, 0, sizeof(pProcessImageIn_l));
    test = 0x1FFF;
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
    int         i;

    ret = oplk_waitSyncEvent(100000);
    if (ret != kErrorOk)
        return ret;

    ret = oplk_exchangeProcessImageOut();
    if (ret != kErrorOk)
        return ret;

    //for (int i = 0; i < sizeof(pProcessImageOut_l->in_MN_array) / sizeof(pProcessImageOut_l->in_MN_array[0]); i++)
    //{
    //    values_IO_l[i] = pProcessImageOut_l->in_MN_array[i];
    //    //printf("arropl at %d = %d \n", i, values_IO_l[i]);
    //}

    cnt_l++;

    aNodeVar_l[0].input = pProcessImageIn_l->in_MN_array[1];
    aNodeVar_l[1].input = pProcessImageIn_l->in_MN_array[26];
    aNodeVar_l[2].input = pProcessImageIn_l->in_MN_array[51];

    values_In_MN_l[1] = pProcessImageIn_l->in_MN_array[1];

    for (int i = 0; i < COMPUTED_PI_OUT_SIZE; i++)
    {
        if (activated_Out_MN_l[i])
        {
            pProcessImageOut_l->out_MN_array[i] = values_Out_MN_l[i];
        }
    }

    for (int i = 0; i < COMPUTED_PI_IN_SIZE; i++)
    {
        if (activated_In_MN_l[i])
        {
            values_In_MN_l[i] = pProcessImageIn_l->in_MN_array[i];
        }
    }


    for (i = 0; (i < MAX_NODES) && (aUsedNodeIds_l[i] != 0); i++)
    {
        /* Running LEDs */
        /* period for LED flashing determined by inputs */
        aNodeVar_l[i].period = (aNodeVar_l[i].input == 0) ? 1 : (aNodeVar_l[i].input * 20);
        if (cnt_l % aNodeVar_l[i].period == 0)
        {
            if (aNodeVar_l[i].leds == 0x00000000)
            {
                aNodeVar_l[i].leds = 0x1;
                aNodeVar_l[i].toggle = 1;
            }
            else
            {
                if (aNodeVar_l[i].toggle)
                {
                    aNodeVar_l[i].leds <<= 1;
                    if (aNodeVar_l[i].leds == APP_LED_MASK_1)
                        aNodeVar_l[i].toggle = 0;
                }
                else
                {
                    aNodeVar_l[i].leds >>= 1;
                    if (aNodeVar_l[i].leds == 0x00000001)
                        aNodeVar_l[i].toggle = 1;
                }
            }
        }

        if (aNodeVar_l[i].input != aNodeVar_l[i].inputOld)
            aNodeVar_l[i].inputOld = aNodeVar_l[i].input;

        if (aNodeVar_l[i].leds != aNodeVar_l[i].ledsOld)
            aNodeVar_l[i].ledsOld = aNodeVar_l[i].leds;
    }

    pProcessImageOut_l->out_MN_array[0] = aNodeVar_l[0].leds;
    pProcessImageOut_l->out_MN_array[13] = aNodeVar_l[1].leds;
    pProcessImageOut_l->out_MN_array[26] = aNodeVar_l[2].leds;
    values_Out_MN_l[0] = 0x1FFF;

    pProcessImageOut_l->out_MN_array[0] = values_Out_MN_l[0];

    
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
        (ULONG)sizeof(UNION_IN),
        (ULONG)sizeof(UNION_OUT));
    eventlog_printMessage(kEventlogLevelInfo,
        kEventlogCategoryGeneric,
        "Allocating process image: Input:%lu Output:%lu",
        (ULONG)sizeof(UNION_IN),
        (ULONG)sizeof(UNION_OUT));

    ret = oplk_allocProcessImage(sizeof(UNION_IN), sizeof(UNION_OUT));
    if (ret != kErrorOk)
        return ret;

    pProcessImageIn_l = (const UNION_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (UNION_OUT*)oplk_getProcessImageOut();

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

bool testOPL() 
{
    return true;
}

bool ExtinctOPL()
{
    shutdownOplImage();
    shutdownPowerlink();
    firmwaremanager_exit();

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

}