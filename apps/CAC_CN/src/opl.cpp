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

int16_t              EG = 1;
int16_t              EC = EG;
Mode                 mode = automatic;
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

/**
 * \brief function to check if a stop order has been
 * received from the master node (MN)
 * \return statusErrDef 
 * infoStopOrderReceived if a stop order has been received
 * or infoNoStopOrder if no stop order has been received
 */
statusErrDef opl::demandeExtinctOPL()
{
    if (values_In_CN_l[nbValuesCN_Out_ByCN] == infoStopOrderReceived)
        return infoStopOrderReceived;
    else
        return infoNoStopOrder;

}

/**
 * \brief function to send the current status code
 * to the MN in the board state (EC) object
 * 
 * \param statusCode the current status code
 */
void opl::sendTelem(int16_t statusCode)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = statusCode;
}

/**
 * \brief function to send the current error code
 * to the MN in the board state (EC) object
 * 
 * \param errorCode the current error code
 */
void opl::sendError(int16_t errorCode)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = errorCode;
}

/**
 * \brief function to display in the console
 * the values coming from the MN
 * 
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

/**
 * \brief function to display in the console
 * the values coming from the MN
 * 
 */
void affValeursProcessIn()
{
    printf("\n-------------PROCESS IN CN--------------\n");
    for (int i = 0; i < SIZE_IN; i++)
    {
        printf("pProcessImageIn_l->in_CN_array[%d]=%d\n", i, pProcessImageIn_l->in_CN_array[i]);
    }
    printf("\n----------------------------------------\n");
}

/**
 * \brief function to display in the console
 * the values coming out of the CN
 * 
 */
void affValeursOut()
{
    printf("\n------------OUT CN--------------\n");
    for (int i = nbValuesCN_Out_ByCN; i < nbValuesCN_Out_ByCN + nbValuesCN_Out + 1; i++)
    {
        printf("values_Out_CN_l[%d]=%d\n", i, values_Out_CN_l[i]);
    }
    printf("\n--------------------------------\n");
}

/**
 * \brief function getter of the value
 * coming from the MN from line according to the structure PI_IN
 * 
 * \param ligne the line according to the structure PI_IN
 * \return int16_t the value coming from the MN
 */
int16_t getValues_In_CN(int ligne)
{
    return values_In_CN_l[ligne];
}

/**
 * \brief function setter of the values
 * coming out of the CN namely, the valves and sensors values.
 * 
 */
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

/**
 * \brief function getter of the value
 * coming out of the CN from line according to the structure PI_OUT
 * 
 * \param ligne the line according to the structure PI_OUT
 * \return int16_t the value coming out of the CN
 */
int16_t getValues_Out_CN(int ligne)
{
    return values_Out_CN_l[ligne];
}

/**
 * \brief function setter to fill the activation array
 * from the CSV activation file "activation.csv"
 * 
 */
void setActivated_Out_CN()
{
    for (int i = 0; i < SIZE_OUT + 1; i++)
    {
        activated_Out_CN_l[i] = getActivation(i);
    }
}

/**
 * \brief function getter of the current general state (EG)
 * 
 * \return int16_t the current general state (EG)
 */
int16_t getEG()
{
    printf("\n\n EG CN truc : %d \n\n", values_In_CN_l[nbValuesCN_In_ByCN]);
    return values_In_CN_l[nbValuesCN_In_ByCN];
}

/**
 * \brief function to check if the general state (EG)
 * has been changed
 * 
 * \return statusErrDef 
 * infoModeSetToManual if the mode to manual order has been received from the MN
 * or infoEGNotChanged if the EG is still the same.
 */
statusErrDef isEGchanged()
{
    if (values_In_CN_l[nbValuesCN_In_ByCN] != EG && values_In_CN_l[nbValuesCN_In_ByCN] != 0)
    {
        EG = values_In_CN_l[nbValuesCN_In_ByCN];
        EC = EG;
        statusErrDef res = resetTimers();
        if (values_In_CN_l[nbValuesCN_In_ByCN] == infoStateToManualMode)
        {
            res = infoModeSetToManual;
            mode = manual;
        }
        else
            mode = automatic;
        return res;
    }
    else
        return infoEGNotChanged;

}

