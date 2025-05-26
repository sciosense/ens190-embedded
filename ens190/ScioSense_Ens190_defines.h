#ifndef SCIOSENSE_ENS190_DEFINES_C_H
#define SCIOSENSE_ENS190_DEFINES_C_H

#include <inttypes.h>

//// Output data structure adresses for the measurement data struct (64 bytes) and sensor info struct (23 bytes)
typedef uint8_t ResultAddress;

//// Baudrates
typedef uint8_t Ens190_Baudrate;
#define ENS190_BAUDRATE_9600_BPS                            (0x00)      // Baudrate of 9600 bps
#define ENS190_BAUDRATE_2400_BPS                            (0x01)      // Baudrate of 2400 bps
#define ENS190_BAUDRATE_4800_BPS                            (0x02)      // Baudrate of 4800 bps
#define ENS190_BAUDRATE_19200_BPS                           (0x03)      // Baudrate of 19200 bps
#define ENS190_BAUDRATE_38400_BPS                           (0x04)      // Baudrate of 38400 bps
#define ENS190_BAUDRATE_115200_BPS                          (0x05)      // Baudrate of 115200 bps

//// Command protocol structure values
#define ENS190_COMMAND_LENGTH                               (8)
typedef const uint8_t Ens190_Command[ENS190_COMMAND_LENGTH];
#define ENS190_COMMAND_ADDRESS_START_BYTE                   (0x1A)

#define ENS190_COMMAND_ADDRESS_GET_CO2_VALUE                (0x01)      // Read CO2 values
#define ENS190_COMMAND_ADDRESS_SET_ABC_PARAMETERS           (0x10)      // Set the Automatic Baseline Correction parameters
#define ENS190_COMMAND_ADDRESS_SET_ALARM_THRESHOLDS         (0x13)      // Set CO2 alarm thresholds
#define ENS190_COMMAND_ADDRESS_SET_CO2_BASELINE_MANUALLY    (0x18)      // Set the CO2 baseline for the algorithm
#define ENS190_COMMAND_ADDRESS_SET_BAUD_RATE                (0x19)      // Set the baud rate to use in UART
#define ENS190_COMMAND_ADDRESS_GET_SERIAL_NUMBER            (0x0E)      // Read serial number of the device
#define ENS190_COMMAND_ADDRESS_GET_FIRMWARE_VERSION_NUMBER  (0x0F)      // Read Firmare version number of the device

#define ENS190_COMMAND_GET_CO2_VALUE_LENGTH                 (4)         // Length of "GetCo2Value" command
#define ENS190_COMMAND_SET_ABC_PARAMETERS_LENGTH            (6)         // Length of "SetAbcParameters" command
#define ENS190_COMMAND_SET_ALARM_THRESHOLDS_LENGTH          (8)         // Length of "SetAlarmThresholds" command
#define ENS190_COMMAND_GET_ALARM_THRESHOLDS_LENGTH          (8)         // Length of "GetAlarmThresholds" command
#define ENS190_COMMAND_SET_CO2_BASELINE_LENGTH              (6)         // Length of "SetCo2Baseline" command
#define ENS190_COMMAND_SET_BAUD_RATE_LENGTH                 (5)         // Length of "SetBaudRate" command
#define ENS190_COMMAND_GET_SERIAL_NUMBER_LENGTH             (4)         // Length of "GetSerialNumber" command
#define ENS190_COMMAND_GET_FIRMWARE_VERSION_NUMBER_LENGTH   (4)         // Length of "GetFirmwareVersionNumber" command

#define ENS190_COMMAND_PAYLOAD_SET_ABC_PARAMETERS_LENGTH    (2)         // Amount of bytes to write for the Automatic Baseline Correction parameters
#define ENS190_COMMAND_PAYLOAD_SET_ALARM_THRESHOLDS_LENGTH  (4)         // Amount of bytes to write for the alarm threshold
#define ENS190_COMMAND_PAYLOAD_SET_CO2_BASELINE_LENGTH      (2)         // Amount of bytes to write for the CO2 baseline
#define ENS190_COMMAND_PAYLOAD_SET_BAUD_RATE_LENGTH         (1)         // Amount of bytes to write for the Baud rate

#define ENS190_COMMAND_GET_CO2_VALUE                        {0x1A, 0x01, 0x01, 0xE4}                            // Request the current CO2 concentration
#define ENS190_COMMAND_SET_ABC_PARAMETERS                   {0x1A, 0x03, 0x10, 0x00, 0x00, 0xFF}                // Set the Automatic Baseline Correction parameters
#define ENS190_COMMAND_SET_ALARM_THRESHOLDS                 {0x1A, 0x05, 0x13, 0x00, 0x00, 0x00, 0x00, 0xFF}    // Set the alarm thresholds
#define ENS190_COMMAND_GET_ALARM_THRESHOLDS                 {0x1A, 0x05, 0x13, 0xFF, 0xFF, 0xFF, 0xFF, 0xD2}    // Read the currently programmed thresholds
#define ENS190_COMMAND_SET_CO2_BASELINE                     {0x1A, 0x03, 0x18, 0x00, 0x00, 0xFF}                // Set the CO2 baseline
#define ENS190_COMMAND_SET_BAUD_RATE                        {0x1A, 0x02, 0x19, 0x00, 0xFF}                      // Set the Baud rate
#define ENS190_COMMAND_GET_SERIAL_NUMBER                    {0x1A, 0x01, 0x0E, 0xD7}                            // Request the serial number
#define ENS190_COMMAND_GET_FIRMWARE_VERSION_NUMBER          {0x1A, 0x01, 0x0F, 0xD6}                            // Request the serial version

