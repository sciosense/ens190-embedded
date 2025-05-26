#ifndef SCIOSENSE_ENS190_C_INL
#define SCIOSENSE_ENS190_C_INL

#include "ScioSense_Ens190.h"

#include <math.h>

#define clear()             if (ens190->io.clear) { ens190->io.clear(ens190->io.config); }
#define wait(ms)            ens190->io.wait(ms)

#define hasAnyFlag(a, b)    (((a) & (b)) != 0)
#define hasFlag(a, b)       (((a) & (b)) == (b))

#define memset(a, b, s)     for(size_t i = 0; i < s; i++) {a[i] = b;}
#define memcpy(a, b, s)     for(size_t i = 0; i < s; i++) {a[i] = b[i];}

static inline uint16_t Ens190_GetValueOf16(const uint8_t* data, const uint16_t resultAddress)
{
    return ((uint16_t)data[resultAddress] << 8) + (uint16_t)data[resultAddress + 1];
}

static inline uint32_t Ens190_GetValueOf32(uint8_t* data, const uint16_t resultAddress)
{
    return  ((uint32_t)data[resultAddress + 0] << 24)
          + ((uint32_t)data[resultAddress + 1] << 16)
          + ((uint32_t)data[resultAddress + 2] <<  8)
          +  (uint32_t)data[resultAddress + 3];
}

static inline uint64_t Ens190_GetValueOf64(uint8_t* data, const uint16_t resultAddress)
{
    return  ((uint64_t)data[resultAddress + 0] << 56)
          + ((uint64_t)data[resultAddress + 1] << 48)
          + ((uint64_t)data[resultAddress + 2] << 40)
          + ((uint64_t)data[resultAddress + 3] << 32)
          + ((uint64_t)data[resultAddress + 4] << 24)
          + ((uint64_t)data[resultAddress + 5] << 16)
          + ((uint64_t)data[resultAddress + 6] << 8)
          +  (uint64_t)data[resultAddress + 7];
}

static inline Result Ens190_Read(ScioSense_Ens190* ens190, uint8_t* data, const size_t size)
{
    return ens190->io.read(ens190->io.config, 0, data, size);
}

static inline Result Ens190_Write(ScioSense_Ens190* ens190, uint8_t* data, const size_t size)
{
    return ens190->io.write(ens190->io.config, 0, data, size);
}

static inline Result Ens190_Invoke(ScioSense_Ens190* ens190, Ens190_Command command, const size_t sizeCommand, uint8_t* resultBuf, const size_t sizeResponse)
{
    Result result;

    result = Ens190_Write(ens190, (uint8_t*)command, sizeCommand);

    if (result == RESULT_OK)
    {
        if (resultBuf != NULL)
        {
            result = Ens190_Read(ens190, resultBuf, sizeResponse);
            if (result == RESULT_OK)
            {
                result = Ens190_CheckCommandResponse(command, resultBuf, (Ens190_CommandResponse)sizeResponse);
            }
        }
    }

    return result;
}

static inline Result Ens190_Reset(ScioSense_Ens190* ens190)
{
    Result result;

    memset(ens190->fwVersion ,      0, ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_PAYLOAD);
    memset(ens190->dataBuffer,      0, ENS190_COMMAND_RESPONSE_CO2_VALUE_PAYLOAD);

    ens190->serialNumber        = 0;
    ens190->productionDate[0]   = 0;

    clear();

    result = Ens190_InvokeReadSerialNumber(ens190);
    if ( result == RESULT_OK )
    {
        result = Ens190_InvokeReadFirmwareVersion(ens190);        
    }

    return result;

}

static inline Result Ens190_InvokeReadSerialNumber(ScioSense_Ens190* ens190)
{
    Result result;
    static const Ens190_Command command = ENS190_COMMAND_GET_SERIAL_NUMBER;
    uint8_t buff[ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_LENGTH];

    result = Ens190_Invoke(ens190, command, ENS190_COMMAND_GET_SERIAL_NUMBER_LENGTH, buff, ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_LENGTH);
    if (
            buff[ENS190_COMMAND_RESPONSE_START_BYTE_ADDRESS]    ==  ENS190_COMMAND_ADDRESS_START_BYTE
        &&  buff[ENS190_COMMAND_RESPONSE_LENGTH_BYTE_ADDRESS]   ==  ( ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_PAYLOAD + 1 )
        &&  buff[ENS190_COMMAND_RESPONSE_COMMAND_BYTE_ADDRESS]  ==  ENS190_COMMAND_ADDRESS_GET_SERIAL_NUMBER
    )
    {
        result = Ens190_CheckData(buff, ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_LENGTH);
        if (result == RESULT_OK)
        {            
            memcpy(ens190->productionDate, (buff + ENS190_COMMAND_RESPONSE_PRODUCTION_DATE_BYTE_ADDRESS), ENS190_RESPONSE_PRODUCTION_DATE_PAYLOAD);
            ens190->serialNumber    = Ens190_GetValueOf16(buff, ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_BYTE_ADDRESS);
        }
    }

    return result;
}

