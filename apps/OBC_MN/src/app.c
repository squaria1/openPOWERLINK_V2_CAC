/**
********************************************************************************
\file   app.c

\brief  Demo MN application which implements a running light

This file contains a demo application for digital input/output data.
It implements a running light on the digital outputs. The speed of
the running light is controlled by the read digital inputs.

\ingroup module_demo_mn_console
*******************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2017, B&R Industrial Automation GmbH
Copyright (c) 2013, SYSTEC electronik GmbH
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

#include <obdpi.h>
#include <eventlog/eventlog.h>
#include <stdint.h>

#include "xap.h"

//============================================================================//
//            G L O B A L   D E F I N I T I O N S                             //
//============================================================================//

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------
#define DEFAULT_MAX_CYCLE_COUNT 20      // 6 is very fast
#define APP_LED_COUNT_1         256       // number of LEDs for CN1
#define APP_LED_MASK_1          (1 << (APP_LED_COUNT_1 - 1))
#define MAX_NODES               255
#define MAX_VALUES              255

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
typedef struct
{
    UINT32                leds;
    UINT32                ledsOld;
    UINT32                input;
    UINT32                inputOld;
    UINT                period;
    int                 toggle;
} APP_NODE_VAR_T;

//------------------------------------------------------------------------------
// local vars
//------------------------------------------------------------------------------
static int              aUsedNodeIds_l[] = {1, 2, 3, 0};
static UINT             cnt_l;
static APP_NODE_VAR_T   aNodeVar_l[MAX_NODES];
static const UNION_OUT*  pProcessImageOut_l;
static UNION_IN*         pProcessImageIn_l;


static int32_t          arrOplIO_l[MAX_VALUES];

// Declare an array to store member names

//------------------------------------------------------------------------------
// local function prototypes
//------------------------------------------------------------------------------
static tOplkError       initProcessImage(void);

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

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
        arrOplIO_l[i] = 0;
    }

    memset(&pProcessImageOut_l, 0, sizeof(pProcessImageOut_l));
    memset(&pProcessImageIn_l, 0, sizeof(pProcessImageIn_l));
   
    ret = initProcessImage();

    return ret;
}

//------------------------------------------------------------------------------
/**
\brief  Shutdown the synchronous data application

The function shuts down the synchronous data application

\return The function returns a tOplkError error code.

\ingroup module_demo_mn_console
*/
//------------------------------------------------------------------------------
void shutdownApp(void)
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

    for (int i = 0; i < sizeof(pProcessImageOut_l->out_array) / sizeof(pProcessImageOut_l->out_array[0]); i++)
    {
        arrOplIO_l[i] = pProcessImageOut_l->out_array[i];
        printf("arropl at %d = %d", i, arrOplIO_l[i]);
    }

    cnt_l++;

    //aNodeVar_l[0].input = pProcessImageOut_l->memberNamesOUT[0];
    //aNodeVar_l[1].input = pProcessImageOut_l->CN2_Input_AI32_SENSOR1;
    //aNodeVar_l[2].input = pProcessImageOut_l->CN3_Input_AI32_SENSOR1;


   /* arrOplIO_l[2] = pProcessImageOut_l->out_array[0];
    arrOplIO_l[3] = pProcessImageOut_l->out_array[1];
    arrOplIO_l[4] = pProcessImageOut_l->out_array[2];
    arrOplIO_l[5] = pProcessImageOut_l->out_array[3];
    arrOplIO_l[6] = pProcessImageOut_l->out_array[4];
    arrOplIO_l[15] = pProcessImageOut_l->out_array[5];
    arrOplIO_l[16] = pProcessImageOut_l->out_array[6];
    arrOplIO_l[17] = pProcessImageOut_l->out_array[7];
    arrOplIO_l[18] = pProcessImageOut_l->out_array[8];
    arrOplIO_l[19] = pProcessImageOut_l->out_array[9];

    arrOplIO_l[27] = pProcessImageOut_l->out_array[10];
    arrOplIO_l[28] = pProcessImageOut_l->out_array[11];
    arrOplIO_l[29] = pProcessImageOut_l->out_array[12];
    arrOplIO_l[40] = pProcessImageOut_l->out_array[14];
    arrOplIO_l[41] = pProcessImageOut_l->out_array[15];
    arrOplIO_l[42] = pProcessImageOut_l->out_array[16];
    arrOplIO_l[43] = pProcessImageOut_l->out_array[17];
    arrOplIO_l[44] = pProcessImageOut_l->out_array[18];

    arrOplIO_l[52] = pProcessImageOut_l->out_array[19];
    arrOplIO_l[53] = pProcessImageOut_l->out_array[20];
    arrOplIO_l[54] = pProcessImageOut_l->out_array[21];
    arrOplIO_l[55] = pProcessImageOut_l->out_array[22];
    arrOplIO_l[56] = pProcessImageOut_l->out_array[23];
    arrOplIO_l[65] = pProcessImageOut_l->out_array[23];
    arrOplIO_l[66] = pProcessImageOut_l->out_array[24];
    arrOplIO_l[67] = pProcessImageOut_l->out_array[25];
    arrOplIO_l[68] = pProcessImageOut_l->out_array[26];
    arrOplIO_l[69] = pProcessImageOut_l->out_array[27];*/


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

    //pProcessImageIn_l->CN1_Output_AI16_EG = aNodeVar_l[0].leds;
    //pProcessImageIn_l->CN2_Output_AI16_EG = aNodeVar_l[1].leds;
    //pProcessImageIn_l->CN3_Output_AI16_EG = aNodeVar_l[2].leds;

    //pProcessImageIn_l->CN1_Output_AI8_VALVE1 = arrOplIO_l[3];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE2 = arrOplIO_l[4];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE3 = arrOplIO_l[5];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE4 = arrOplIO_l[6];

    //pProcessImageIn_l->CN1_Output_AI8_VALVE1 = arrOplIO_l[28];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE2 = arrOplIO_l[29];

    //pProcessImageIn_l->CN1_Output_AI8_VALVE1 = arrOplIO_l[53];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE2 = arrOplIO_l[54];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE3 = arrOplIO_l[55];
    //pProcessImageIn_l->CN1_Output_AI8_VALVE4 = arrOplIO_l[56];

    //pProcessImageIn_l->CN1_Output_AI16_EG = arrOplIO_l[1];
    //pProcessImageIn_l->CN2_Output_AI16_EG = arrOplIO_l[1];
    //pProcessImageIn_l->CN3_Output_AI16_EG = arrOplIO_l[1];

    ret = oplk_exchangeProcessImageIn();

    return ret;
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

    pProcessImageIn_l = (UNION_IN*)oplk_getProcessImageIn();
    pProcessImageOut_l = (const UNION_OUT*)oplk_getProcessImageOut();

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

void setArrOplIO_l(int32_t arrOplIO_g[])
{
    for (int i = 0; i < MAX_VALUES; i++)
    {
        arrOplIO_l[i] = arrOplIO_g[i];
    }
}

int32_t* getArrOplIO_l()
{
    return arrOplIO_l;
}
