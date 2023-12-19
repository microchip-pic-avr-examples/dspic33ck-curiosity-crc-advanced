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

#ifndef HARDWARE_CRC_H
#define	HARDWARE_CRC_H

#include <stdint.h>
//cppcheck-suppress misra-c2012-21.6
#include <stdio.h>
#include "mcc_generated_files/crc/crc.h"
#include "crc_settings.h"
#include "print.h"
#include "swap.h"

/**
 *  The use of stdio.h functions does not follow the MISRA 2012 specification.
 * 
 *  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
 * 
 *  Reasoning: printf is required for printing to the terminal in this code example
 */

//Section: Hardware CRC APIs

/**
 * @brief        Configures the module to prepare for calculation
 *               This function 
 * @param[in]    settings - struct that contains all of the boolean variables used in the calculation
 *               of the CRC value
 * @return       none
 */
void configureHardwareCRC(CRC_SETTINGS settings);

/**
 * @brief        Performs the hardware CRC calculation for 16-bit data.
 *               This function performs the hardware CRC calculation for 16-bit data, and then
 *               processes the result based on the settings.
 * @param[in]    settings - struct that contains all of the boolean variables used in the calculation
 *               of the CRC value
 * @param[in]    inputData - The 8-bit data array for which the CRC value is to be calculated
 * @param[in]    inputDataSize - The number of 8-bit words in the input data
 * @return       Returns the calculated CRC value calculated given the input settings
 */
uint32_t calculateHardwareCRC(CRC_SETTINGS settings, uint8_t inputData[], uint8_t inputDataSize);

#endif	/* HARDWARE_CRC_H */