static inline Result Ens190_InvokeReadFirmwareVersion(ScioSense_Ens190* ens190)
{
    Result result;

    static const Ens190_Command command = ENS190_COMMAND_GET_FIRMWARE_VERSION_NUMBER;
    uint8_t buff[ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_LENGTH];

    result = Ens190_Invoke(ens190, command, ENS190_COMMAND_GET_FIRMWARE_VERSION_NUMBER_LENGTH, buff, ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_LENGTH);
    if (
            buff[ENS190_COMMAND_RESPONSE_START_BYTE_ADDRESS]    ==  ENS190_COMMAND_ADDRESS_START_BYTE
        &&  buff[ENS190_COMMAND_RESPONSE_LENGTH_BYTE_ADDRESS]   ==  ( ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_PAYLOAD + 1 )
        &&  buff[ENS190_COMMAND_RESPONSE_COMMAND_BYTE_ADDRESS]  ==  ENS190_COMMAND_ADDRESS_GET_FIRMWARE_VERSION_NUMBER
)
    {
        result = Ens190_CheckData(buff, ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_LENGTH);
        if (result == RESULT_OK)
        {
            memcpy(ens190->fwVersion, (buff + ENS190_COMMAND_RESPONSE_DATA_BYTE_ADDRESS), ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_PAYLOAD);
        }
    }

    return result;
}

static inline Result Ens190_Update(ScioSense_Ens190* ens190)
{
    Result result;

    static const Ens190_Command command = ENS190_COMMAND_GET_CO2_VALUE;
    uint8_t buff[ENS190_COMMAND_RESPONSE_CO2_VALUE_LENGTH];

    result = Ens190_Invoke(ens190, command, ENS190_COMMAND_GET_CO2_VALUE_LENGTH, buff, ENS190_COMMAND_RESPONSE_CO2_VALUE_LENGTH);
    if (
            buff[ENS190_COMMAND_RESPONSE_START_BYTE_ADDRESS]    ==  ENS190_COMMAND_ADDRESS_START_BYTE
        &&  buff[ENS190_COMMAND_RESPONSE_LENGTH_BYTE_ADDRESS]   ==  ( ENS190_COMMAND_RESPONSE_CO2_VALUE_PAYLOAD + 1 )
        &&  buff[ENS190_COMMAND_RESPONSE_COMMAND_BYTE_ADDRESS]  ==  ENS190_COMMAND_ADDRESS_GET_CO2_VALUE
)
    {
        result = Ens190_CheckData(buff, ENS190_COMMAND_RESPONSE_CO2_VALUE_LENGTH);
        if (result == RESULT_OK)
        {
            memcpy(ens190->dataBuffer, (buff + ENS190_COMMAND_RESPONSE_DATA_BYTE_ADDRESS), ENS190_COMMAND_RESPONSE_CO2_VALUE_PAYLOAD);
        }
    }

    return result;
}

static inline Result Ens190_InvokeSetAbcParameters(ScioSense_Ens190* ens190, uint16_t periodHrs)
{
    Result result;

    uint8_t commandSetAbcParameters[] = ENS190_COMMAND_SET_ABC_PARAMETERS;
    commandSetAbcParameters[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS] = periodHrs >> 8;
    commandSetAbcParameters[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS + 1] = (uint8_t) periodHrs;
    commandSetAbcParameters[ENS190_COMMAND_SET_ABC_PARAMETERS_LENGTH - 1] = Ens190_CalculateChecksum(commandSetAbcParameters, ENS190_COMMAND_SET_ABC_PARAMETERS_LENGTH);

    uint8_t buff[ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH];
    
    result = Ens190_Invoke(ens190, commandSetAbcParameters, ENS190_COMMAND_SET_ABC_PARAMETERS_LENGTH, buff, ENS190_COMMAND_RESPONSE_ABC_PARAMETERS_LENGTH);

    if (result == RESULT_OK)
    {
        result = Ens190_CheckCommandResponse(commandSetAbcParameters, buff, ENS190_COMMAND_RESPONSE_ABC_PARAMETERS_LENGTH);
        if (result == RESULT_OK)
        {
            uint16_t currentPeriodHours = ((uint16_t)buff[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS] << 8) + ((uint16_t)buff[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS+1]);

            if ( currentPeriodHours != periodHrs )
            {
                result = RESULT_INVALID;
            }
        }
    }

    return result;
}

