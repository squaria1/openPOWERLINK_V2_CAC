#include "opl.h"

uint8_t              mode;
int16_t              EG = 1;
int16_t              EC;
const uint16_t       nbValuesCN_Out = SIZE_OUT / NB_NODES - 1;
const uint16_t       nbValuesCN_In = SIZE_IN / NB_NODES - 1;
const uint16_t       nbValuesCN_Out_ByCN = (SIZE_OUT / NB_NODES) * (NODEID - 1);
const uint16_t       nbValuesCN_In_ByCN = (SIZE_IN / NB_NODES) * (NODEID - 1);

opl::opl()
{

}

opl::~opl()
{
    //destructor
}


int16_t opl::demandeExtinctOPL()
{
    if (values_In_CN_l[0] == 0x1FFF)
        return 0;
    else
        return 1;

}

void opl::sendTelem(int16_t statusCode)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = statusCode;
}

void opl::sendError(int16_t errorCode)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = errorCode;
}

/*
void setValues_In_CN(int ligne)
{
    switch (mode)
    {
    case 0: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel
        for (int i = 0; i < SIZE_IN; i++)
        {
            if (i % (MAX_VALVES + 1) == 0)
                values_In_CN_l[i] = getValeur(i);
        }
        break;
    default:
        break;
    }
}
*/

void affValeursIn()
{
    printf("\n-------------IN CN--------------\n");
    for (int i = 0; i < SIZE_IN; i++)
    {
        printf("values_In_CN_l[%d]=%d\n", i, values_In_CN_l[i]);
    }
    printf("\n--------------------------------\n");
}

void affValeursProcessIn()
{
    printf("\n-------------PROCESS IN CN--------------\n");
    for (int i = 0; i < SIZE_IN; i++)
    {
        printf("pProcessImageIn_l->in_CN_array[%d]=%d\n", i, pProcessImageIn_l->in_CN_array[i]);
    }
    printf("\n----------------------------------------\n");
}

void affValeursOut()
{
    printf("\n------------OUT CN1--------------\n");
    for (int i = nbValuesCN_Out_ByCN; i < nbValuesCN_Out_ByCN + nbValuesCN_Out + 1; i++)
    {
        printf("activated_Out_CN_l[%d]=%d\n", i + 1, activated_Out_CN_l[i + 1]);
        printf("values_Out_CN_l[%d]=%d\n", i, values_Out_CN_l[i]);
    }
    printf("\n--------------------------------\n");
}

int16_t getValues_In_CN(int ligne)
{
    return values_In_CN_l[ligne];
}

void setValues_Out_CN()
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = EC;
    for (int i = 0; i < MAX_VALVES; i++) { //0 taille tab de benoit
        values_Out_CN_l[i + nbValuesCN_Out_ByCN + 1] = getValveValue(i + nbValuesCN_In_ByCN + 2);
    }

    for (int i = 0; i < MAX_SENSORS; i++) { //0 taille tab de benoit
        values_Out_CN_l[i + nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 + 1] = getAdc_value(i);
    }
}

int16_t getValues_Out_CN(int ligne)
{
    return values_Out_CN_l[ligne];
}
/*
void setActivated_In_CN(int ligne)
{
    switch (mode)
    {
    case 0: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel
        for (int i = 0; i < SIZE_IN; i++)
        {
            if (i % (MAX_VALVES + 1) == 0)
                activated_In_CN_l[i] = getActivation(data, i);
        }
        break;
    case 1: // mode manuel : l'état des vannes proviennent directement du MN
        for (int i = 0; i < SIZE_IN; i++)
        {
            activated_In_CN_l[i] = getActivation(data, i);
        }
    default:
        break;
    }
}
*/

void setActivated_Out_CN()
{
    for (int i = 0; i < SIZE_OUT; i++)
    {
        activated_Out_CN_l[i] = getActivation(i);
    }
}

