/*
* This file was autogenerated by openCONFIGURATOR-2.2.1_release on 28-Jan-2024 17:34:27
* Project: ASTREOS-1_OpenPOWERLINK
* Application process for OBC_MN(240)
*/
#ifndef XAP_h
#define XAP_h

#include <stdint.h>
#include "configDefine.h"

typedef struct
{
	int16_t out_MN_array[SIZE_OUT + 2 * NB_NODES];
	unsigned PADDING_VAR_1:16;
} PI_OUT;

typedef struct
{
	int16_t in_MN_array[SIZE_IN + 2 * NB_NODES];
	unsigned PADDING_VAR_1:16;
} PI_IN;

#endif
