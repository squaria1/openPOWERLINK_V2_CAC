/**
 * \file configDefine.h
 * \brief header file that contains all the program defines
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all of the program defines from each module.
 * The defines are set at compile time so they can't be changed
 * at runtime.
 */

#ifndef CONFIGOPL_H
#define CONFIGOPL_H

#include "nbNodes.h"

// OPL
/**
 * \brief OpenPOWERLINK cycle length,
 * not shown in the MN but in the CNs.
 */
#define CYCLE_LEN           UINT_MAX
/**
 * \brief the MN node ID (240 by default)
 */
#define NODEID              0xF0                //=> MN
/**
 * \brief The MN default ip address (not important)
 */
#define IP_ADDR             0xc0a801F0          // 192.168.100.1
/**
 * \brief Network mask of /24
 */
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0
/**
 * \brief The MN default gateway (not important)
 */
#define DEFAULT_GATEWAY     0xC0A864FE          // 192.168.100.C_ADR_RT1_DEF_NODE_ID
/**
 * \brief The maximum number of MN and CN
 */
#define MAX_NODES               255
/**
 * \brief The MN device name (look in /etc/network/interfaces for the name in Linux)
 */
#define DEVNAME					"eth0"
/**
 * \brief The OpenPOWERLINK binary file name containing all RPDO and TPDO 
 * structures between the MN and the CNs. This file is generated by OpenCONFIGURATOR.
 */
#define CDCFILE					"mnobd.cdc"

// xapOpl
/**
 * \brief Total size of RPDOs coming into every CNs
 */
#define SIZE_OUT    (NB_NODES * 25)
/**
 * \brief Total size of TPDOs coming out of every CNs
 */
#define SIZE_IN     (NB_NODES * 13)
/**
 * \brief Total size in bytes for every RPDOs (int16_t is 2 bytes)
 */
#define COMPUTED_PI_OUT_SIZE   ((SIZE_OUT + 1) * 2)
/**
 * \brief Total size in bytes for every TPDOs (int16_t is 2 bytes)
 */
#define COMPUTED_PI_IN_SIZE    ((SIZE_IN + 1) * 2)

// CSV
/**
 * \brief maximum path length for a general state CSV file
 */
#define MAX_PATH_LENGTH 1024
/**
 * \brief maximum line size for every common CSV file
 */
#define MAX_LINE_SIZE 2832
/**
 * \brief maximum line size for every physical CSV file
 */

/**
 * \brief relative directory and file location used by the program
 */
#ifdef _WIN32
#define COMMON_PHYSICAL_CONFIG_DIRECTORY "..\\..\\..\\..\\common\\commonCSVFiles\\activation.csv"
#define EG_ETAT_DIRECTORY "..\\..\\..\\..\\common\\commonCSVFiles\\liaisonEGEtat.csv"
// TelemFiles
#define TELEMFILES_DIRECTORY "..\\..\\..\\include\\telemFiles\\"
#else
#define COMMON_PHYSICAL_CONFIG_DIRECTORY "../../../common/commonCSVFiles/activation.csv"
#define EG_ETAT_DIRECTORY "../../../common/commonCSVFiles/liaisonEGEtat.csv"
// TelemFiles
#define TELEMFILES_DIRECTORY "../../include/telemFiles/"
#endif

// TelemFiles
/**
 * \brief maximum length of telemetry status or error message
 */
#define EVENTLOG_MAX_LENGTH 1024

// Main
/**
 * \brief delay in milliseconds at the end of the initialisation state
 * to wait for the CN to connect to the MN.
 */
#define DELAYMSINIT 2000
/**
 * \brief delay in milliseconds at the end of the controlAndAcquisition state
 * to avoid dropping the connexion between the CN and the MN.
 */
#define DELAYMSCONTROL 1

#endif