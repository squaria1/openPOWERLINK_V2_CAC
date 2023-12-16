/*
* This file was autogenerated by openCONFIGURATOR-2.2.1_release on 16-Dec-2023 00:37:42
* Project: ASTREOS-1_OpenPOWERLINK
* Application process for OBC_MN(240)
*/
#ifndef XAP_h
#define XAP_h

#define COMPUTED_PI_OUT_SIZE 192

typedef union
{
	struct
	{
		signed CN1_Input_AI16_EC1 : 16;
		signed CN1_Input_AI8_VALVE1 : 8;
		signed CN1_Input_AI8_VALVE2 : 8;
		signed CN1_Input_AI8_VALVE3 : 8;
		signed CN1_Input_AI8_VALVE4 : 8;
		signed CN1_Input_AI8_VALVE5 : 8;
		signed CN1_Input_AI8_VALVE6 : 8;
		signed CN1_Input_AI8_VALVE7 : 8;
		signed CN1_Input_AI8_VALVE8 : 8;
		signed CN1_Input_AI8_VALVE9 : 8;
		signed CN1_Input_AI8_VALVE10 : 8;
		signed CN1_Input_AI8_VALVE11 : 8;
		signed CN1_Input_AI8_VALVE12 : 8;
		signed CN1_Input_AI32_SENSOR1 : 32;
		signed CN1_Input_AI32_SENSOR2 : 32;
		signed CN1_Input_AI32_SENSOR3 : 32;
		signed CN1_Input_AI32_SENSOR4 : 32;
		signed CN1_Input_AI32_SENSOR5 : 32;
		signed CN1_Input_AI32_SENSOR6 : 32;
		signed CN1_Input_AI32_SENSOR7 : 32;
		signed CN1_Input_AI32_SENSOR8 : 32;
		signed CN1_Input_AI32_SENSOR9 : 32;
		signed CN1_Input_AI32_SENSOR10 : 32;
		signed CN1_Input_AI32_SENSOR11 : 32;
		signed CN1_Input_AI32_SENSOR12 : 32;
		signed CN2_Input_AI16_EC2 : 16;
		signed CN2_Input_AI8_VALVE1 : 8;
		signed CN2_Input_AI8_VALVE2 : 8;
		signed CN2_Input_AI8_VALVE3 : 8;
		signed CN2_Input_AI8_VALVE4 : 8;
		signed CN2_Input_AI8_VALVE5 : 8;
		signed CN2_Input_AI8_VALVE6 : 8;
		signed CN2_Input_AI8_VALVE7 : 8;
		signed CN2_Input_AI8_VALVE8 : 8;
		signed CN2_Input_AI8_VALVE9 : 8;
		signed CN2_Input_AI8_VALVE10 : 8;
		signed CN2_Input_AI8_VALVE11 : 8;
		signed CN2_Input_AI8_VALVE12 : 8;
		signed CN2_Input_AI32_SENSOR1 : 32;
		signed CN2_Input_AI32_SENSOR2 : 32;
		signed CN2_Input_AI32_SENSOR3 : 32;
		signed CN2_Input_AI32_SENSOR4 : 32;
		signed CN2_Input_AI32_SENSOR5 : 32;
		signed CN2_Input_AI32_SENSOR6 : 32;
		signed CN2_Input_AI32_SENSOR7 : 32;
		signed CN2_Input_AI32_SENSOR8 : 32;
		signed CN2_Input_AI32_SENSOR9 : 32;
		signed CN2_Input_AI32_SENSOR10 : 32;
		signed CN2_Input_AI32_SENSOR11 : 32;
		signed CN2_Input_AI32_SENSOR12 : 32;
		signed CN3_Input_AI16_EC3 : 16;
		signed CN3_Input_AI8_VALVE1 : 8;
		signed CN3_Input_AI8_VALVE2 : 8;
		signed CN3_Input_AI8_VALVE3 : 8;
		signed CN3_Input_AI8_VALVE4 : 8;
		signed CN3_Input_AI8_VALVE5 : 8;
		signed CN3_Input_AI8_VALVE6 : 8;
		signed CN3_Input_AI8_VALVE7 : 8;
		signed CN3_Input_AI8_VALVE8 : 8;
		signed CN3_Input_AI8_VALVE9 : 8;
		signed CN3_Input_AI8_VALVE10 : 8;
		signed CN3_Input_AI8_VALVE11 : 8;
		signed CN3_Input_AI8_VALVE12 : 8;
		signed CN3_Input_AI32_SENSOR1 : 32;
		signed CN3_Input_AI32_SENSOR2 : 32;
		signed CN3_Input_AI32_SENSOR3 : 32;
		signed CN3_Input_AI32_SENSOR4 : 32;
		signed CN3_Input_AI32_SENSOR5 : 32;
		signed CN3_Input_AI32_SENSOR6 : 32;
		signed CN3_Input_AI32_SENSOR7 : 32;
		signed CN3_Input_AI32_SENSOR8 : 32;
		signed CN3_Input_AI32_SENSOR9 : 32;
		signed CN3_Input_AI32_SENSOR10 : 32;
		signed CN3_Input_AI32_SENSOR11 : 32;
		signed CN3_Input_AI32_SENSOR12 : 32;
	} PI_OUT;
	signed out_array[80];
} UNION_OUT;


