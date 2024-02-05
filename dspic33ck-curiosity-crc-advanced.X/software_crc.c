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

#include "mcc_generated_files/timer/tmr1.h"
#include "software_crc.h"

#define MAX_DATA_SIZE 16

static uint32_t softwareCRCResult = 0;
static uint16_t softwareCRC16BitResult = 0;

void configureSoftwareCRC(CRC_SETTINGS settings)
{
    if(settings.isCRC32Bit)
    {
        if(!(settings.isInitialZero))
        {
            softwareCRCResult = 0xFFFFFFFFu;
        }
    }
    else
    {
        if(!(settings.isInitialZero))
        {
            softwareCRC16BitResult = 0xFFFFu;
        }
    }
}

uint32_t calculateSoftwareCRC(CRC_SETTINGS settings, uint8_t message[], uint8_t messageLength)
{    
    uint8_t bits = 8;
    uint16_t xorValue16Bits = 0;
    uint32_t xorValue32Bits = 0;
    
    uint8_t softwareBuffer8Bit[MAX_DATA_SIZE];
    
    uint16_t crc16ccitt_polynomial = 0x1021;
    uint32_t crc32_polynomial = 0x04C11DB7;
    
    Timer1.Initialize();
    
    for(uint8_t word = 0; word < messageLength; word++)
    {
        softwareBuffer8Bit[word] = message[word];
        if(settings.isLsb)
        {
            softwareBuffer8Bit[word] = reverseByte(softwareBuffer8Bit[word]);
        }
    }

    for(uint8_t word = 0; word < messageLength; word++)
    {
        if(settings.isCRC32Bit)
        {
            xorValue32Bits = softwareBuffer8Bit[word];
            xorValue32Bits = (xorValue32Bits << 24);
            softwareCRCResult ^= xorValue32Bits;
            
            for(uint8_t count = 0; count < bits; count++) 
            {
                if(softwareCRCResult >= 0x80000000u)
                {
                    softwareCRCResult = (softwareCRCResult << 1) ^ crc32_polynomial;
                }
                else
                {
                    softwareCRCResult = (softwareCRCResult << 1);
                }
            }
        }
        else
        {
            xorValue16Bits = softwareBuffer8Bit[word];
            xorValue16Bits = (xorValue16Bits << 8);
            softwareCRC16BitResult ^= xorValue16Bits;
            
            for(uint8_t count = 0; count < bits; count++) 
            {
                if(softwareCRC16BitResult >= 0x8000u)
                {
                    softwareCRC16BitResult = (softwareCRC16BitResult << 1) ^ crc16ccitt_polynomial;
                }
                else
                {
                    softwareCRC16BitResult = (softwareCRC16BitResult << 1);
                }
            }
        }
    }
    
    Timer1.Stop();
    
    if(settings.isCRC32Bit)
    {
        if(settings.isReversed)
        {
            softwareCRCResult = reverse32Bits(softwareCRCResult, true);
        }

        if(!settings.isFinalXORZero)
        {
            softwareCRCResult ^= 0xFFFFFFFFul;
        }
    }
    else
    {
        if(settings.isReversed)
        {
            softwareCRC16BitResult = reverse16Bits(softwareCRC16BitResult, true);
        }
        
        if(!settings.isFinalXORZero)
        {
            softwareCRC16BitResult ^= 0xFFFFu;
        }
        
        softwareCRCResult = softwareCRC16BitResult;
    }
    
    return softwareCRCResult;
}