#define ENS190_COMMAND_PAYLOAD_BYTE_ADDRESS                 (3)
#define ENS190_COMMAND_PAYLOAD_HIGH_ALARM_THRESHOLD         (3)
#define ENS190_COMMAND_PAYLOAD_LOW_ALARM_THRESHOLD          (5)

//// Command response addresses ( answer protocol structure )
typedef uint8_t Ens190_CommandResponse;

#define ENS190_COMMAND_RESPONSE_CO2_VALUE_LENGTH                (8)     // Command response length for "ReadCo2" command
#define ENS190_COMMAND_RESPONSE_ABC_PARAMETERS_LENGTH           (6)     // Command response length for "SetAbcParameters" command
#define ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_LENGTH         (8)     // Command response length for "SetAlarmThresholds" command
#define ENS190_COMMAND_RESPONSE_CO2_BASELINE_LENGTH             (4)     // Command response length for "SetCo2Baseline" command
#define ENS190_COMMAND_RESPONSE_BAUDRATE_LENGTH                 (5)     // Command response length for "SetBaudRate" command
#define ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_LENGTH            (11)    // Command response length for "GetSerialNumber" command
#define ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_LENGTH  (15)    // Command response length for "GetFirmwareVesionNumber" command

#define ENS190_COMMAND_RESPONSE_CO2_VALUE_PAYLOAD               (4)     // Command response payload length for "ReadCo2" command
#define ENS190_COMMAND_RESPONSE_ABC_PARAMETERS_PAYLOAD          (2)     // Command response payload length for "SetAbcParameters" command
#define ENS190_COMMAND_RESPONSE_ALARM_THRESHOLDS_PAYLOAD        (4)     // Command response payload length for "SetAlarmThresholds" command
#define ENS190_COMMAND_RESPONSE_BAUDRATE_PAYLOAD                (1)     // Command response payload length for "SetBaudRate" command
#define ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_PAYLOAD           (7)     // Command response payload length for "GetSerialNumber" command
#define ENS190_RESPONSE_PRODUCTION_DATE_PAYLOAD                 (3)     // Command response payload length for production date part of the "GetSerialNumber"
#define ENS190_RESPONSE_SERIAL_NUMBER_PAYLOAD                   (2)     // Command response payload length for serial number part of the "GetSerialNumber"
#define ENS190_COMMAND_RESPONSE_FIRMWARE_VERSION_NUMBER_PAYLOAD (11)    // Command response payload length for "GetFirmwareVesionNumber" command

#define ENS190_COMMAND_RESPONSE_START_BYTE_ADDRESS              (0)
#define ENS190_COMMAND_RESPONSE_LENGTH_BYTE_ADDRESS             (1)
#define ENS190_COMMAND_RESPONSE_COMMAND_BYTE_ADDRESS            (2)
#define ENS190_COMMAND_RESPONSE_DATA_BYTE_ADDRESS               (3)    // Data start address
#define ENS190_COMMAND_RESPONSE_ALARM_HIGH_THRESHOLD_ADDRESS    (3)    // High alarm threshold start address
#define ENS190_COMMAND_RESPONSE_ALARM_LOW_THRESHOLD_ADDRESS     (5)    // Low alarm threshold start address
#define ENS190_COMMAND_RESPONSE_PRODUCTION_DATE_BYTE_ADDRESS    (3)    // Production date address in Serial number command responde
#define ENS190_COMMAND_RESPONSE_SERIAL_NUMBER_BYTE_ADDRESS      (6)    // Seriual number address in Serial number command responde

//// SystemTiming in ms
#define ENS190_SYSTEM_TIMING_STANDARD_MEASURE   (4000)

//// Result and Errors
#ifndef SCIOSENSE_RESULT_CODES
#define SCIOSENSE_RESULT_CODES
typedef int8_t Result;
#define RESULT_NOT_ALLOWED      (4)     // The requested command is not allowed.
#define RESULT_CHECKSUM_ERROR   (3)     // The value was read, but the checksum over the payload (valid and data) does not match.
#define RESULT_INVALID          (2)     // The value was read, but the data is invalid.
#define RESULT_IO_ERROR         (1)     // There was an IO communication error, read/write the stream failed.
#define RESULT_OK               (0)     // All OK; The value was read, the checksum matches, and data is valid.
#endif

#endif // SCIOSENSE_ENS190_DEFINES_C_H