#define COMPUTED_PI_IN_SIZE 44
typedef union
{
	struct
	{
		signed CN1_Output_AI16_EG : 16;
		signed CN1_Output_AI8_VALVE1 : 8;
		signed CN1_Output_AI8_VALVE2 : 8;
		signed CN1_Output_AI8_VALVE3 : 8;
		signed CN1_Output_AI8_VALVE4 : 8;
		signed CN1_Output_AI8_VALVE5 : 8;
		signed CN1_Output_AI8_VALVE6 : 8;
		signed CN1_Output_AI8_VALVE7 : 8;
		signed CN1_Output_AI8_VALVE8 : 8;
		signed CN1_Output_AI8_VALVE9 : 8;
		signed CN1_Output_AI8_VALVE10 : 8;
		signed CN1_Output_AI8_VALVE11 : 8;
		signed CN1_Output_AI8_VALVE12 : 8;
		signed CN2_Output_AI16_EG : 16;
		signed CN2_Output_AI8_VALVE1 : 8;
		signed CN2_Output_AI8_VALVE2 : 8;
		signed CN2_Output_AI8_VALVE3 : 8;
		signed CN2_Output_AI8_VALVE4 : 8;
		signed CN2_Output_AI8_VALVE5 : 8;
		signed CN2_Output_AI8_VALVE6 : 8;
		signed CN2_Output_AI8_VALVE7 : 8;
		signed CN2_Output_AI8_VALVE8 : 8;
		signed CN2_Output_AI8_VALVE9 : 8;
		signed CN2_Output_AI8_VALVE10 : 8;
		signed CN2_Output_AI8_VALVE11 : 8;
		signed CN2_Output_AI8_VALVE12 : 8;
		signed CN3_Output_AI16_EG : 16;
		signed CN3_Output_AI8_VALVE1 : 8;
		signed CN3_Output_AI8_VALVE2 : 8;
		signed CN3_Output_AI8_VALVE3 : 8;
		signed CN3_Output_AI8_VALVE4 : 8;
		signed CN3_Output_AI8_VALVE5 : 8;
		signed CN3_Output_AI8_VALVE6 : 8;
		signed CN3_Output_AI8_VALVE7 : 8;
		signed CN3_Output_AI8_VALVE8 : 8;
		signed CN3_Output_AI8_VALVE9 : 8;
		signed CN3_Output_AI8_VALVE10 : 8;
		signed CN3_Output_AI8_VALVE11 : 8;
		signed CN3_Output_AI8_VALVE12 : 8;
	} PI_IN;
	signed out_array[40];
}UNION_IN;


#endif
