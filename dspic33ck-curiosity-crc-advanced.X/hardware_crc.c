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

#include "hardware_crc.h"

#define MAX_DATA_SIZE 8

static void initialize32Bits(void)
{
    uint32_t polynomialXOR32Bits = 0x04C11DB7u;
    uint16_t polynomialWidth32Bits = 32;

    CRCXORL = (uint16_t)polynomialXOR32Bits;
    CRCXORH = (uint16_t)(polynomialXOR32Bits >> 16);
    CRCCONHbits.PLEN = polynomialWidth32Bits - (uint16_t)1;
}

static void setInitialValue(bool isCRC32Active, bool isInitialZero)
{
    uint16_t indirect16BitData = 0x84CFu;
    uint32_t indirect32BitData = 0x46AF6449u;
    
    if (isInitialZero) {
        CRCWDATH = 0;
        CRCWDATL = 0;
    }
    else 
    {
        if(isCRC32Active)
        {
            CRCWDATH = (uint16_t)(indirect32BitData >> 16);
            CRCWDATL = (uint16_t)indirect32BitData;
        }
        else
        {
            CRCWDATL = indirect16BitData;
        }
    }
}

void configureHardwareCRC(CRC_SETTINGS settings)
{
    CRC_Initialize();
    
    if(settings.isCRC32Bit)
    {
        initialize32Bits();
    }
    
    CRCCONLbits.LENDIAN = settings.isLsb ? 1 : 0;

    setInitialValue(settings.isCRC32Bit, settings.isInitialZero);
}

uint32_t calculateHardwareCRC(CRC_SETTINGS settings, uint8_t inputData[], uint8_t inputDataSize)
{
    uint32_t crcResult = 0;
    
    CRC_CalculateBufferStart(inputData, inputDataSize);
    
    while(!CRC_CalculationIsDone()) 
    {
        CRC_Tasks();
    }
    
    if(settings.isCRC32Bit)
    {
        crcResult = CRC_CalculationResultGet(settings.isReversed, settings.isFinalXORZero ? 0x00000000ul : 0xFFFFFFFFul);
    }
    else
    {
        crcResult = CRC_CalculationResultGet(settings.isReversed, settings.isFinalXORZero ? 0x0000ul : 0xFFFFul);
    }
    
    return crcResult;
}