int16_t getEG()
{
    printf("\n\n EG CN truc : %d \n\n", values_In_CN_l[nbValuesCN_In_ByCN]);
    return values_In_CN_l[nbValuesCN_In_ByCN];
}

int16_t isEGchanged()
{
    if (values_In_CN_l[nbValuesCN_In_ByCN] != EG && values_In_CN_l[nbValuesCN_In_ByCN] != 0)
    {
        EG = values_In_CN_l[nbValuesCN_In_ByCN];
        int16_t ret = resetTimers();
        return ret;
    }
    else
        return 1;

}

void setEC1(int16_t EC1)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = EC1;
}

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------

int16_t initOPL()
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
        return 1;
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

    setActivated_Out_CN();

    ret = initPowerlink(CYCLE_LEN,
        opts.devName,
        aMacAddr_l,
        opts.nodeId);

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

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
tOplkError initApp()
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
tOplkError processSync()
{
    tOplkError  ret = kErrorOk;

    if (oplk_waitSyncEvent(100000) != kErrorOk)
        return ret;


    ret = oplk_exchangeProcessImageOut();
    if (ret != kErrorOk)
        return ret;


    //Process PI_IN --> variables entrant dans le CN
    //values_In_CN_l[nbValuesCN_In_ByCN] = pProcessImageIn_l->in_CN_array[nbValuesCN_In_ByCN];
    for (int i = 0; i < SIZE_IN; i++)
    {
        values_In_CN_l[i] = pProcessImageIn_l->in_CN_array[i];
    }

    //int skipSensorsOutFromIn = 1;

    //// Example : CN3 and 3 CNs --> from nbValuesCN_Out_ByCN = 75 / 3 * (3 - 1) = 50 to nbValuesCN_Out_ByCN + nbValuesCN_Out = 50 + 25 = 75
    //switch (mode)
    //{
    //case 0: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel
    //    for (int i = 1; i < SIZE_IN; i++)
    //    {
    //        if (i % (nbValuesCN_In + 1) == 0)
    //            skipSensorsOutFromIn += nbValuesCN_In + 1;
    //        else
    //            skipSensorsOutFromIn += 1;
    //        if (i % (nbValuesCN_In + 1) != 0 && activated_Out_CN_l[skipSensorsOutFromIn])
    //            values_In_CN_l[i] = pProcessImageIn_l->in_CN_array[i];
    //    }
    //    break;
    //case 1: // mode manuel : l'état des vannes proviennent directement du MN
    //    for (int i = nbValuesCN_In_ByCN; i < nbValuesCN_In_ByCN + nbValuesCN_In; i++)
    //    {
    //        values_In_CN_l[i] = pProcessImageIn_l->in_CN_array[i];
    //    }
    //    break;
    //default:
    //    break;
    //}

    //Process PI_OUT --> variables sortant du CN
    setValues_Out_CN();

    for (int i = nbValuesCN_Out_ByCN; i < nbValuesCN_Out_ByCN + nbValuesCN_Out; i++)
    {
        if (activated_Out_CN_l[i + 1])
            pProcessImageOut_l->out_CN_array[i] = values_Out_CN_l[i];
    }

    ret = oplk_exchangeProcessImageIn();
    if (ret != kErrorOk)
        return ret;

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

    obdSize = 2;

    //Link image EC of the correct NODEID
    /*
    ret = linkPDO_in(varEntries, obdSize, 0, 0x6501, 0x01);
    if (ret != kErrorOk)
    {
        return ret;
    }
    */

    // Init process image output
    // Example : CN3 and 3 CNs --> from nbValuesCN_Out_ByCN = 75 / 3 * (3 - 1) = 50 to nbValuesCN_Out_ByCN + nbValuesCN_Out = 50 + 25 = 75
    for (int i = nbValuesCN_Out_ByCN; i <= nbValuesCN_Out_ByCN + nbValuesCN_Out; i++)
    {
        if (activated_Out_CN_l[i + 1])
        {
            //Link valves images
            if (i > nbValuesCN_Out_ByCN && i <= nbValuesCN_Out_ByCN + nbValuesCN_Out / 2)
            {
                ret = linkPDO_out(obdSize, i, 0x6500, 0x01 + i % (nbValuesCN_Out / 2));
            }
            //Link sensors images
            else if (i > nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 && i <= nbValuesCN_Out_ByCN + nbValuesCN_Out)
            {
                ret = linkPDO_out(obdSize, i, 0x6502, 0x01 + i % (nbValuesCN_Out / 2));
            }
            else if (i == nbValuesCN_Out_ByCN)
            {
                ret = linkPDO_out(obdSize, i, 0x6501, NODEID);
            }
            if (ret != kErrorOk)
            {
                return ret;
            }
        }
    }
    // Init process image input
    ret = linkPDO_in(obdSize, nbValuesCN_In_ByCN, 0x6511, 0xF0);
    if (ret != kErrorOk)
    {
        return ret;
    }

    for (int i = nbValuesCN_In_ByCN + 1; i < nbValuesCN_In_ByCN + nbValuesCN_In + 1; i++)
    {
        if (activated_Out_CN_l[i + nbValuesCN_In_ByCN + 1 - (NODEID - 1)])
        {
            //Link valves images in from MN
            ret = linkPDO_in(obdSize, i, 0x6510, 0x01 + i % nbValuesCN_In);
            if (ret != kErrorOk)
            {
                return ret;
            }
        }
    }


    //switch (mode)
    //{
    //case 0: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel
    //    for (int i = 1; i < SIZE_IN; i++)
    //    {
    //        if (i % (nbValuesCN_In + 1) == 0)
    //            skipSensorsOutFromIn += nbValuesCN_In + 1;
    //        else
    //            skipSensorsOutFromIn += 1;
    //        if (i % (nbValuesCN_In + 1) != 0 && activated_Out_CN_l[skipSensorsOutFromIn])
    //        {
    //            //Link valves images in from MN
    //            ret = linkPDO_in(obdSize, i, 0x6510, 0x01 + i % nbValuesCN_In);
    //            if (ret != kErrorOk)
    //            {
    //                return ret;
    //            }
    //        }
    //    }
    //    break;
    //case 1: // mode manuel : l'état des vannes proviennent directement du MN
    //    for (int i = nbValuesCN_In_ByCN + 1; i <= nbValuesCN_In_ByCN + nbValuesCN_In / 2; i++)
    //    {
    //        //Link valves images in from MN
    //        ret = linkPDO_in(obdSize, i, 0x6510, 0x01 + i % (nbValuesCN_In / 2));
    //        if (ret != kErrorOk)
    //        {
    //            return ret;
    //        }
    //    }
    //    break;
    //default:
    //    break;
    //}

    // Link image input EG


    fprintf(stderr, "Linking process vars... ok\n\n");

    return kErrorOk;
}


tOplkError linkPDO_in(tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex) {
    tOplkError  ret = kErrorOk;
    UINT varEntries = 1;
    size_t sizeElementArray = sizeof(int16_t) * arrayIndex;
    printf("linkPDO_in sizeof(int16_t) * arrayIndex:%u)= %" PRIu64 "\n", arrayIndex, sizeElementArray);
    ret = oplk_linkProcessImageObject(index,
        subIndex,
        sizeElementArray,
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

tOplkError linkPDO_out(tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex) {
    tOplkError  ret = kErrorOk;
    UINT varEntries = 1;
    size_t sizeElementArray = sizeof(int16_t) * arrayIndex;
    printf("linkPDO_out sizeof(int16_t) * arrayIndex:%d)= %" PRIu64 "\n", arrayIndex, sizeElementArray);
    ret = oplk_linkProcessImageObject(index,
        subIndex,
        sizeElementArray,
        TRUE,
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

int16_t extinctOPL()
{
    shutdownOplImage();
    shutdownPowerlink();
    system_exit();

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

