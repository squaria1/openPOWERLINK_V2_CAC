#ifndef STATUSERRORDEFINE_H
#define STATUSERRORDEFINE_H

typedef enum
{
	noError						= 0x0000,

	/*=====INFORMATION AND SUCCESS=====*/
	// CSV
	infoInitCSV					= 0x0101,
	infoCSVChanged				= 0x0102,
	infoShutdownCSV				= 0x01FF,

	// OPL
	infoInitOPL					= 0x0201,
	infoEGNotChanged			= 0x0202,
	infoStopOrderReceived		= 0x0203,
	infoNoStopOrder				= 0x0204,
	infoShutdownOPL				= 0x02FF,

	// Valve
	infoInitValve				= 0x0301,
	infoVerifDependSucess		= 0x0302,
	infoNoDepend				= 0x0303,
	infoValveAlreadyActivated	= 0x0304,
	infoAllDependNotActivated	= 0x0305,
	infoShutdownValve			= 0x03FF,

	// Sensor
	infoInitSensor				= 0x0401,
	infoReadChannels			= 0x0402,
	infoShutdownSensor			= 0x04FF,

	// Main state
	infoStateToInit				= 0x7001,
	infoStateToControl			= 0x7002,
	infoStateToShutdown			= 0x7FFF,

	/*=============ERROR===============*/
	errOpenTelemFile			= 0xE001,
	errTestWriteFile			= 0xE002,
	errCloseTelemFile			= 0xE0FF,

	// CSV
	errAllocDataEG				= 0xE101,
	errAllocDataEtats			= 0xE102,
	errAllocDataPhysValves		= 0xE103,
	errAllocDataPhysSensors		= 0xE104,
	errAllocDataActivation		= 0xE105,
	errOpenEGFile				= 0xE106,
	errEGNotFoundInFile			= 0xE107,
	errOpenEtatsFile			= 0xE108,
	errOpenPhysValvesFile		= 0xE109,
	errOpenPhysSensorsFile		= 0xE10A,
	errOpenActivationFile		= 0xE10B,

	// OPL
	errOPLSystemInit			= 0xE201,
	errSelNetInterface			= 0xE202,
	errInitObjDictionary		= 0xE203,
	errOplkInit					= 0xE204,
	errOplkCreate				= 0xE205,
	errOplkAllocProcessImage	= 0xE206,
	errLinkPDOout				= 0xE207,
	errLinkPDOEGin				= 0xE208,
	errLinkPDOin				= 0xE209,
	errSendNMTResetCommand		= 0xE20A,
	errOplkFreeProcessImage		= 0xE20B,
	errSystemSendTerminate		= 0xE20C,
	errOplKernelStackDown		= 0xE20D,

	// Valve
	errGPIOPathEmpty			= 0xE301,
	errOpenGPIO					= 0xE302,
	errGPIOGetLine				= 0xE303,
	errGPIORequestOutput		= 0xE304,
	errGetValveInitValue		= 0xE305,
	errValueInitIsNotBinary		= 0xE306,
	errGPIOSetInitValue			= 0xE307,
	errGPIOGetValue				= 0xE308,
	errGPIODependValue			= 0xE309,
	errDependOutsideOfRange		= 0xE30A,
	errValueIsNotBinary			= 0xE30B,
	errGPIOSetValue				= 0xE30C,
	errGPIORelease				= 0xE3FF,

	// Sensor
	errOpenAdc					= 0xE401,
	errReadAdc					= 0xE402,
	errCloseAdc					= 0xE4FF,


} statusErrDef;

#endif