/**********************************************************
This is a header file for 12-bit SPI DAC MCP4921 IC.
File contains functions to initialize and write DAC values.
***********************************************************/

#ifndef mcp4921_h
#define mcp4921_h

#include "spi.h"  // Include SPI driver for 8051

// MCP4921 uses a single SPI command (16 bits). CS must be pulled low during transmission.

sbit cs_bar_dac = P1^5;  // Chip Select for MCP4921 (you can change the pin as per your setup)

// Function declarations
void dac_init(void);                    // Initialize the DAC (sets CS high)
void dac_write(unsigned int value);     // Send 12-bit value to DAC (0-4095)


// Function definitions

/**********************************************************
dac_init()

Initializes peripherals specific to MCP4921 IC (slave select)
***********************************************************/
void dac_init(void)
{
    cs_bar_dac = 1;  // CS high = DAC disabled
}

/**********************************************************
dac_write()

Writes 12-bit value (0 to 4095) to MCP4921 DAC.
Control bits: 
 - Buffered = 0 (unbuffered)
 - Gain = 1 (1x)
 - Shutdown = 1 (active mode)
 => Control nibble = 0x3
***********************************************************/
void dac_write(unsigned int value)
{
    unsigned long int dac_data;
    
    // Ensure value is 12-bit
    value &= 0x0FFF;

    // Create 16-bit word: control bits (0x3 << 12) + 12-bit data
    dac_data = ((unsigned long int)(0x3) << 12) | value;

    // Send over SPI
    cs_bar_dac = 0;                   // Enable DAC (CS low)
    spi_trx(dac_data << 8);           // Shift to send as MSB first over 3-byte SPI
    cs_bar_dac = 1;                   // Disable DAC (CS high)
}

#endif