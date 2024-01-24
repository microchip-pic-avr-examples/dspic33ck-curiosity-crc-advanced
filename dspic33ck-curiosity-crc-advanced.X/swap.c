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
#include "swap.h" 

uint8_t reverseByte(uint8_t inputByte)
{
    uint8_t bitsPerByte = 8;
    
    uint8_t result = 0;
    uint8_t inputByteCopy = inputByte;
    
    for(uint8_t bitNum = 0; bitNum < bitsPerByte; bitNum++)
    {
        if((inputByteCopy & (uint8_t)0x01) == (uint8_t)0x01)
        {
            result = result | (1u << (7u - bitNum));
        }
        
        inputByteCopy = (inputByteCopy >> 1);
    }
    
    return result;
}

uint16_t reverse16Bits(uint16_t inputData, bool isFinalXORReversed)
{
    uint16_t ms8Bits = inputData & 0xFF00u;
    uint16_t ls8Bits = inputData & 0x00FFu;
    uint16_t result = 0;
    
    ms8Bits = (ms8Bits >> 8);
    ms8Bits = reverseByte(ms8Bits);
    ls8Bits = reverseByte(ls8Bits);
    
    if(isFinalXORReversed)
    {
        result = (ls8Bits << 8) | ms8Bits;
    }
    else
    {
        result = (ms8Bits << 8) | ls8Bits;
    }
    
    return result;
}

uint32_t reverse32Bits(uint32_t inputData, bool isFinalXORReversed)
{
    uint32_t ms16Bits = inputData & 0xFFFF0000u;
    uint32_t ls16Bits = inputData & 0x0000FFFFu;
    uint32_t result = 0;
    
    ms16Bits = (ms16Bits >> 16);
    ms16Bits = reverse16Bits(ms16Bits, isFinalXORReversed);
    ls16Bits = reverse16Bits(ls16Bits, isFinalXORReversed);
    
    if(isFinalXORReversed)
    {
        result = (ls16Bits << 16) | ms16Bits;
    }
    else
    {
        result = (ms16Bits << 16) | ls16Bits;
    }
    
    return result;
}

uint16_t reverseByteOrder(uint16_t inputData)
{
    uint16_t result = ((inputData << 8) & 0xFF00u) | ((inputData >> 8) & 0x00FFu);
    
    return result;
}

uint32_t reverseByteOrder32Bit(uint32_t inputData)
{
    uint32_t ms16Bits = inputData & 0xFFFF0000u;
    uint32_t ls16Bits = inputData & 0x0000FFFFu;
    uint32_t result = 0;
    
    ms16Bits = (ms16Bits >> 16);
    ms16Bits = reverseByteOrder(ms16Bits);
    ls16Bits = reverseByteOrder(ls16Bits);
    
    result = (ls16Bits << 16) | ms16Bits;
    
    return result;
}

static uint32_t convert16BitTo32Bit(uint16_t byte1, uint16_t byte2)
{
    uint32_t result = byte1;
    result = (result << 16) | byte2;
    return result;
}

uint16_t convert8BitTo16Bit(uint8_t byte1, uint8_t byte2)
{
    uint16_t result = byte1;
    result = (result << 8) | byte2;
    return result;
}

uint32_t convert8BitTo32Bit(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    uint16_t ms16Bits = convert8BitTo16Bit(byte1, byte2);
    uint16_t ls16Bits = convert8BitTo16Bit(byte3, byte4);
    uint32_t result = convert16BitTo32Bit(ms16Bits, ls16Bits);
    return result;
}