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
#ifndef SOFTWARECRC_H
#define	SOFTWARECRC_H

#include <stdint.h>
#include "crc_settings.h"
#include "swap.h"

//Section: Software CRC APIs

/**
 * @brief        Sets variables in preparation for software calculation
 *               This function set values that will be used in the software CRC calculation.
 * @param[in]    settings - struct that contains the boolean variables for the settings used to 
 *               configure the values for calculation
 * @param[in]    message[] - The data for which the CRC value will be calculated
 * @param[in]    messageLength - Number of words in the message
 * @return       none
 */
void configureSoftwareCRC(CRC_SETTINGS settings);

/**
 * @brief        Calculate and processes the CRC value for 16-bit data.
 *               This function calculates the CRC value for 16-bit data. It then applies the settings 
 *               to process the value. 
 * @param[in]    settings - struct that contains all boolean variables used in the calculation of 
 *               the CRC value
 * @param[in]    messageLength - Number of words in the message
 * @return       Returns the final 16-bit CRC value after all of the calculation and processing
 */
uint32_t calculateSoftwareCRC(CRC_SETTINGS settings, uint8_t message[], uint8_t messageLength);

#endif	/* SOFTWARECRC_H */