/*
* This file was autogenerated by openCONFIGURATOR-2.2.1_release on 16-Dec-2023 00:37:42
* Project: ASTREOS-1_OpenPOWERLINK
* Application process for OBC_MN(240)
*/
#ifndef XAPOPL_h
#define XAPOPL_h

#define COMPUTED_PI_IN_SIZE 75

typedef struct
{
	/*
	struct
	{
		signed CN1_Input_AI16_EC1 : 16;										//0
		signed CN1_Input_AI8_VALVE1 : 8;									//1
		signed CN1_Input_AI8_VALVE2 : 8;									//2
		signed CN1_Input_AI8_VALVE3 : 8;									//3
		signed CN1_Input_AI8_VALVE4 : 8;									//4
		signed CN1_Input_AI8_VALVE5 : 8;									//5
		signed CN1_Input_AI8_VALVE6 : 8;									//6
		signed CN1_Input_AI8_VALVE7 : 8;									//7
		signed CN1_Input_AI8_VALVE8 : 8;									//8
		signed CN1_Input_AI8_VALVE9 : 8;									//9
		signed CN1_Input_AI8_VALVE10 : 8;									//10
		signed CN1_Input_AI8_VALVE11 : 8;									//11
		signed CN1_Input_AI8_VALVE12 : 8;									//12
		signed CN1_Input_AI32_SENSOR1 : 32;									//13
		signed CN1_Input_AI32_SENSOR2 : 32;									//14
		signed CN1_Input_AI32_SENSOR3 : 32;									//15
		signed CN1_Input_AI32_SENSOR4 : 32;									//16
		signed CN1_Input_AI32_SENSOR5 : 32;									//17
		signed CN1_Input_AI32_SENSOR6 : 32;									//18
		signed CN1_Input_AI32_SENSOR7 : 32;									//19
		signed CN1_Input_AI32_SENSOR8 : 32;									//20
		signed CN1_Input_AI32_SENSOR9 : 32;									//21
		signed CN1_Input_AI32_SENSOR10 : 32;								//22
		signed CN1_Input_AI32_SENSOR11 : 32;								//23
		signed CN1_Input_AI32_SENSOR12 : 32;								//24
		signed CN2_Input_AI16_EC2 : 16;										//25
		signed CN2_Input_AI8_VALVE1 : 8;									//26
		signed CN2_Input_AI8_VALVE2 : 8;									//27
		signed CN2_Input_AI8_VALVE3 : 8;									//28
		signed CN2_Input_AI8_VALVE4 : 8;									//29
		signed CN2_Input_AI8_VALVE5 : 8;									//30
		signed CN2_Input_AI8_VALVE6 : 8;									//31
		signed CN2_Input_AI8_VALVE7 : 8;									//32
		signed CN2_Input_AI8_VALVE8 : 8;									//33
		signed CN2_Input_AI8_VALVE9 : 8;									//34
		signed CN2_Input_AI8_VALVE10 : 8;									//35
		signed CN2_Input_AI8_VALVE11 : 8;									//36
		signed CN2_Input_AI8_VALVE12 : 8;									//37
		signed CN2_Input_AI32_SENSOR1 : 32;									//38
		signed CN2_Input_AI32_SENSOR2 : 32;									//39
		signed CN2_Input_AI32_SENSOR3 : 32;									//40
		signed CN2_Input_AI32_SENSOR4 : 32;									//41
		signed CN2_Input_AI32_SENSOR5 : 32;									//42
		signed CN2_Input_AI32_SENSOR6 : 32;									//43
		signed CN2_Input_AI32_SENSOR7 : 32;									//44
		signed CN2_Input_AI32_SENSOR8 : 32;									//45
		signed CN2_Input_AI32_SENSOR9 : 32;									//46
		signed CN2_Input_AI32_SENSOR10 : 32;								//47
		signed CN2_Input_AI32_SENSOR11 : 32;								//48
		signed CN2_Input_AI32_SENSOR12 : 32;								//49
		signed CN3_Input_AI16_EC3 : 16;										//50
		signed CN3_Input_AI8_VALVE1 : 8;									//51
		signed CN3_Input_AI8_VALVE2 : 8;									//52
		signed CN3_Input_AI8_VALVE3 : 8;									//53
		signed CN3_Input_AI8_VALVE4 : 8;									//54
		signed CN3_Input_AI8_VALVE5 : 8;									//55
		signed CN3_Input_AI8_VALVE6 : 8;									//56
		signed CN3_Input_AI8_VALVE7 : 8;									//57
		signed CN3_Input_AI8_VALVE8 : 8;									//58
		signed CN3_Input_AI8_VALVE9 : 8;									//59
		signed CN3_Input_AI8_VALVE10 : 8;									//60
		signed CN3_Input_AI8_VALVE11 : 8;									//61
		signed CN3_Input_AI8_VALVE12 : 8;									//62
		signed CN3_Input_AI32_SENSOR1 : 32;									//63
		signed CN3_Input_AI32_SENSOR2 : 32;									//64
		signed CN3_Input_AI32_SENSOR3 : 32;									//65
		signed CN3_Input_AI32_SENSOR4 : 32;									//66
		signed CN3_Input_AI32_SENSOR5 : 32;									//67
		signed CN3_Input_AI32_SENSOR6 : 32;									//68
		signed CN3_Input_AI32_SENSOR7 : 32;									//69
		signed CN3_Input_AI32_SENSOR8 : 32;									//70
		signed CN3_Input_AI32_SENSOR9 : 32;									//71
		signed CN3_Input_AI32_SENSOR10 : 32;								//72
		signed CN3_Input_AI32_SENSOR11 : 32;								//73
		signed CN3_Input_AI32_SENSOR12 : 32;								//74
	} PI_IN;
	*/
	signed out_MN_array[COMPUTED_PI_IN_SIZE];
} PI_OUT;


