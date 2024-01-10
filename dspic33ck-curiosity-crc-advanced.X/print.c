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
#include "print.h"

#define MAX_LINE_WIDTH 80

#define TEXT_COLOR_RED      "\033""[0;31m"
#define TEXT_COLOR_GREEN    "\033""[0;32m"
#define TEXT_COLOR_YELLOW   "\033""[0;33m"
#define TEXT_COLOR_DEFAULT  "\033""[0m"

void injectError(uint32_t *message)
{
    (void) printf("Error has been created.\r\n");
    
    *message ^= 0x10;
    
    return;
}

static uint8_t processUserInput(uint8_t userInput)
{
    uint8_t result;
    
    switch(userInput)
    {
        case '1':
            crcSettings.isLsb ^= 1;
            result = TOGGLE_SETTINGS;
            break;
            
        case '2':
            crcSettings.isInitialZero ^= 1;
            result = TOGGLE_SETTINGS;
            break;
            
        case '3':
            crcSettings.isReversed ^= 1;
            result = TOGGLE_SETTINGS;
            break;
            
        case '4':
            crcSettings.isFinalXORZero ^= 1;
            result = TOGGLE_SETTINGS;
            break;
            
        case '5':
            crcSettings.isCRC32Bit ^= 1;
            result = TOGGLE_SETTINGS;
            break;
            
        case '6':
            crcSettings.hasTransmissionError ^= 1;
            result = TOGGLE_SETTINGS;
            break;
            
        case '7':
            demoSettings.isDataSubMenuActive = true;
            result = TOGGLE_DATA_SETTINGS;
            break;
            
        case 'c':
        case 'C':
            result = DO_HARDWARE_CRC;
            break;
            
        case 't':
        case 'T':
            result = LEAVE_TRANSMITTER;
            break;
            
        default:
            result = ERROR;
            break;
    }
    
    return result;
}

uint8_t UART_ExecuteMainMenuInput(uint8_t* buffer)
{
    uint8_t currentStatus;
    
    if(demoSettings.isDataSubMenuActive)
    {
        if((buffer[0] > (uint8_t)0x30) && (buffer[0] < (uint8_t)0x35))
        {
            crcSettings.currentData = buffer[0] - (uint8_t)0x31;
            currentStatus = TOGGLE_SETTINGS;
        }
        else
        {
            currentStatus = ERROR;
        }
        
        demoSettings.isDataSubMenuActive = false;
    }
    else
    {
        currentStatus = processUserInput(buffer[0]);
    }
    
    return currentStatus;
}

void print16BitArray(uint8_t array[], uint8_t arrayLength)
{
    uint8_t array16BitLength = arrayLength / (uint8_t)2;
    for (uint8_t word = 0; word < (array16BitLength - (uint8_t)1); word++)
    {
        (void) printf("0x%04X, ", (unsigned short)convert8BitTo16Bit(array[(uint8_t)2 * word], array[((uint8_t)2 * word) + (uint8_t)1]));
    }
    (void) printf("0x%04X\r\n", (unsigned short)convert8BitTo16Bit(array[arrayLength - (uint8_t)2], array[arrayLength - (uint8_t)1]));
    
    return;
}

void print32BitArray(uint8_t array[], uint8_t arrayLength)
{
    uint8_t array32BitLength = arrayLength / (uint8_t)4;
    for (uint8_t word = 0; word < (array32BitLength - (uint8_t)1); word++)
    {
        (void) printf("0x%08lX, ", (unsigned long)convert8BitTo32Bit(array[(uint8_t)4 * word],
                array[((uint8_t)4 * word) + (uint8_t)1], 
                array[((uint8_t)4 * word) + (uint8_t)2],
                array[((uint8_t)4 * word) + (uint8_t)3]));
    }
    (void) printf("0x%08lX\r\n", (unsigned long)convert8BitTo32Bit(array[arrayLength - (uint8_t)4], 
                                                            array[arrayLength - (uint8_t)3], 
                                                            array[arrayLength - (uint8_t)2], 
                                                            array[arrayLength - (uint8_t)1]));
    
    return;
}

