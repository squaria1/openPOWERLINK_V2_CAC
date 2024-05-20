/**
 * \file xapOpl.h
 * \brief header file generated by openCONFIGURATOR and then modified
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains the modified OpenPOWERLINK RPDOs (PI_IN) and TPDOs (PI_OUT)
 * structures to process the OpenPOWERLINK object images.
 * 
 * The original autogenerated by openCONFIGURATOR structures contained
 * hundreds of struct members only for 6 CNs. To allow iteration inside those
 * structures and flexibility with the number of CNs, they have been replaced 
 * by two arrays switching all the previous struct members into array values.
 */

/*
* This file was autogenerated by openCONFIGURATOR-2.2.1_release on 28-Jan-2024 17:34:27
* Project: ASTREOS-1_OpenPOWERLINK
* Application process for OBC_MN(240)
*/

#ifndef XAP_h
#define XAP_h

#include <stdint.h>
#include "configDefine.h"
#include "nodeId.h"

/**
 * \brief PI_OUT contains the TPDOs (Transmitted Process Data Object)
 * so the OpenPOWERLINK values coming out of the CN (Controlled Node) to the MN
 */
typedef struct
{
	int16_t out_CN_array[SIZE_OUT];			/**< the array of OpenPOWERLINK values coming out of the CN */
} PI_OUT;

/**
 * \brief PI_IN contains the RPDOs (Received Process Data Object)
 * so the OpenPOWERLINK values coming from the MN (Master Node) to the CN
 */
typedef struct
{
	int16_t in_CN_array[SIZE_IN];			/**< the array of OpenPOWERLINK values coming from the MN */
} PI_IN;

#endif
