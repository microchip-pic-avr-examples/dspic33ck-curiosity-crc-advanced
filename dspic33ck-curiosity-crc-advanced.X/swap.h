/*
© [2024] Microchip Technology Inc. and its subsidiaries.

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

#ifndef SWAP_H
#define	SWAP_H

//cppcheck-suppress misra-c2012-21.6
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/**
 *  The use of stdio.h functions does not follow the MISRA 2012 specification.
 * 
 *  (Rule 21.6) REQUIRED: The standard library input/output functions shall not be used
 * 
 *  Reasoning: printf is required for printing to the terminal in this code example
 */

//Section: CRC Swaps APIs

/**
 * @brief        Reverses the order of bits within a byte.
 *               The function takes an input byte and reverses the bits. It returns this 
 *               reversed byte.
 * @param[in]    inputByte - Byte to be reversed
 * @return       Returns the reversed byte
 */
uint8_t reverseByte(uint8_t inputByte);

/**
 * @brief        Performs reversals on a 16-bit input.
 *               This function reverses the bits within the two bytes separately. It can also 
 *               swap the bytes if needed using the boolean input.
 * @param[in]    inputData - 16-bit input data that needs to be reversed/swapped
 * @param[in]    isFinalXORReversed - When true, the CRC value is reversed before the final XOR.
 * @return       Returns the reversed 16-bit value
 */
uint16_t reverse16Bits(uint16_t inputData, bool isFinalXORReversed);

/**
 * @brief        Performs reversals on a 32-bit input.
 *               This function reverses the bits within the four bytes separately. It can also
 *               reverse the order of the bytes if needed using the boolean input.
 * @param[in]    inputData - 32-bit input data that needs to be reversed/swapped
 * @param[in]    isFinalXORReversed - When true, the CRC value is reversed before the final XOR.
 * @return       Returns the reversed 32-bit value
 */
uint32_t reverse32Bits(uint32_t inputData, bool isFinalXORReversed);

/**
 * @brief        Reverses the order of bytes.
 *               This function takes a 16-bit input and reverses the order of the two bytes. NOTE:
 *               This does not reverse BIT order, it reverses BYTE order.
 * @param[in]    inputData - 16-bit data that needs to be reversed
 * @return       Returns a 16-bit value with reversed byte order. 
 */
uint16_t reverseByteOrder(uint16_t inputData);

/**
 * @brief        Reverses the byte order of a 32-bit value.
 * @param[in]    inputData - 32-bit data that needs to be reversed
 * @return       Returns a 32-bit value with the byte order reversed
 */
uint32_t reverseByteOrder32Bit(uint32_t inputData);

/**
 * @brief        Takes two 8-bit integers and combines them into a 16-bit integer.
 * @param[in]    byte1 - 8-bit data that will be the leftmost byte
 * @param[in]    byte2 - 8-bit data that will be the rightmost byte
 * @return       Returns a 16-bit value that is a combination of the two 8 bit values
 */
uint16_t convert8BitTo16Bit(uint8_t byte1, uint8_t byte2);

/**
 * @brief        Takes four 8-bit integers and combines them into a 32-bit integer.
 * @param[in]    byte1 - 8-bit data that will be the leftmost byte
 * @param[in]    byte2 - 8-bit data that will be the second leftmost byte
 * @param[in]    byte3 - 8-bit data that will be the rightmost byte
 * @param[in]    byte4 - 8-bit data that will be the second rightmost byte
 * @return       Returns a 32-bit value that is a combination of the four 8 bit values
 */
uint32_t convert8BitTo32Bit(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

#endif	/* SWAP_H */