void printSeparator(uint8_t length)
{
    for(uint8_t count = 0; count < length; count++)
    {
        (void) printf("*");
    }
    (void) printf("\r\n");
}

void printDataSubMenu(CRC_SETTINGS settings, uint8_t* inputData[], uint8_t inputDataSizes[])
{
    printSeparator(MAX_LINE_WIDTH);
    (void) printf("Select a data sequence\r\n");
    printSeparator(MAX_LINE_WIDTH);
    
    for(uint8_t i = 0; i < (uint8_t)4; i++)
    {
        if(i == settings.currentData)
        {
            (void) printf("%s --> %d. ", TEXT_COLOR_GREEN, i + (uint8_t)1);
        }
        else
        {
            (void) printf("     %d. ", i + (uint8_t)1);
        }
        
        if(settings.isCRC32Bit)
        {
            print32BitArray(inputData[i], inputDataSizes[i]);
        }
        else
        {
            print16BitArray(inputData[i], inputDataSizes[i]);
        }
        (void) printf("%s", TEXT_COLOR_DEFAULT);
    }
}

void printWithColor(const char* inputString, bool settingValue)
{
    (void) printf("%s%s%-21s%s%s  ", 
            settingValue ? TEXT_COLOR_RED : TEXT_COLOR_GREEN, 
            settingValue ? " " : "[", inputString, 
            settingValue ?  " " : "]", TEXT_COLOR_DEFAULT);
}

void printLine(const char* lineTitle, const char* settingOption1, const char* settingOption2, bool settingValue)
{
    (void) printf("%-23s", lineTitle);
    printWithColor(settingOption1, settingValue);
    printWithColor(settingOption2, !settingValue);
    (void) printf("\r\n");
}

void printLine32BitDependent(const char* lineTitle, bool settingValue, bool is32Bit)
{
    (void) printf("%-23s", lineTitle);
    if(is32Bit)
    {
        printWithColor("0xFFFFFFFF", settingValue);
        printWithColor("0x00000000", !settingValue);
    }
    else
    {
        printWithColor("0xFFFF", settingValue);
        printWithColor("0x0000", !settingValue);
    }
    (void) printf("\r\n");
}

void printMenu(uint8_t* inputData[], uint8_t inputDataSize[])
{
    printSeparator(MAX_LINE_WIDTH);
    (void) printf("Current Settings\r\n");
    printSeparator(MAX_LINE_WIDTH);
    
    printLine("1. Shift Direction:", "MSB", "LSB", crcSettings.isLsb);
    
    printLine32BitDependent("2. Initial Value:", crcSettings.isInitialZero, crcSettings.isCRC32Bit);
    
    printLine("3. Reverse Result:", "Not Reversed", "Reversed", crcSettings.isReversed);
    
    printLine32BitDependent("4. Final XOR Value:", crcSettings.isFinalXORZero, crcSettings.isCRC32Bit);
    
    printLine("5. Polynomial:", "CRC-16-CCITT (0x1021)", "CRC-32 (0x04C11DB7)", crcSettings.isCRC32Bit);
    
    (void) printf("6. Inject virtual transmission error: %s%s%s\r\n", TEXT_COLOR_GREEN, 
            crcSettings.hasTransmissionError ? "Yes" : "No", TEXT_COLOR_DEFAULT);
    
    (void) printf("7. Select Data Sequence\r\n   Current: ");
    
    if(crcSettings.isCRC32Bit)
    {
        print32BitArray(inputData[crcSettings.currentData], inputDataSize[crcSettings.currentData]);
    }
    else
    {
        print16BitArray(inputData[crcSettings.currentData], inputDataSize[crcSettings.currentData]);
    }
    
    printSeparator(MAX_LINE_WIDTH);
    
    (void) printf("\r\nType %s1-7%s to toggle the setting. \r\nType %sc%s to calculate using these settings.\r\n", TEXT_COLOR_GREEN, TEXT_COLOR_DEFAULT, TEXT_COLOR_GREEN, TEXT_COLOR_DEFAULT);
    
    if(demoSettings.isHardwareCRCComplete)
    {
        (void) printf("Type %st%s to begin virtual transmission and data validation.\r\n", TEXT_COLOR_GREEN, TEXT_COLOR_DEFAULT);
    }
    
    return;
}

