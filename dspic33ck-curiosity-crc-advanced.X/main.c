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

#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/timer/tmr1.h"
#include "mcc_generated_files/uart/uart1.h"
#include "crc_settings.h"
#include "demo_settings.h"
#include "hardware_crc.h"
#include "print.h"
#include "software_crc.h"

#define MAX_COMMAND_LENGTH 2
#define MAX_DATA_SIZE 16

const static struct UART_INTERFACE *UartSerial = &UART1_Drv;

CRC_SETTINGS crcSettings = {
    .hasTransmissionError = false, 
    .isCRC32Bit = false, 
    .isFinalXORZero = false, 
    .isInitialZero = false, 
    .isLsb = false, 
    .isReversed = false,
    .currentData = 0
};

DEMO_SETTINGS demoSettings = {
    .hasSerialMutex = false,
    .isDataSubMenuActive = false,
    .isHardwareCRCComplete = false
};
 
static volatile uint8_t transmitterStatus;

static void UART_ProcessCommand_Callback(void)
{
    static uint8_t buffer[MAX_COMMAND_LENGTH];
    static uint8_t userMessage;
    
    if(UartSerial->IsRxReady())
    {
        userMessage = UartSerial->Read();
        if((userMessage != (uint8_t)0x0A) && (userMessage != (uint8_t)0x0D))
        {
            buffer[0] = userMessage;
        }
        else
        {
            buffer[1] = (uint8_t)0;
            transmitterStatus = UART_ExecuteMainMenuInput(buffer);
            demoSettings.hasSerialMutex = true;
        }
    }
}

int main(void) 
{
    SYSTEM_Initialize();
    UartSerial->RxCompleteCallbackRegister(&UART_ProcessCommand_Callback);
    
    uint8_t data0[2] = {0x6c, 0x93};
    uint8_t data1[8] = {0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31};
    uint8_t data2[16] = {0x2F, 0x2F, 0x20, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 
                     0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x20};
    uint8_t data3[4] = {0x00, 0x00, 0x00, 0x00};

    uint32_t dataSize[4] = {2, 8, 16, 4};
    uint8_t* data[4] = {data0, data1, data2, data3};
    
    uint32_t hardwareCRCTimerCount = 0;
    uint32_t softwareCRCTimerCount = 0;
    double timesRatio;
    
    //transmitter begin
    
    uint32_t transmitterCRCResult = 0;
    uint32_t transmission[MAX_DATA_SIZE + 1];
    
    bool isTransmitterActive = true;
    
    printMenu(data, dataSize);
    
    while(isTransmitterActive)
    {
        if(demoSettings.hasSerialMutex)
        {
            switch(transmitterStatus)
            {
                case TOGGLE_SETTINGS:
                    (void) printf("\r\nSettings have been updated.\r\n\r\n");
                    printMenu(data, dataSize);
                    break;
                    
                case TOGGLE_DATA_SETTINGS:
                    printDataSubMenu(crcSettings, data, dataSize);
                    break;
                    
                case DO_HARDWARE_CRC:
                    (void) printf("\r\nInitial data: ");
                    crcSettings.isCRC32Bit ? 
                            print32BitArray(data[crcSettings.currentData], dataSize[crcSettings.currentData]) :
                            print16BitArray(data[crcSettings.currentData], dataSize[crcSettings.currentData]);
                    
                    configureHardwareCRC(crcSettings);
                    
                    transmitterCRCResult = calculateHardwareCRC(
                            crcSettings, 
                            data[crcSettings.currentData], 
                            dataSize[crcSettings.currentData]);
                    
                    hardwareCRCTimerCount = TMR1_Counter16BitGet();
                    
                    printCRCCalculationResult(transmitterCRCResult);
                    
                    for (uint8_t word = 0; word < dataSize[crcSettings.currentData]; word++)
                    {
                        transmission[word] = data[crcSettings.currentData][word];
                    }
                    transmission[dataSize[crcSettings.currentData]] = transmitterCRCResult;
                        
                    demoSettings.isHardwareCRCComplete = true;
                    printMenu(data, dataSize);
                    
                    break;
                    
                case LEAVE_TRANSMITTER:
                    if(demoSettings.isHardwareCRCComplete)
                    {
                        isTransmitterActive = false;
                        (void) printf("\r\nLEAVING TRANSMITTER. Beginning Virtual Transmission...\r\n");
                        
                        printTransmission(transmission, dataSize[crcSettings.currentData]);
                    }
                    break;
                    
                case ERROR:
                default:
                    (void) printf("\r\nInvalid input. Please make a valid selection.\r\n");
                    printMenu(data, dataSize);
                    break;
            }
            demoSettings.hasSerialMutex = false;
        }
    }
    
    //transmitter end
    
    if(crcSettings.hasTransmissionError)
    {
        injectError(&transmission[0]);
    }
    
    //receiver begin
    
    uint32_t receiverCRCResult = 0;
    uint8_t receiverBuffer[MAX_DATA_SIZE];
    
    (void) printf("\r\nTransmission received:\n");
    printTransmission(transmission, dataSize[crcSettings.currentData]);
    
    for(uint8_t word = 0; word < dataSize[crcSettings.currentData]; word++)
    {
        receiverBuffer[word] = (uint8_t)transmission[word];
    }
    
    configureSoftwareCRC(crcSettings);
    
    receiverCRCResult = calculateSoftwareCRC(
            crcSettings, 
            receiverBuffer, 
            dataSize[crcSettings.currentData]);
    
    softwareCRCTimerCount = TMR1_Counter16BitGet();
    
    (void) printf("\r\nValidating virtual transmission with software calculation.");
    printCRCCalculationResult(transmitterCRCResult);
    
    printCompareCalculationResults(
            receiverCRCResult, 
            transmission[dataSize[crcSettings.currentData]]);
    
    //receiver end
    
    timesRatio = (double)softwareCRCTimerCount / (double)hardwareCRCTimerCount;
    printBenchmarkingResults(hardwareCRCTimerCount, softwareCRCTimerCount, timesRatio);
    
    while(1) {};
}