static inline Result Ens190_SetAlarmThresholds(ScioSense_Ens190* ens190, uint16_t highAlarmThresholdPpm, uint16_t lowAlarmThresholdPpm)
{
    Result result;

    uint8_t commandSetAlarmThresholds[] = ENS190_COMMAND_SET_ALARM_THRESHOLDS;
    commandSetAlarmThresholds[ENS190_COMMAND_PAYLOAD_HIGH_ALARM_THRESHOLD] = highAlarmThresholdPpm >> 8;
    commandSetAlarmThresholds[ENS190_COMMAND_PAYLOAD_HIGH_ALARM_THRESHOLD + 1] = (uint8_t) highAlarmThresholdPpm;
    commandSetAlarmThresholds[ENS190_COMMAND_PAYLOAD_LOW_ALARM_THRESHOLD] = lowAlarmThresholdPpm >> 8;
    commandSetAlarmThresholds[ENS190_COMMAND_PAYLOAD_LOW_ALARM_THRESHOLD + 1] = (uint8_t) lowAlarmThresholdPpm;
    commandSetAlarmThresholds[ENS190_COMMAND_SET_ABC_PARAMETERS_LENGTH - 1] = Ens190_CalculateChecksum(commandSetAlarmThresholds, ENS190_COMMAND_SET_ALARM_THRESHOLDS_LENGTH);

    uint8_t rbuff[ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH];

    result = Ens190_Invoke(ens190, commandSetAlarmThresholds, ENS190_COMMAND_GET_ALARM_THRESHOLDS_LENGTH, rbuff, ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH);
    
    if (result == RESULT_OK)
    {
        result = Ens190_CheckCommandResponse(commandSetAlarmThresholds, rbuff, ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH);
        if (result == RESULT_OK)
        {
            ens190->highAlarmPpm = ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS] << 8) + ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS+1]);
            ens190->lowAlarmPpm = ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS] << 8) + ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS+1]);

            if ( (highAlarmThresholdPpm!=ens190->highAlarmPpm) || (lowAlarmThresholdPpm!=ens190->lowAlarmPpm) )
            {
                result = RESULT_INVALID;
            }
        }
    }

    return result;
}

static inline Result Ens190_GetAlarmThresholds(ScioSense_Ens190* ens190)
{
    Result result;

    static const Ens190_Command commandGetAlarmThresholds = ENS190_COMMAND_GET_ALARM_THRESHOLDS;
    uint8_t rbuff[ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH];
    
    result = Ens190_Invoke(ens190, commandGetAlarmThresholds, ENS190_COMMAND_GET_ALARM_THRESHOLDS_LENGTH, rbuff, ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH);
    
    if (result == RESULT_OK)
    {
        result = Ens190_CheckCommandResponse(commandGetAlarmThresholds, rbuff, ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH);
        if (result == RESULT_OK)
        {
            ens190->highAlarmPpm = ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS] << 8) + ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS+1]);
            ens190->lowAlarmPpm = ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS] << 8) + ((uint16_t)rbuff[ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS+1]);
        }
    }

    return result;
}

static inline uint16_t Ens190_GetAlarmThresholdHigh(ScioSense_Ens190* ens190)
{
    Ens190_GetAlarmThresholds(ens190);

    return ens190->highAlarmPpm;
}

static inline uint16_t Ens190_GetAlarmThresholdLow(ScioSense_Ens190* ens190)
{
    Ens190_GetAlarmThresholds(ens190);

    return ens190->lowAlarmPpm;
}

static inline Result Ens190_SetCo2Baseline(ScioSense_Ens190* ens190, uint16_t co2BaselinePpm)
{
    Result result;

    uint8_t commandSetCo2Baseline[] = ENS190_COMMAND_SET_CO2_BASELINE;
    commandSetCo2Baseline[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS] = co2BaselinePpm >> 8;
    commandSetCo2Baseline[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS + 1] = (uint8_t) co2BaselinePpm;
    commandSetCo2Baseline[ENS190_COMMAND_SET_CO2_BASELINE_LENGTH - 1] = Ens190_CalculateChecksum(commandSetCo2Baseline, ENS190_COMMAND_SET_CO2_BASELINE_LENGTH);

    uint8_t rbuff[ENS190_COMMAND_RESPONSE_CO2_BASELINE_LENGTH];

    result = Ens190_Invoke(ens190, commandSetCo2Baseline, ENS190_COMMAND_GET_ALARM_THRESHOLDS_LENGTH, rbuff, ENS190_COMMAND_RESPONSE_CO2_BASELINE_LENGTH);
    
    if (result == RESULT_OK)
    {
        result = Ens190_CheckCommandResponse(commandSetCo2Baseline, rbuff, ENS190_COMMAND_RESPONSE_CO2_BASELINE_LENGTH);
    }

    return result;
}

