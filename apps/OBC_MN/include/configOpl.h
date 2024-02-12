#ifndef CONFIGOPL_H
#define CONFIGOPL_H

#define CYCLE_LEN           UINT_MAX
#define NODEID              0xF0                //=> MN
#define IP_ADDR             0xc0a801F0          // 192.168.100.1
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0
#define DEFAULT_GATEWAY     0xC0A864FE          // 192.168.100.C_ADR_RT1_DEF_NODE_ID

#define DEFAULT_MAX_CYCLE_COUNT 20      // 6 is very fast
#define APP_LED_COUNT_1         8       // number of LEDs for CN1
#define APP_LED_MASK_1          (1 << (APP_LED_COUNT_1 - 1))
#define MAX_NODES               255
#define MAX_VALUES              255

#define COMPUTED_PI_OUT_SIZE 152
#define SIZE_OUT 75
#define COMPUTED_PI_IN_SIZE 80
#define SIZE_IN 39

#endif