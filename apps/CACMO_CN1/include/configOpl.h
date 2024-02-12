#ifndef CONFIGOPL_H
#define CONFIGOPL_H

#define CYCLE_LEN           10000
#define NODEID              1                   // could be changed by command param
#define NB_NODES            3
#define IP_ADDR             0xc0a80142          // 192.168.100.1
#define DEFAULT_GATEWAY     0xC0A801FE          // 192.168.100.C_ADR_RT1_DEF_NODE_ID
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0
#define MAX_SENSORS 8
#define MAX_VALVES 12

#define COMPUTED_PI_OUT_SIZE 152
#define SIZE_OUT 75
#define COMPUTED_PI_IN_SIZE 80
#define SIZE_IN 39

#endif