static inline Result Ens190_SetBaudrate(ScioSense_Ens190* ens190, const Ens190_Baudrate baudrate)
{
    Result result;

    uint8_t commandSetBaudrate[] = ENS190_COMMAND_SET_BAUD_RATE;
    commandSetBaudrate[ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS] = baudrate;
    commandSetBaudrate[ENS190_COMMAND_SET_BAUD_RATE_LENGTH - 1] = Ens190_CalculateChecksum(commandSetBaudrate, ENS190_COMMAND_SET_BAUD_RATE_LENGTH);
    uint8_t buff[ENS190_COMMAND_RESPONSE_BAUDRATE_LENGTH];
    
    result = Ens190_Invoke(ens190, commandSetBaudrate, ENS190_COMMAND_SET_BAUD_RATE_LENGTH, buff, ENS190_COMMAND_RESPONSE_BAUDRATE_LENGTH);
    
    if (result == RESULT_OK)
    {
        result = Ens190_CheckCommandResponse(commandSetBaudrate, buff, ENS190_COMMAND_RESPONSE_BAUDRATE_LENGTH);
        if (result == RESULT_OK)
        {
            if ( baudrate != buff[ENS190_COMMAND_RESPONSE_DATA_BYTE_ADDRESS] )
            {
                result = RESULT_INVALID;
            }
            ens190->highAlarmPpm = ((uint16_t)buff[ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS] << 8) + ((uint16_t)buff[ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS+1]);
            ens190->lowAlarmPpm = ((uint16_t)buff[ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS] << 8) + ((uint16_t)buff[ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS+1]);
        }
    }

    return result;
}

static inline bool Ens190_IsConnected(ScioSense_Ens190* ens190)
{
    return ens190->fwVersion[0] != 0;
}

static inline uint16_t Ens190_GetCo2(ScioSense_Ens190* ens190)
{
    return Ens190_GetValueOf16(ens190->dataBuffer, 0);
}

static inline uint8_t* Ens190_GetFirmwareVersion(ScioSense_Ens190* ens190)
{
    return ens190->fwVersion;
}

static inline uint16_t Ens190_GetSerialNumber(ScioSense_Ens190* ens190)
{
    return ens190->serialNumber;
}

static inline uint8_t* Ens190_GetProductionDate(ScioSense_Ens190* ens190)
{
    return ens190->productionDate;
}

static inline Result Ens190_CheckData(const uint8_t* data, const Ens190_CommandResponse size)
{
    if (size < 3)
    {
        return RESULT_CHECKSUM_ERROR;
    }

    const uint8_t payloadSize   = ((uint8_t)size) - 1;
    const uint8_t checksum     = data[payloadSize];
    uint8_t calculatedChecksum = Ens190_CalculateChecksum(data, size);

    return (checksum == calculatedChecksum) ? RESULT_OK : RESULT_CHECKSUM_ERROR;
}

static inline uint8_t Ens190_CalculateChecksum(const uint8_t* data, const Ens190_CommandResponse size)
{
    const uint8_t payloadSize   = ((uint8_t)size) - 1;
    int16_t dataSum = 0;

    for (uint8_t i = 0; i<payloadSize; i++)
    {
        dataSum += data[i];
    }

    return (uint8_t) ( ((uint16_t)size * 256) - dataSum );
}

static inline Result Ens190_CheckCommandResponse(const Ens190_Command command, const uint8_t* data, const Ens190_CommandResponse size)
{
    Result result = RESULT_INVALID;

    if
    (
        command[ENS190_COMMAND_RESPONSE_START_BYTE_ADDRESS]     == data[ENS190_COMMAND_RESPONSE_START_BYTE_ADDRESS]
     && command[ENS190_COMMAND_RESPONSE_COMMAND_BYTE_ADDRESS]   == data[ENS190_COMMAND_RESPONSE_COMMAND_BYTE_ADDRESS]
    )
    {
        result = Ens190_CheckData(data, size);
    }

    return result;
}


#undef wait
#undef clear
#undef hasAnyFlag
#undef hasFlag
#undef memset
#undef memcpy

#endif // SCIOSENSE_ENS190_C_INL