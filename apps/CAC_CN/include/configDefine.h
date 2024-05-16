#ifndef CONFIGOPL_H
#define CONFIGOPL_H

// OPL
#define CYCLE_LEN           10000
#define IP_ADDR             0xc0a801A6          // 192.168.1.166
#define DEFAULT_GATEWAY     0xC0A801FE          // 192.168.1.C_ADR_RT1_DEF_NODE_ID
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0

#define DEVNAME				"eth0"

// xapOpl
#define SIZE_OUT    (NB_NODES * 25)
#define SIZE_IN     (NB_NODES * 13)
#define COMPUTED_PI_OUT_SIZE   ((SIZE_OUT + 1) * 2)
#define COMPUTED_PI_IN_SIZE    ((SIZE_IN + 1) * 2)

// Sensor
#define IIOSYSPATH "/sys/bus/iio/devices/iio:device0/"
#define MODBUS_DEVICELOC "/dev/ttyUSB0"
#define MODBUS_NBREG 1
#define MAX_SENSORS 12
#define MAX_ADC 8
#define ADC_READ_ERROR -100000

// Valve
#define CHIP_PATH "/dev/gpiochip0"
#define MAX_VALVES 12

// CSV
#define MAX_PATH_LENGTH 1024
#define MAX_LINE_SIZE 2832
#define MAX_PHYSICAL_LINE_SIZE 512
#define MAX_ROWS 5

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
#define DELAYMSCONTROL 30

#endif