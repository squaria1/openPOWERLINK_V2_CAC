/**
 * \file sensor.cpp
 * \brief Module to read sensors values
 * \author Mael Parot, Corentin Berthon
 * \version 1.1
 * \date 11/04/2024
 *
 * Contains all functions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */

#include "sensor.h"

static int i = 0, num = 0;
static uint8_t tabSensorActivated[MAX_SENSORS];
static int fd[MAX_SENSORS];
static int16_t valSensors[MAX_SENSORS];

#if (TARGET_SYSTEM == _WIN32_)
#else
static modbus_t* ctx[MAX_SENSORS] = {};
static uint16_t modbusReg[MODBUS_NBREG]; // will store read registers values
#endif

sensor::sensor()
{
    //constructor
    //initialize the tabSensorActivated array
    memset(tabSensorActivated, 0, sizeof(tabSensorActivated));
}

sensor::~sensor()
{
    //destructor
}

/**
 * \brief function to initialize the sensor module.
 * 
 * \return statusErrDef that values errOpenAdc
 * when a sysfs file of the MCP3008 fails to open
 * or errReadAdc when a sysfs file read of the MCP3008 fails
 * or noError when the function exits successfully.
 */
statusErrDef sensor::initSensor() {
    statusErrDef res = noError;

    #if (TARGET_SYSTEM == _WIN32_)
    #else
        for (int i = 0; i < MAX_SENSORS; i++)
        {
            switch (getSensorType(i + 1))
            {
            case 1:
                tabSensorActivated[i] = getActivation(i + nbValuesCN_Out_ByCN + nbValuesCN_Out / 2 + 2);
                break;
            case 2:
                ctx[i] = modbus_new_rtu(MODBUS_DEVICELOC, getModbusBaudRate(i+1), 
                                        getModbusParity(i+1), getModbusDataBits(i+1), getModbusStopBit(i+1));
                if (!ctx[i]) {
                    fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
                }
                if (modbus_connect(ctx[i]) == -1) {
                    fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
                    modbus_free(ctx[i]);
                }
                modbus_set_slave(ctx[i], getModbusAddrRemoteSlave(i+1));
                break;
            default:
                break;
            }
        }

        res = readChannels();
        if (res != noError)
            return res;
    #endif
    return res;
}

/**
 * \brief function to shutdown the sensor module.
 * 
 * \return statusErrDef that values errCloseAdc
 * when a sysfs file of the MCP3008 fails to close
 * or noError when the function exits successfully.
 */
statusErrDef sensor::extinctSensor() {
    statusErrDef res = noError;

    closeAdc();

#if (TARGET_SYSTEM == _WIN32_)
#else
    for (int i = 0; i < MAX_SENSORS; i++)
    {
        if (getSensorType(i + 1) == 2)
        {
            modbus_close(ctx[i]);
            modbus_free(ctx[i]);
        }
    }
    #endif

    return res;
}

/**
 * \brief function getter of the value of a sensor
 * from the array of sensors.
 * 
 * \param index the index of the array of sensors
 * \return the value of the sensor in 2 bytes signed.
 */
int16_t getAdc_value(int index) {
    return valSensors[index];
}

/**
 * \brief function the read the channels of the
 * MCP3008 by opening, reading and closing the sysfs files.
 * 
 * \return statusErrDef that values errOpenAdc 
 * when a sysfs file of the MCP3008 fails to open
 * or errReadAdc when a sysfs file read of the MCP3008 fails
 * or noError when the function exits successfully. 
 */
statusErrDef readChannels()
{
    int i;
    statusErrDef res = noError;
    memset(fd, 0, sizeof(fd));
    memset(valSensors, 0, sizeof(valSensors));

    for (i = 0; i < MAX_ADC; i++) {
        if (tabSensorActivated[i]) {
            fd[i] = openAdc(i);
            if (fd[i] == ADC_READ_ERROR)
            {
                res = errOpenAdc;
                break;
            }
        }
    }

    for (i = 0; i < MAX_ADC; i++) {
        if (!tabSensorActivated[i])
            continue;

        valSensors[i] = readAdc(fd[i]);

        // reset for next read
        lseek(fd[i], 0, SEEK_SET);

        if (valSensors[i] == ADC_READ_ERROR)
        {
            res = errReadAdc;
            break;
        }
    }

    closeAdc();

#if (TARGET_SYSTEM == _WIN32_)
#else
    for (int i = 0; i < MAX_SENSORS; i++)
    {
        if (getSensorType(i + 1) == 2)
        {
            num = modbus_read_registers(ctx[i], getModbusStartAddress(i+1), MODBUS_NBREG, modbusReg);
            if (num != 1)// number of read registers is not the one expected
                fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
            else
            {
                printf("Modbus read registers success! \n");
                valSensors[i] = modbusReg[0];
            }
        }
    }

    #endif

    return res;
}

/**
 * \brief function to close the sysfs files
 * of the MCP3008 kernel module.
 * 
 * \return statusErrDef that values errCloseAdc 
 * when a sysfs file of the MCP3008 fails to close
 * or noError when the function exits successfully.  
 */
statusErrDef closeAdc()
{
    statusErrDef res = noError;
    int ret = 0;

    for (i = 0; i < MAX_ADC; i++) {
        if (fd[i] > 0)
        {
            ret = close(fd[i]);
            if (ret < 0)
            {
                res = errCloseAdc;
                break;
            }
        }
    }

    return res;
}

/**
 * \brief function to read the value of a sensor
 * from a sysfs file of the MCP3008 kernel module.
 * 
 * \param fd the file location
 * \return ADC_READ_ERROR when the file reading fails
 * or the value of a sensor.
 */
int readAdc(int fd)
{
    char buff[8];
    buff[7] = 0;

    int val = ADC_READ_ERROR;

    memset(buff, 0, sizeof(buff));

    if (read(fd, buff, 8) < 0)
        perror("read()");
    else
        val = atoi(buff);

    return val;
}

/**
 * \brief function to open a sysfs file 
 * of the MCP3008 kernel module.
 * 
 * \param adc the MCP3008 channel number
 * \return ADC_READ_ERROR when the file opening fails
 * or the file location.
 */
int openAdc(int adc)
{
    char path[128];
    
    sprintf(path, "%sin_voltage%d_raw", IIOSYSPATH, adc);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open()");
        printf("%s\n", path);
        return ADC_READ_ERROR;
    }

    return fd;
}