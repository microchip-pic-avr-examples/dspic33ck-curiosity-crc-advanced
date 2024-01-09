/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS "AS IS." 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PRINT_H
#define	PRINT_H

#include "mcc_generated_files/system/clock.h"
#include "mcc_generated_files/timer/tmr1.h"
//cppcheck-suppress misra-c2012-21.6
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "crc_settings.h"
#include "demo_settings.h"
#include "swap.h"

/**
 *  The use of stdio.h functions does not follow the MISRA 2012 specification.
 * 
 *  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
 * 
 *  Reasoning: printf is required for printing to the terminal in this code example
 */

enum 
{
    TOGGLE_SETTINGS = 0,
    TOGGLE_DATA_SETTINGS,
    DO_HARDWARE_CRC,
    LEAVE_TRANSMITTER,
    ERROR
};

#define timerCountToMs(timerCount, timerPeriod) ((timerCount / (double)Timer1.PeriodGet()) * (double)timerPeriod)

#define calculateInstructions(softwareCRCCalculationTime) ((double)CLOCK_InstructionFrequencyGet() * softwareCRCCalculationTime)

//Section: Printing APIs

/**
 * @brief        Creates an error during transmission.
 *               This function allows the user to force an error during transmission, allowing
 *               the user to test the CRC's error-catching ability.
 * @param[in]    message - Pointer to the first value of the message array being transmitted
 * @return       none
 */
void injectError(uint32_t *message);

/**
 * @brief        Takes the user input and sets the transmitter status
 *               Executes the user input. Works for both the main menu and data sub-menu. 
 * @param[in]    buffer - user input from the menu
 * @return       Returns an integer for the transmitter status
 */
uint8_t UART_ExecuteMainMenuInput(uint8_t* buffer);

/**
 * @brief        Prints a 16-bit array.
 * @param[in]    array[] - The 8-bit array that will be printed as a 16-bit array
 * @param[in]    arrayLength - Size of the 8-bit array
 * @return       none
 */
void print16BitArray(uint8_t array[], uint8_t arrayLength);

/**
 * @brief        Prints a 32-bit array.
 * @param[in]    array[] - The 8-bit array that will be printed as a 32-bit array
 * @param[in]    arrayLength - Size of the 8-bit array   
 * @return       none
 */
void print32BitArray(uint8_t array[], uint8_t arrayLength);

/**
 * @brief        Prints a line of asterisks in order to separate parts of the output.
 * @param[in]    length - The number of asterisks in the separator
 * @return       none
 */
void printSeparator(uint8_t length);

/**
 * @brief        Prints the sub-menu for selecting the desired data vector.
 * @param[in]    settings - struct that contains all of the boolean variables for all of the 
 *               settings
 * @param[in]    inputData - array that holds pointers to all four 8-bit data vectors
 * @param[in]    inputDataSizes - array that holds the number of words in all 8-bit data
 *               vectors
 * @return       none
 */
void printDataSubMenu(CRC_SETTINGS settings, uint8_t* inputData[], uint8_t inputDataSizes[]);

/**
 * @brief        Prints an input string with a color based on the input setting
 * @param        inputString - string of characters to be printed
 * @param        settingValue - value of the setting that determines the text color (false ->
 *               red, true -> green)
 * @return       none
 */
void printWithColor(const char* inputString, bool settingValue);

/**
 * @brief        Prints a line of the menu
 * @param        lineTitle - title of the line, includes the row and the setting on the line
 * @param        settingOption1 - string that represents one of the options (ex: MSB)
 * @param        settingOption2 - string that represents one of the options (ex: LSB)
 * @param        settingValue - value of the setting that determines the text color (false ->
 *               red, true -> green)
 * @return       none
 */
void printLine(const char* lineTitle, const char* settingOption1, const char* settingOption2, bool settingValue);

/**
 * @brief        Prints a line of the menu for the settings that are dependent on the number 
 *               of bits
 * @param        lineTitle - title of the line, includes the row and the setting on the line
 * @param        settingValue - value of the setting that determines the text color (false ->
 *               red, true -> green)
 * @param        is32Bit - if true, the system is in 32-bit calculation mode, if false, the 
 *               system is in 16-bit calculation mode
 */
void printLine32BitDependent(const char* lineTitle, bool settingValue, bool is32Bit);

/**
 * @brief        Prints the main menu for the CRC application.
 *               This function prints the menu of active settings for the user to view.
 * @param[in]    inputData - 8-bit data to be printed
 * @param[in]    inputDataSize - Number of words in the 8-bit data to be printed
 * @return       none
 */
void printMenu(uint8_t* inputData[], uint8_t inputDataSize[]);

/**
 * @brief        Prints the transmission vector
 * @param[in]    transmission - vector to be printed
 * @param[in]    inputDataSize - Number of 8-bit words in the transmission
 * @return       none
 */
void printTransmission(uint32_t transmission[], uint8_t inputDataSize);

/**
 * @brief        Compares two values and prints whether the calculated values match and the 
 *               values themselves. Used in the validation of the two CRC calculations.
 * @param[in]    transmittedValue - first value to be compared
 * @param[in]    calculatedValue - second value to be compared
 * @return       none
 */
void printCompareCalculationResults(uint32_t transmittedValue, uint32_t calculatedValue);

/**
 * @brief        Prints the result of a CRC calculation
 * @param[in]    value - result of CRC calculation, to be printed
 * @return       none
 */
void printCRCCalculationResult(uint32_t value);

/**
 * @brief        Prints the results of benchmarking
 * @param[in]    hardwareCRCTimerCount - value of the counter from the timer run 
 *               during the calculation of the hardware CRC value
 * @param[in]    softwareCRCTimerCount - value of the counter from the timer run 
 *               during the calculation of the software CRC value
 * @param[in]    hardwareToSoftwareTimeRatio - ratio of the software CRC Timer count
 *               to the hardware CRC Timer count
 * @return       none
 */
void printBenchmarkingResults(uint16_t hardwareCRCTimerCount, uint16_t softwareCRCTimerCount, double hardwareToSoftwareTimeRatio);

#endif	/* PRINT_H */