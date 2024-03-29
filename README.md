<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture>

# dsPIC33CK Curiosity CRC Advanced Code Example

**Basic Code Example for MCC Melody CRC Module**: [dsPIC33CK Curiosity CRC Basic Code Example](https://github.com/microchip-pic-avr-examples/dspic33ck-curiosity-crc-basic)

The code example uses the [MPLAB® Code Configurator Melody CRC Driver](https://www.npmjs.com/package/@mchp-mcc/crc-16bit-driver) to configure the CRC module using settings determined at runtime. These settings can be toggled with a menu that is displayed in the terminal. This will calculate the CRC value with a hardware implementation utilizing the MCC Melody CRC Module driver and a software implementation. These two values are then compared to verify the calculation. The calculations are also timed, and the results are printed at the end, along with other benchmarking information.

# Related Documentation

[MCC Melody CRC API Reference](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=CRC_16BIT_MELODY_DRIVER&version=latest&redirect=true)

# Software Used

## Required Software

- MPLAB® X IDE **6.15** or newer (https://www.microchip.com/MPLABXIDE)
- MPLAB® XC16 Compiler **2.10** or a newer compiler (https://www.microchip.com/xc16)
- MPLAB® Code Configurator (MCC) Plugin **5.4.1** or newer (https://www.microchip.com/mcc)
- MPLAB® Code Configurator (MCC) Core **5.6.1** or newer (https://www.microchip.com/mcc)
- MPLAB® Code Configurator (MCC) Melody **2.6.1** or newer (https://www.microchip.com/melody)
- CRC Driver **1.0.3** or newer (MCC Content Manager)
- CRC PLIB **1.3.0** or newer (MCC Content Manager)
- UART Driver **1.10.0** or newer (MCC Content Manager)
- UART PLIB **1.5.0** or newer (MCC Content Manager)
- Any terminal program, like MPLAB® Data Visualizer (https://www.microchip.com/datavisualizer) or Tera Term (https://ttssh2.osdn.jp/index.html.en)

# Hardware Used

## Required Hardware

- [dsPIC33CK Curiosity Development Board (dsPIC33CK256MP508)](https://www.microchip.com/en-us/development-tool/DM330030)

# Hardware Setup

1. Connect the board to the PC using a USB cable.

    ![Hardware Setup Image](images/hardware_setup.jpg)

# Software Setup

## Terminal Setup (Data Visualizer)

1. Launch the MPLAB® Data Visualizer.

    ![Data Visualizer Icon](images/data_visualizer_icon.JPG)

2. Find the correct COM Port from the list on the left and click the play button.

    ![COM Port list withplay button highlighted](images/dv_COM_select_play_highlighted.JPG)

3. Select the "Send to Terminal" button.

    ![Send to Terminal button](images/dv_data_capture_menu.JPG)

4. Click on the settings icon next to the source dropdown in the input section.

    ![Source Settings Icon](images/dv_source_settings_button.JPG)

5. Verify that the serial port settings match the following:

    ![Source Settings Menu](images/dv_source_settings.JPG)

## Terminal Setup (Tera Term)
1. Launch Tera Term
2. Go to File -> New Connection.
3. Select the "Serial" option and select the correct COM Port from the dropdown menu.

    ![COM port selection](images/tera_term_COM_port_selection.JPG)

4. Go to Setup -> Serial port and ensure that the settings match the following:

    ![Tera Term Serial Port Settings](images/tera_term_serial_port_menu.JPG)

# CRC Settings

The CRC calculations performed in this code example have different settings that can be toggled at runtime. **Note:** Settings toggled at runtime will NOT be reflected in the MCC Melody CRC driver or UI.

- Shift direction: The direction the data is shifted through the registers. Can start with the most significant bit (MSB) or the least significant bit (LSB)
- Initial Value: The initial value of the CRC result. Can theoretically be anything, but traditionally is either zero (0x0000 for 16-bit or 0x00000000 for 32-bit) or -1 (0xFFFF for 16-bit or 0xFFFFFFFF for 32-bit).
- Reverse: The result of the CRC calculation can be reversed immediately after calculation. Reverse operation is performed if the flag is true.
- Final XOR Value: The value XOR'd with the CRC result after the calculation and any reverses. Can theoretically be anything, but traditionally is either zero (0x0000 for 16-bit or 0x00000000 for 32-bit) or -1 (0xFFFF for 16-bit or 0xFFFFFFFF for 32-bit).
- Polynomial: Generator polynomial used in the calculation of the CRC value. Many effective polynomials exist. This code example uses the polynomials for CRC-16-CCITT (0x1021) and CRC-32 (0x04C11DB7).
- Data: The input data for which the CRC value is calculated. Can be any value of any length. Four data strings can be used in this code example:
  - 0x6c, 0x93 (default value configured in MCC Melody CRC Driver Simulator)
  - 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31 ("87654321")
  - 0x2F, 0x2F, 0x20, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x20 ("// Hello world! ")
  - 0x00, 0x00, 0x00, 0x00 (All null characters)

## Online Calculator

Online calculators can be used to test different configurations and try different settings. Most developers compare results with an online calculator. An example that was used in the development of this code example is the [Online Calculator by Sven Reifegerste (Zorc)](http://www.zorc.breitbandkatze.de/crc.html).

### Online Calculator:

![Labeled Online Calculator](images/online_calculator_UI.JPG)

**NOTE: Number 8, Seed Shift Direction, is assumed to be MSB in this online calculator.**

### MCC Melody: 

![Labeled MCC Melody UI](images/demo_CRC_settings.JPG)

**NOTE: The "Reverse CRC value" and "Final XOR Value" only work for the simulator, they will not be used in the firmware calculations.**

![Labeled CRC_CalculationResultGet()](images/CRC_CalculationResultGet.JPG)

The firmware function CRC_CalculationGet() is where the Reverse and Final XOR Value settings are implemented.

# Operation

1. Open the dspic33ck-curiosity-crc-advanced project in MPLAB® X IDE.

2. Open the terminal program that will be used. 

3. In the MPLAB® X IDE, build and program the device. Within the terminal program a menu will be printed out displaying the settings. Below the menu there are two prompts for entering inputs. These single character inputs can be typed into the terminal and are entered by pressing the enter key.

    ![CRC Advanced Code Example Menu](images/advanced_main_menu.JPG)

    Menu Options

    | Number | Menu Setting | MCC Melody CRC Setting | Option 1 | Option 2 |
    | --- | --- | --- | --- | --- |
    | 1 | Shift Direction | Shift Direction | MSB | LSB | 
    | 2 | Initial Value | Initial Value | -1 (0xFFFF/0xFFFFFFFF) | 0 (0x0000/0x00000000) |
    | 3 | Reverse Result | Reverse | Not Reversed | Reversed |
    | 4 | Final XOR Value | Final XOR Value | -1 (0xFFFF/0xFFFFFFFF) | 0 (0x0000/0x00000000) |
    | 5 | Polynomial | Polynomial | CRC-16-CCITT (0x1021) | CRC-32 (0x04C11DB7) |
    | 6 | Inject virtual transmission error **\*** | N/A | No | Yes |
    | 7 | Select Data Sequence | Data | Data Submenu | N/A |

    **\*** Inject virtual transmission error - allows the user to force an error in the virtual transmission. It XOR's the first byte by 0x10. This can be used to demonstrate the CRC's error detection capabilities.

4. The settings can be toggled by entering the number associated with the setting.
   
   Enter "7" to print the data submenu. Here, the user can select the data sequence to be used in the calculations. Using this menu, enter "2" to switch to the second data sequence.

    ![CRC Advanced Demo Data Sub-menu](images/advanced_data_submenu.JPG)

5. Enter "c" to calculate. This performs a CRC calculation using the MCC Melody CRC module with the current selections in the menu. The data being used is then reprinted to the terminal, along with the result of the hardware CRC calculation:

    ![Hardware CRC Result](images/advanced_hardware_crc_output.JPG)

    The menu is also reprinted with a new option: "Type t to begin virtual transmission and data validation."

6.  Enter "t" to continue.

7. The terminal will now print the process for virtual transmission. 
    - The virtually transmitted message is comprised of the previous hardware CRC calculation's "Initial Data" with the "CRC Value" appended to the end. 
    - The virtual receiver will then print the message that was received. Any errors created during transmission (caused by toggling option 6 in the main menu to be "Yes") will show up here.
    - The virtual receiver will perform a software CRC calculation on the "Initial Data" (NOT including the appended CRC value) received with the current settings selected. It then prints the CRC value calculated.

    ![Transmission and Software CRC Result](images/advanced_software_crc_result.JPG)

8. The software CRC calculation result is compared with the value of the hardware CRC result from the virtual message. It will print whether or not the transmission has been validated. 
   
    - Successful Transmission:

        ![Valid transmission](images/advanced_transmission_valid.JPG)

    - Invalid Transmission:

        ![Invalid transmission](images/advanced_transmission_invalid.JPG)

9. After virtual transmission is complete Benchmarking results will be printed. This section and the printed values are explained further in the **Benchmarking** section below.
   
    ![Benchmarking Results (Default Path)](images/advanced_benchmarking.JPG)

10. After the benchmarking is printed the program will end. **To start over press the clear (or MCLR) button on the board.**

# Benchmarking

Benchmarking timing results are recorded using the MCC Melody Timer module while the hardware and software CRC values are being calculated. This is where a comparison of the performance of both CRC implementations are made. Additionally, the number of instruction cycles is printed for the software implementation.

**NOTE: Faster and more complex software CRC implementations exist compared to the one used in this demo's firmware.**

The benchmarking results are dependent on the size of the input data and optimization levels. For example, for smaller data sequences the software implementation will be faster than hardware. However, for large data inputs, the hardware implementation will be much faster.

## Compiler Optimization

The MPLAB® XC16 compiler provides users with compiler optimization options. These options allow the user to speed up their code. This CRC Advanced Demo is functional at all optimization levels. 

The optimization settings can be found in the project properties. Go to Project Properties -> XC16 -> Optimization Level to change optimization level for the project.

The table below shows the optimization results for hardware and software CRC calculations using data vector {0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31} with default settings.

| Optimization Level | Hardware CRC | Software CRC |
|---|---|---|
| 0 |0.0068 ms|0.0190 ms|
| 1 |0.0048 ms|0.0109 ms|
| 2* |0.0041 ms|0.0083 ms|
| s** |0.0050 ms|0.0116 ms|
| 3** |0.0041 ms|0.0062 ms|

*Project is set to this level by default

**MPLAB® XC16 PRO optimizations