#define COMPUTED_PI_OUT_SIZE 39
typedef struct
{
	/*
	struct
	{
		signed CN1_Output_AI16_EG : 16;											//0
		signed CN1_Output_AI8_VALVE1 : 8;										//1
		signed CN1_Output_AI8_VALVE2 : 8;										//2
		signed CN1_Output_AI8_VALVE3 : 8;										//3
		signed CN1_Output_AI8_VALVE4 : 8;										//4
		signed CN1_Output_AI8_VALVE5 : 8;										//5
		signed CN1_Output_AI8_VALVE6 : 8;										//6
		signed CN1_Output_AI8_VALVE7 : 8;										//7
		signed CN1_Output_AI8_VALVE8 : 8;										//8
		signed CN1_Output_AI8_VALVE9 : 8;										//9
		signed CN1_Output_AI8_VALVE10 : 8;										//10
		signed CN1_Output_AI8_VALVE11 : 8;										//11
		signed CN1_Output_AI8_VALVE12 : 8;										//12
		signed CN2_Output_AI16_EG : 16;											//13
		signed CN2_Output_AI8_VALVE1 : 8;										//14
		signed CN2_Output_AI8_VALVE2 : 8;										//15
		signed CN2_Output_AI8_VALVE3 : 8;										//16
		signed CN2_Output_AI8_VALVE4 : 8;										//17
		signed CN2_Output_AI8_VALVE5 : 8;										//18
		signed CN2_Output_AI8_VALVE6 : 8;										//19
		signed CN2_Output_AI8_VALVE7 : 8;										//20
		signed CN2_Output_AI8_VALVE8 : 8;										//21
		signed CN2_Output_AI8_VALVE9 : 8;										//22
		signed CN2_Output_AI8_VALVE10 : 8;										//23
		signed CN2_Output_AI8_VALVE11 : 8;										//24
		signed CN2_Output_AI8_VALVE12 : 8;										//25
		signed CN3_Output_AI16_EG : 16;											//26
		signed CN3_Output_AI8_VALVE1 : 8;										//27
		signed CN3_Output_AI8_VALVE2 : 8;										//28
		signed CN3_Output_AI8_VALVE3 : 8;										//29
		signed CN3_Output_AI8_VALVE4 : 8;										//30
		signed CN3_Output_AI8_VALVE5 : 8;										//31
		signed CN3_Output_AI8_VALVE6 : 8;										//32
		signed CN3_Output_AI8_VALVE7 : 8;										//33
		signed CN3_Output_AI8_VALVE8 : 8;										//34
		signed CN3_Output_AI8_VALVE9 : 8;										//35
		signed CN3_Output_AI8_VALVE10 : 8;										//36
		signed CN3_Output_AI8_VALVE11 : 8;										//37
		signed CN3_Output_AI8_VALVE12 : 8;										//38
	} PI_OUT;
	*/
	signed in_MN_array[COMPUTED_PI_OUT_SIZE];
}PI_IN;


#endif
