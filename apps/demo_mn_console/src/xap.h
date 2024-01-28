/*
* This file was autogenerated by openCONFIGURATOR-2.2.1_release on 28-Jan-2024 17:34:27
* Project: ASTREOS-1_OpenPOWERLINK
* Application process for OBC_MN(240)
*/
#ifndef XAP_h
#define XAP_h

#define COMPUTED_PI_OUT_SIZE 152
typedef struct
{
	signed CN1_EC_In_AI16_EC1:16;
	signed CN1_VALVE_In_AI16_VALVE1:16;
	signed CN1_VALVE_In_AI16_VALVE2:16;
	signed CN1_VALVE_In_AI16_VALVE3:16;
	signed CN1_VALVE_In_AI16_VALVE4:16;
	signed CN1_VALVE_In_AI16_VALVE5:16;
	signed CN1_VALVE_In_AI16_VALVE6:16;
	signed CN1_VALVE_In_AI16_VALVE7:16;
	signed CN1_VALVE_In_AI16_VALVE8:16;
	signed CN1_VALVE_In_AI16_VALVE9:16;
	signed CN1_VALVE_In_AI16_VALVE10:16;
	signed CN1_VALVE_In_AI16_VALVE11:16;
	signed CN1_VALVE_In_AI16_VALVE12:16;
	signed CN1_SENSOR_In_AI16_SENSOR1:16;
	signed CN1_SENSOR_In_AI16_SENSOR2:16;
	signed CN1_SENSOR_In_AI16_SENSOR3:16;
	signed CN1_SENSOR_In_AI16_SENSOR4:16;
	signed CN1_SENSOR_In_AI16_SENSOR5:16;
	signed CN1_SENSOR_In_AI16_SENSOR6:16;
	signed CN1_SENSOR_In_AI16_SENSOR7:16;
	signed CN1_SENSOR_In_AI16_SENSOR8:16;
	signed CN1_SENSOR_In_AI16_SENSOR9:16;
	signed CN1_SENSOR_In_AI16_SENSOR10:16;
	signed CN1_SENSOR_In_AI16_SENSOR11:16;
	signed CN1_SENSOR_In_AI16_SENSOR12:16;
	signed CN2_EC_In_AI16_EC2:16;
	signed CN2_VALVE_In_AI16_VALVE1:16;
	signed CN2_VALVE_In_AI16_VALVE2:16;
	signed CN2_VALVE_In_AI16_VALVE3:16;
	signed CN2_VALVE_In_AI16_VALVE4:16;
	signed CN2_VALVE_In_AI16_VALVE5:16;
	signed CN2_VALVE_In_AI16_VALVE6:16;
	signed CN2_VALVE_In_AI16_VALVE7:16;
	signed CN2_VALVE_In_AI16_VALVE8:16;
	signed CN2_VALVE_In_AI16_VALVE9:16;
	signed CN2_VALVE_In_AI16_VALVE10:16;
	signed CN2_VALVE_In_AI16_VALVE11:16;
	signed CN2_VALVE_In_AI16_VALVE12:16;
	signed CN2_SENSOR_In_AI16_SENSOR1:16;
	signed CN2_SENSOR_In_AI16_SENSOR2:16;
	signed CN2_SENSOR_In_AI16_SENSOR3:16;
	signed CN2_SENSOR_In_AI16_SENSOR4:16;
	signed CN2_SENSOR_In_AI16_SENSOR5:16;
	signed CN2_SENSOR_In_AI16_SENSOR6:16;
	signed CN2_SENSOR_In_AI16_SENSOR7:16;
	signed CN2_SENSOR_In_AI16_SENSOR8:16;
	signed CN2_SENSOR_In_AI16_SENSOR9:16;
	signed CN2_SENSOR_In_AI16_SENSOR10:16;
	signed CN2_SENSOR_In_AI16_SENSOR11:16;
	signed CN2_SENSOR_In_AI16_SENSOR12:16;
	signed CN3_EC_In_AI16_EC3:16;
	signed CN3_VALVE_In_AI16_VALVE1:16;
	signed CN3_VALVE_In_AI16_VALVE2:16;
	signed CN3_VALVE_In_AI16_VALVE3:16;
	signed CN3_VALVE_In_AI16_VALVE4:16;
	signed CN3_VALVE_In_AI16_VALVE5:16;
	signed CN3_VALVE_In_AI16_VALVE6:16;
	signed CN3_VALVE_In_AI16_VALVE7:16;
	signed CN3_VALVE_In_AI16_VALVE8:16;
	signed CN3_VALVE_In_AI16_VALVE9:16;
	signed CN3_VALVE_In_AI16_VALVE10:16;
	signed CN3_VALVE_In_AI16_VALVE11:16;
	signed CN3_VALVE_In_AI16_VALVE12:16;
	signed CN3_SENSOR_In_AI16_SENSOR1:16;
	signed CN3_SENSOR_In_AI16_SENSOR2:16;
	signed CN3_SENSOR_In_AI16_SENSOR3:16;
	signed CN3_SENSOR_In_AI16_SENSOR4:16;
	signed CN3_SENSOR_In_AI16_SENSOR5:16;
	signed CN3_SENSOR_In_AI16_SENSOR6:16;
	signed CN3_SENSOR_In_AI16_SENSOR7:16;
	signed CN3_SENSOR_In_AI16_SENSOR8:16;
	signed CN3_SENSOR_In_AI16_SENSOR9:16;
	signed CN3_SENSOR_In_AI16_SENSOR10:16;
	signed CN3_SENSOR_In_AI16_SENSOR11:16;
	signed CN3_SENSOR_In_AI16_SENSOR12:16;
	unsigned PADDING_VAR_1:16;
} PI_OUT;

