#ifndef CONFIGOPL_H
#define CONFIGOPL_H

// OPL
#define CYCLE_LEN           10000
#define NODEID              1                   // could be changed by command param
#define NB_NODES            3
#define IP_ADDR             0xc0a80142          // 192.168.100.1
#define DEFAULT_GATEWAY     0xC0A801FE          // 192.168.100.C_ADR_RT1_DEF_NODE_ID
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0

// xapOpl
#define COMPUTED_PI_OUT_SIZE 152
#define SIZE_OUT 75
#define COMPUTED_PI_IN_SIZE 80
#define SIZE_IN 39

// Sensor
char iiosyspath[] = "/sys/bus/iio/devices/iio:device0/";
#define MAX_SENSORS 8

// Valve
#define CHIP_PATH "/dev/gpiochip0"
#define MAX_VALVES 12

// CSV
#define MAX_LINE_SIZE 2832
#define MAX_FIELD_SIZE 2832
#define MAX_ROWS 5
#define MAX_DEPENDANCE 2832
#define STATE_CSV_DIRECTORY "..\\..\\..\\include\\stateCSV\\Etat_.csv"
#define PHYSICAL_CONFIG_VANNES_DIRECTORY "..\\..\\..\\include\\physicalCONFIG\\physicalCONFIG_vannes.csv"
#define PHYSICAL_CONFIG_SENSORS_DIRECTORY "..\\..\\..\\include\\physicalCONFIG\\physicalCONFIG_sensors.csv"
#define COMMON_PHYSICAL_CONFIG_DIRECTORY "..\\..\\..\\..\\common\\commonPhysicalCONFIG\\commonPhysicalCONFIG.csv"


#endif