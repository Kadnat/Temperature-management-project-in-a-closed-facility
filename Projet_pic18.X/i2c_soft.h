#ifndef i2c_soft_H
#define	i2c_soft_H

    #define SCL_PORT PORTA		//The port where the SCL pin is located
    #define SCL_TRIS TRISA		//The tristate register for the SCL bit
    #define SCL_ON_MASK  0b00010000	//This should OR-mask ON the SCL bit
    #define SCL_OFF_MASK 0b11101111	//This should AND-mask OFF the SCL bit

    #define SDA_PORT PORTA
    #define SDA_TRIS TRISA
    #define SDA_COPY_PORT PORTD
    #define SDA_COPY_TRIS TRISD
    #define SDA_COPY_MASK 0b00000001
    #define SDA_ON_MASK  0b00100000	//This should OR-mask ON the SDA bit
    #define SDA_OFF_MASK 0b11011111	//This should AND-mask OFF the SDA bit



//User functions implemented.
//------------------------------------------------------------------
char I2C_ReadRegister(char deviceAddress, char registerAddress);
void I2C_WriteRegister(char deviceAddress, char registerAddress, char data);


//Support functions implemented:
//------------------------------------------------------------------
void I2C_Delay();				//Adjust as needed
void SCL_Low();					//Asserts SCL low
void SCL_High();				//Releases SCL to float high
void SDA_Low();					//Asserts SDA low
void SDA_High();				//Releases SDA to float high
void I2C_SendZero();				//Sends a "zero" bit
void I2C_SendOne();				//Sends a "one" bit
void I2C_Start();				//Sends the I2C start sequence
void I2C_Stop();				//Sends the I2C stop sequence



void I2C_WriteByte(char);			//Writes a byte to the I2C bus
char I2C_ReadResult();				//Clocks in a character
char I2C_ReadResult_withAck();
void I2C_WriteToAddress(char deviceAddress);	//Selects a device for writing
void I2C_ReadFromAddress(char deviceAddress);	//Selects a device for reading
void I2C_Test();                       //send from 0 to 255 in i2c for test


#endif


