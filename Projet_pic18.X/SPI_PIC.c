/**
 * @file
 * @author Tyler Gamvrelis
 *
 * Created on July 21, 2017, 11:41 AM
 *
 * @ingroup SPI
 */

/********************************* Includes **********************************/
#include "SPI_PIC.h"    

/***************************** Public Functions ******************************/
unsigned char spiTransfer(unsigned char byteToTransfer){   
    // Write byte to buffer. This byte will be transferred to the shift register
    // and transmitted in hardware. As the bits to be transmitted are shifted 
    // out, the bits to be received are shifted in
    SSPBUF = byteToTransfer; 
    
    // Wait until buffer has received a byte and it is latched into the buffer.
    // This will also indicate that the outgoing byte is sent
    while(!SSPSTATbits.BF | !SSPIF){
        continue;
    }

    return SSPBUF;
}

void spiSend(unsigned char val){
    spiTransfer(val);
}

unsigned char spiReceive(void){
    return spiTransfer(0xFF);
}

void spiInit(unsigned char divider){    
    mssp_disable();
    SSPSTAT = 0x00; // Default, data latched/shifted on rising edge
    
    // Configure SSPCON1. Set clock idle state high, and divider as parameter. 
    // Supposedly, the SD card requires that the clock idle state is high, so
    // be careful if you modify this and plan on using the SD card
    switch(divider){
        case 4:
            SSPCON1 = 0b00010000;
            break;
        case 16:
            SSPCON1 = 0b00010001;
            break;
        case 64:
            SSPCON1 = 0b00010010;
            break;
        default:
            SSPCON1 = 0b00010001; // FOSC/16
    }

    // Enforce correct pin configuration for relevant pins
    TRIS_SDO = 0;
    TRIS_SDI = 1;
    TRIS_SCK = 0;
    
    mssp_enable();
}

void SPI_sendResetSequence(void)
{
    //Clear data buffers
    SSPCON1bits.SSPEN = 0; // Disable SPI
    SSPCON1bits.SSPEN = 1; // Enable SPI
    
    //Enable TX and Disable RX
    // Pas de registres spécifiques pour activer TX et désactiver RX sur PIC18F4620
    
    //Clear status bit
    // Pas de registre spécifique pour effacer le bit d'état sur PIC18F4620
    
    //Load Byte 0
    SSPBUF = 0xFF;
    
    //Set data length
    // Pas de registre spécifique pour définir la longueur des données sur PIC18F4620
    
    //Write / Read Index
    uint8_t wIndex = 1;
    
    //While counter is not zero
    while (wIndex < 10)
    {
        if (!SSPSTATbits.BF) // Si le buffer n'est pas plein
        {
            //TX Buffer has space, load next byte (until we hit the LEN)
            SSPBUF = 0xFF;
            wIndex++;
        }
    }
}

void SPI_sendBytes(uint8_t* txData, uint16_t len)
{
    //Write / Read Index
    uint16_t wIndex = 0;
    
    //While counter is not zero
    while (wIndex < len)
    {
        //TX Buffer has space, load next byte (until we hit the LEN)
        spiSend(txData[wIndex]);
        wIndex++;
    }
}

void SPI_exchangeBytes(uint8_t* txData, uint8_t* rxData, uint8_t len)
{
    //Write / Read Index
    uint8_t index = 0;
    
    //While counter is not zero
    while (index < len)
    {
        //TX Buffer has space, load next byte (until we hit the LEN)
        spiSend(txData[index]);
        
        //RX Buffer Ready
        rxData[index] = spiReceive();
        
        index++;
    }
}

//Sends and receives a single byte
uint8_t SPI_exchangeByte(uint8_t data)
{
    uint8_t output = data;
    SPI_exchangeBytes(&output, &output, 1);
    return output;
}

void SPI_receiveBytesTransmitFF(uint8_t* rxData, uint16_t len)
{
    //Write / Read Index
    uint16_t index = 0;
    
    //While counter is not zero
    while (index < len)
    {
        //TX Buffer has space, load next byte
        spiSend(0xFF);
        
        //RX Buffer Ready
        rxData[index] = spiReceive();
        
        index++;
    }
}
