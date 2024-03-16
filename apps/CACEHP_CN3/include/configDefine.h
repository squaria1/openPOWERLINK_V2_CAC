#ifndef CONFIGOPL_H
#define CONFIGOPL_H

// OPL
#define CYCLE_LEN           10000
#define NODEID              3                   // could be changed by command param
#define NB_NODES            3
#define IP_ADDR             0xc0a80121          // 192.168.1.33
#define DEFAULT_GATEWAY     0xC0A801FE          // 192.168.1.C_ADR_RT1_DEF_NODE_ID
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0

#define DEVNAME				"eth0"

// xapOpl
#define COMPUTED_PI_OUT_SIZE 152
#define SIZE_OUT 75
#define COMPUTED_PI_IN_SIZE 80
#define SIZE_IN 39

// Sensor
#define IIOSYSPATH "/sys/bus/iio/devices/iio:device0/"
#define MAX_SENSORS 8
#define ADC_READ_ERROR -100000

// Valve
#define CHIP_PATH "/dev/gpiochip0"
#define MAX_VALVES 12

// CSV
#define MAX_PATH_LENGTH 1024
#define MAX_LINE_SIZE 2832
#define MAX_PHYSICAL_LINE_SIZE 1024
#define MAX_ROWS 5
#define MAX_DEPENDANCE 2832

#ifdef _WIN32
#define STATE_CSV_DIRECTORY "..\\..\\..\\..\\common\\commonCSVFiles\\stateCSV\\"
#define PHYSICAL_CONFIG_VANNES_DIRECTORY "..\\..\\..\\include\\physicalCONFIG\\physicalCONFIG_valves.csv"
#define PHYSICAL_CONFIG_SENSORS_DIRECTORY "..\\..\\..\\include\\physicalCONFIG\\physicalCONFIG_sensors.csv"
#define COMMON_PHYSICAL_CONFIG_DIRECTORY "..\\..\\..\\..\\common\\commonCSVFiles\\activation.csv"
#define EG_ETAT_DIRECTORY "..\\..\\..\\..\\common\\commonCSVFiles\\liaisonEGEtat.csv"
// TelemFiles
#define TELEMFILES_DIRECTORY "..\\..\\..\\include\\telemFiles\\"
#else
#define STATE_CSV_DIRECTORY "../../../common/commonCSVFiles/stateCSV/"
#define PHYSICAL_CONFIG_VANNES_DIRECTORY "../../include/physicalCONFIG/physicalCONFIG_valves.csv"
#define PHYSICAL_CONFIG_SENSORS_DIRECTORY "../../include/physicalCONFIG/physicalCONFIG_sensors.csv"
#define COMMON_PHYSICAL_CONFIG_DIRECTORY "../../../common/commonCSVFiles/activation.csv"
#define EG_ETAT_DIRECTORY "../../../common/commonCSVFiles/liaisonEGEtat.csv"
// TelemFiles
#define TELEMFILES_DIRECTORY "../../include/telemFiles/"
#endif

// TelemFiles
#define EVENTLOG_MAX_LENGTH 1024

// Main
#define DELAYMSINIT 2000
#define DELAYMSCONTROL 10

#endif