/**
 * \brief function setter of the board state (EC)
 * 
 * \param EC the board state (EC)
 */
void setEC1(int16_t EC)
{
    values_Out_CN_l[nbValuesCN_Out_ByCN] = EC;
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
 * or errSelNetInterface when the selection
 * of the network interface fails when
 * in a Windows machine. Can be caused by the absence of WinPcap.
 * or errInitObjDictionary when the object dictionary header file (objdict.h)
 * has incorrect values or syntax.
 * or errOplkInit when The OpenPOWERLINK stack fails to initialize, 
 * main cause: the stack is not found by the application, check the CN .lib files
 * or errOplkCreate when the OpenPOWERLINK stack fails to create a new instance
 * or errOplkAllocProcessImage when the allocation of the input
 * and/or output structure fails because parts of the structure 
 * doesn't exist in the objdict.h file
 * or errLinkPDOout when an output object (TPDO) 
 * doesn't exist for the same reasons above
 * or errLinkPDOin when an input object (RPDO) 
 * doesn't exist for the same reasons above
 * or errSendNMTResetCommand when the OpenPOWERLINK reset command fails
 * or noError when the function exits successfully. 
 */
statusErrDef initOPL()
{
    statusErrDef    res = noError;
    tOptions        opts;

    strncpy(opts.devName, DEVNAME, 128);
    opts.nodeId = NODEID;
    opts.logFormat = kEventlogFormatReadable;
    opts.logCategory = 0xffffffff;
    opts.logLevel = 0xffffffff;

    if (system_init() != 0)
    {
        fprintf(stderr, "Error initializing system!");
        return errOPLSystemInit;
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

    res = initPowerlink(CYCLE_LEN,
        opts.devName,
        aMacAddr_l,
        opts.nodeId);
    if (res != noError)
        return res;

    res = initProcessImage();
    if (res != noError)
        return res;

    res = initOplThread();
    if (res != noError)
        return res;

    return noError;
}


/**
\brief  Initialize the openPOWERLINK stack

The function initializes the openPOWERLINK stack.

\param[in]      cycleLen_p          Length of POWERLINK cycle.
\param[in]      devName_p           Device name string.
\param[in]      macAddr_p           MAC address to use for POWERLINK interface.
\param[in]      nodeId_p            POWERLINK node ID.

\return statusErrDef that values errSelNetInterface
when the selection of the network interface fails when
in a Windows machine can be caused by the absence of WinPcap.
or errInitObjDictionary when the object dictionary header file (objdict.h)
has incorrect values or syntax.
or errOplkInit when the OpenPOWERLINK stack fails maybe because the stack is
not found by the application, check the CN .lib files.
or errOplkCreate when the OpenPOWERLINK stack fails to create a new instance
or noError when the function exits successfully. 
*/
statusErrDef initPowerlink(UINT32 cycleLen_p,
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


    #if (TARGET_SYSTEM == _WIN32_)
        if (netselect_selectNetworkInterface(devName, sizeof(devName)) < 0)
            return errSelNetInterface;
    #else
        strncpy(devName, devName_p, 128);
    #endif


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
            "oplk_initialize() failed with \"%s\" (0x%04x)\n",
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
        eventlog_printMessage(kEventlogLevelFatal,
            kEventlogCategoryControl,
            "oplk_create() failed with \"%s\" (0x%04x)\n",
            debugstr_getRetValStr(ret),
            ret);
        return errOplkCreate;
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
statusErrDef initOplThread(void)
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
        return errSendNMTResetCommand;
    }

    setupInputs();

    processSync();

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
\brief  Synchronous data handler

The function implements the synchronous data handler.

\return The function returns a tOplkError error code.

\ingroup module_demo_mn_console
*/
tOplkError processSync()
{
    tOplkError  res = kErrorOk;

    if (oplk_waitSyncEvent(100000) != kErrorOk)
        return res;


    res = oplk_exchangeProcessImageOut();
    if (res != kErrorOk)
        return res;


    //Process PI_IN --> variables entrant dans le CN
    values_In_CN_l[nbValuesCN_In_ByCN] = pProcessImageIn_l->in_CN_array[nbValuesCN_In_ByCN];

    int skipSensorsOutFromIn = 1;
    // Example : CN3 and 3 CNs --> from nbValuesCN_Out_ByCN = 75 / 3 * (3 - 1) = 50 to nbValuesCN_Out_ByCN + nbValuesCN_Out = 50 + 25 = 75
    switch (mode)
    {
    case automatic: // mode automatique : lecture de l'état des vannes depuis le CSV de l'etat general actuel
        break;
    case manual: // mode manuel : l'état des vannes proviennent directement du MN
        for (int i = 1; i < SIZE_IN; i++)
        {
            if (i % (nbValuesCN_In + 1) == 0)
                skipSensorsOutFromIn += nbValuesCN_In + 1;
            else
                skipSensorsOutFromIn += 1;
            if (i % (nbValuesCN_In + 1) != 0 && activated_Out_CN_l[skipSensorsOutFromIn])
                values_In_CN_l[i] = pProcessImageIn_l->in_CN_array[i];
        }
        break;
    default:
        break;
    }

    //Process PI_OUT --> variables sortant du CN
    setValues_Out_CN();

    for (int i = nbValuesCN_Out_ByCN; i < nbValuesCN_Out_ByCN + nbValuesCN_Out + 1; i++)
    {
        if (activated_Out_CN_l[i + 1])
            pProcessImageOut_l->out_CN_array[i] = values_Out_CN_l[i];
    }

    res = oplk_exchangeProcessImageIn();
    if (res != kErrorOk)
        return res;

    return res;
}

/**
\brief  Setup inputs

The function initializes the digital input port.

\ingroup module_demo_cn_console
*/
void setupInputs(void)
{
    memset(&values_In_CN_l, 0, sizeof(values_In_CN_l));
}

/**
\brief  Initialize process image

The function initializes the process image of the application.

\return statusErrDef that values errOplkAllocProcessImage
when the allocation of the input and/or output structure 
doesn't exist in the objdict.h file
or errLinkPDOout when an output object (TPDO) 
doesn't exist for the same reasons above.
or errLinkPDOin when an input object (RPDO) 
doesn't exist for the same reasons above.
or noError when the function exits successfully.
*/
statusErrDef initProcessImage(void)
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
        return errOplkAllocProcessImage;

    pProcessImageIn_l = (const PI_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (PI_OUT*)oplk_getProcessImageOut();

    /* link process variables used by CN to object dictionary */
    fprintf(stderr, "Linking process image vars:\n");

    obdSize = 2;

    //Link image EC of the correct NODEID

    // Init process image output
    // Example : CN3 and 3 CNs --> from nbValuesCN_Out_ByCN = 75 / 3 * (3 - 1) = 50 to nbValuesCN_Out_ByCN + nbValuesCN_Out = 50 + 25 = 75
    for (int i = nbValuesCN_Out_ByCN; i <= nbValuesCN_Out_ByCN + nbValuesCN_Out + 1; i++)
    {
        if (activated_Out_CN_l[i + 1])
        {
            //Link valves images
            if (i > nbValuesCN_Out_ByCN && i <= nbValuesCN_Out_ByCN + nbValuesCN_Out / 2)
                ret = linkPDO_out(obdSize, i, 0x6500, 0x00 + (i % (nbValuesCN_Out+1)));
            //Link sensors images
            else if (i > nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 && i <= nbValuesCN_Out_ByCN + nbValuesCN_Out)
                ret = linkPDO_out(obdSize, i, 0x6502, 0x01 + ((i % (nbValuesCN_Out+1)) % (nbValuesCN_In+1)));
            //Link the board state
            else if (i == nbValuesCN_Out_ByCN)
                ret = linkPDO_out(obdSize, i, 0x6501, NODEID);
            if (ret != kErrorOk)
                return errLinkPDOout;
        }
    }

    // Init process image input

    for (int i = nbValuesCN_In_ByCN; i < nbValuesCN_In_ByCN + nbValuesCN_In + 1; i++)
    {
        if (activated_Out_CN_l[i + nbValuesCN_In_ByCN + 2 - NODEID])
        {
            //Link valves images in from the MN
            if(i > nbValuesCN_In_ByCN && i <= nbValuesCN_In_ByCN + nbValuesCN_In)
                ret = linkPDO_in(obdSize, i, 0x6510, 0x00 + (i % (nbValuesCN_In+1)));
            //Link the incoming general state from the MN
            else if(i == nbValuesCN_In_ByCN)
                ret = linkPDO_in(obdSize, nbValuesCN_In_ByCN, 0x6511, 0xF0);
            if (ret != kErrorOk)
                return errLinkPDOin;
        }
    }

    fprintf(stderr, "Linking process vars... ok\n\n");

    return noError;
}

/**
 * \brief function to link a CN RPDO (recieved process data object)
 * gotten from the mnobd.cdc binary file generated by
 * OpenCONFIGURATOR to the OpenPOWERLINK application.
 * 
 * \param obdSize the size of the PDO in bytes
 * \param arrayIndex the index of the CN RPDO array
 * \param index the index of the CN RPDO in the
 * OpenPOWERLINK object library gotten from this file:
 * 00000000_POWERLINK_CiA401_CN.xdd
 * \param subIndex the sub index of the specific object 
 * to be linked gotten from this file: 
 * 00000000_POWERLINK_CiA401_CN.xdd
 * 
 * \return The function returns a tOplkError error code.
 */
tOplkError linkPDO_in(tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex) {
    tOplkError  ret = kErrorOk;
    UINT varEntries = 1;
    size_t sizeElementArray = sizeof(int16_t) * arrayIndex;
    printf("linkPDO_in sizeof(int16_t) * arrayIndex:%u)= %" PRIu64 "\n", arrayIndex, sizeElementArray);    
    printf("subIndex : %d \n", subIndex);
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
        return ret;
    }

    return ret;
}

/**
 * \brief function to link a CN TPDO (transmitted process data object)
 * gotten from the mnobd.cdc binary file generated by
 * OpenCONFIGURATOR to the OpenPOWERLINK application.
 * 
 * \param obdSize the size of the PDO in bytes
 * \param arrayIndex the index of the CN TPDO array
 * \param index the index of the CN RPDO in the
 * OpenPOWERLINK object library gotten from this file:
 * 00000000_POWERLINK_CiA401_CN.xdd
 * \param subIndex the sub index of the specific object 
 * to be linked gotten from this file: 
 * 00000000_POWERLINK_CiA401_CN.xdd
 * 
 * \return The function returns a tOplkError error code.
 */
tOplkError linkPDO_out(tObdSize obdSize, UINT16 arrayIndex, UINT16 index, UINT8 subIndex) {
    tOplkError  ret = kErrorOk;
    UINT varEntries = 1;
    size_t sizeElementArray = sizeof(int16_t) * arrayIndex;
    printf("linkPDO_out sizeof(int16_t) * arrayIndex:%d)= %" PRIu64 "\n", arrayIndex, sizeElementArray);
    printf("subIndex : %d \n", subIndex);
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
        return ret;
    }

    return ret;
}

/**
 * \brief function to shutdown the OpenPOWERLINK module.
 * 
 * \return statusErrDef that values errOplkFreeProcessImage
 * when the freeing of memory of the OpenPOWERLINK stack fails.
 * or noError when the function exits successfully.
 */
statusErrDef extinctOPL()
{
    statusErrDef res = noError;

    res = shutdownOplImage();
    shutdownPowerlink();
    system_exit();

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
statusErrDef shutdownOplImage(void)
{
    tOplkError ret = kErrorOk;

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
    UINT    i;
    tOplkError  ret = kErrorOk;

    fGsOff_l = FALSE;

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