void printTransmission(uint32_t transmission[], uint8_t inputDataSize)
{
    (void) printf("Message: ");
    if(crcSettings.isCRC32Bit)
    {
        for(uint8_t word = 0; word < (inputDataSize / (uint8_t)4); word++)
        {
            uint8_t index = (uint8_t)4 * word;
            (void) printf("0x%08lX, ", (unsigned long)convert8BitTo32Bit(
                    transmission[index], 
                    transmission[index + (uint8_t)1], 
                    transmission[index + (uint8_t)2], 
                    transmission[index + (uint8_t)3]));
        }
        (void) printf("0x%08lX\r\n", (unsigned long)transmission[inputDataSize]);
    }
    else
    {
        for(uint8_t word = 0; word < (inputDataSize / (uint8_t)2); word++)
        {
            uint8_t index = (uint8_t)2 * word;
            (void) printf("0x%04X, ", (unsigned short)convert8BitTo16Bit(transmission[index], transmission[index + (uint8_t)1]));
        }
        (void) printf("0x%04X\r\n", (unsigned short)transmission[inputDataSize]);
    }
}

void printCompareCalculationResults(uint32_t transmittedValue, uint32_t calculatedValue)
{
    (void) printf("Virtual transmission ");
    if (transmittedValue == calculatedValue)
    {
        (void) printf("%svalidated%s. ", TEXT_COLOR_GREEN, TEXT_COLOR_DEFAULT);
        (void) printf(crcSettings.isCRC32Bit ? "0x%08lX == 0x%08lX" : "0x%04lX == 0x%04lX", 
            (unsigned long)transmittedValue, (unsigned long)calculatedValue);
    }
    else
    {
        (void) printf("%sinvalid%s. ", TEXT_COLOR_RED, TEXT_COLOR_DEFAULT);
        (void) printf(crcSettings.isCRC32Bit ? "0x%08lX != 0x%08lX" : "0x%04lX != 0x%04lX", 
            (unsigned long)transmittedValue, (unsigned long)calculatedValue);
    }
    
    (void) printf("\r\n");
}

void printCRCCalculationResult(uint32_t value)
{
    (void) printf("\r\nCRC value: %s0x", TEXT_COLOR_YELLOW);
    (void) printf(crcSettings.isCRC32Bit ? "%08lX" : "%04lX", (unsigned long)value);
    (void) printf("%s\r\n\r\n", TEXT_COLOR_DEFAULT);
}

void printBenchmarkingResults(uint16_t hardwareCRCTimerCount, uint16_t softwareCRCTimerCount, double hardwareToSoftwareTimeRatio) 
{
    uint8_t timerPeriod = 2;
    
    double hardwareCRCCalculationTime = (double) timerCountToMs(hardwareCRCTimerCount, timerPeriod);
    double softwareCRCCalculationTime = (double) timerCountToMs(softwareCRCTimerCount, timerPeriod);
    
    (void) printf("\r\nBenchmarking:\r\n");
    (void) printf("Hardware Time: %.4f ms\n", hardwareCRCCalculationTime);
    (void) printf("Software Time: %.4f ms\n", softwareCRCCalculationTime);
    (void) printf("\r\nThe hardware calculation is %.2fx faster than the software calculation.\r\n", hardwareToSoftwareTimeRatio);
    (void) printf("\r\nThe number of instruction cycles required for the software calculation: %.0lf\r\n", 
            ((double)calculateInstructions(softwareCRCCalculationTime)));
}