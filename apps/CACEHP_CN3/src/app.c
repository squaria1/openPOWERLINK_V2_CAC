/**
********************************************************************************
\file   app.c

\brief  Demo CN application which implements a digital input/output node

This file contains a demo application for digital input/output data.

\ingroup module_demo_cn_console
*******************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2016, B&R Industrial Automation GmbH
Copyright (c) 2013, SYSTEC electronic GmbH
Copyright (c) 2013, Kalycito Infotech Private Ltd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "app.h"

#include <oplk/oplk.h>
#include <oplk/debugstr.h>
#include <eventlog/eventlog.h>
#include "xap.h"

#include <stddef.h>
#include <stdio.h>

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// module global vars
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// global function prototypes
//------------------------------------------------------------------------------


//============================================================================//
//            P R I V A T E   D E F I N I T I O N S                           //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local types
//------------------------------------------------------------------------------
/* structure for input process image */
//typedef struct
//{
//   UINT8                digitalIn;
//} PI_IN;

/* structure for output process image */
//typedef struct
//{
//   UINT8                digitalOut;
//} PI_OUT;

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
/* process image */
static PI_IN*           pProcessImageIn_l;
static const PI_OUT*    pProcessImageOut_l;
static PI_IN            AppProcessImageIn_g;
static PI_OUT           AppProcessImageOut_g;

/* application variables */
static UINT16            EC_l;            // 8 bit digital input
static UINT32            MEP_TP04_l;           // 8 bit digital output

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static tOplkError   initProcessImage(void);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

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
\brief  Shutdown the synchronous data application

The function shuts down the synchronous data application

\return The function returns a tOplkError error code.

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
void shutdownApp(void)
{
    oplk_freeProcessImage();
}

//------------------------------------------------------------------------------
/**
\brief  Synchronous data handler

The function implements the synchronous data handler.

\return The function returns a tOplkError error code.

\ingroup module_demo_cn_console
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
    EC_l = pProcessImageOut_l->CN3_AnalogueOutput_00h_AI16_EC;

    /* setup output image - digital inputs */
    pProcessImageIn_l->CN3_AnalogueInput_00h_AI32_MEP_TP04 = MEP_TP04_l;

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
    MEP_TP04_l = 1;
}

//------------------------------------------------------------------------------
/**
\brief  Increase inputs

The function changes the digital input port by shifting the set bit to the
left (increase the value).

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
void printEC(void)
{
    //if (digitalIn_l == 128)
    //    digitalIn_l = 1;
    //else
    //    digitalIn_l = digitalIn_l << 1;

    //printf("\b \b");
    //printInputs();

    printf("Valeur input EC: %d\n", EC_l);
}

//------------------------------------------------------------------------------
/**
\brief  Decrease inputs

The function changes the digital input port by shifting the set bit to the
right (decrease the value).

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
void decreaseMEP_TP04_l(void)
{
    if (MEP_TP04_l == 1)
        MEP_TP04_l = 2147483648;
    else
        MEP_TP04_l = MEP_TP04_l >> 1;

    printf("\b \b");
    printInputs();
}


//------------------------------------------------------------------------------
/**
\brief  Print outputs

The function prints the value of the digital output port on the console.

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
void printMEP_TP04_l(void)
{
    //int i;

    //printf("\b \b");
    //printf("Digital outputs: ");
    //for (i = 0; i < 8; i++)
    //{
    //    if (((digitalOut_l >> i) & 1) == 1)
    //        printf("*");
    //    else
    //        printf("-");
    //}
    //printf("\n");

    printf("Valeur output MEP_TP01: %d\n", MEP_TP04_l);
}

//------------------------------------------------------------------------------
/**
\brief  Print inputs

The function prints the value of the digital input port on the console.

\ingroup module_demo_cn_console
*/
//------------------------------------------------------------------------------
void printInputs(void)
{
    int i;

    printf("Digital inputs: ");
    for (i = 0; i < 32; i++)
    {
        if (((MEP_TP04_l >> i) & 1) == 1)
            printf("*");
        else
            printf("-");
    }
    printf("\n");
}

//============================================================================//
//            P R I V A T E   F U N C T I O N S                               //
//============================================================================//
/// \name Private Functions
/// \{

//------------------------------------------------------------------------------
/**
\brief  Initialize process image

The function initializes the process image of the application.

\return The function returns a tOplkError error code.
*/
//------------------------------------------------------------------------------
static tOplkError initProcessImage(void)
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

    pProcessImageIn_l = (PI_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (const PI_OUT*)oplk_getProcessImageOut();

    /* link process variables used by CN to object dictionary */
    fprintf(stderr, "Linking process image vars:\n");

    obdSize = sizeof(pProcessImageIn_l->CN3_AnalogueInput_00h_AI32_MEP_TP04);
    varEntries = 1;
    ret = oplk_linkProcessImageObject(0x6402,
                                      0x0C,
                                      offsetof(PI_IN, CN3_AnalogueInput_00h_AI32_MEP_TP04),
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

    obdSize = sizeof(pProcessImageOut_l->CN3_AnalogueOutput_00h_AI16_EC);
    varEntries = 1;
    ret = oplk_linkProcessImageObject(0x6411,
                                      0x03,
                                      offsetof(PI_OUT, CN3_AnalogueOutput_00h_AI16_EC),
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

    fprintf(stderr, "Linking process vars... ok\n\n");

    return kErrorOk;
}

/// \}