#define COMPUTED_PI_IN_SIZE 80
typedef struct
{
	signed CN1_EC_Out_AI16_EG:16;
	signed CN1_VALVE_Out_AI16_VALVE1:16;
	signed CN1_VALVE_Out_AI16_VALVE2:16;
	signed CN1_VALVE_Out_AI16_VALVE3:16;
	signed CN1_VALVE_Out_AI16_VALVE4:16;
	signed CN1_VALVE_Out_AI16_VALVE5:16;
	signed CN1_VALVE_Out_AI16_VALVE6:16;
	signed CN1_VALVE_Out_AI16_VALVE7:16;
	signed CN1_VALVE_Out_AI16_VALVE8:16;
	signed CN1_VALVE_Out_AI16_VALVE9:16;
	signed CN1_VALVE_Out_AI16_VALVE10:16;
	signed CN1_VALVE_Out_AI16_VALVE11:16;
	signed CN1_VALVE_Out_AI16_VALVE12:16;
	signed CN2_EC_Out_AI16_EG:16;
	signed CN2_VALVE_Out_AI16_VALVE1:16;
	signed CN2_VALVE_Out_AI16_VALVE2:16;
	signed CN2_VALVE_Out_AI16_VALVE3:16;
	signed CN2_VALVE_Out_AI16_VALVE4:16;
	signed CN2_VALVE_Out_AI16_VALVE5:16;
	signed CN2_VALVE_Out_AI16_VALVE6:16;
	signed CN2_VALVE_Out_AI16_VALVE7:16;
	signed CN2_VALVE_Out_AI16_VALVE8:16;
	signed CN2_VALVE_Out_AI16_VALVE9:16;
	signed CN2_VALVE_Out_AI16_VALVE10:16;
	signed CN2_VALVE_Out_AI16_VALVE11:16;
	signed CN2_VALVE_Out_AI16_VALVE12:16;
	signed CN3_EC_Out_AI16_EG:16;
	signed CN3_VALVE_Out_AI16_VALVE1:16;
	signed CN3_VALVE_Out_AI16_VALVE2:16;
	signed CN3_VALVE_Out_AI16_VALVE3:16;
	signed CN3_VALVE_Out_AI16_VALVE4:16;
	signed CN3_VALVE_Out_AI16_VALVE5:16;
	signed CN3_VALVE_Out_AI16_VALVE6:16;
	signed CN3_VALVE_Out_AI16_VALVE7:16;
	signed CN3_VALVE_Out_AI16_VALVE8:16;
	signed CN3_VALVE_Out_AI16_VALVE9:16;
	signed CN3_VALVE_Out_AI16_VALVE10:16;
	signed CN3_VALVE_Out_AI16_VALVE11:16;
	signed CN3_VALVE_Out_AI16_VALVE12:16;
	unsigned PADDING_VAR_1:16;
} PI_IN;

#endif
