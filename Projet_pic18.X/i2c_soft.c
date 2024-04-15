/************************** i2c_soft.c **************************/
/*
 * File:   i2c_soft.c
 * Author: Nathanaël BLAVO BALLARIN
 *
 * Created on March 23, 2024, 10:59
 */
/******************** Includes Section ********************/
#include "i2c_soft.h"
#include "common.h"
/**********************************************************/

//Function implementations
//----------------------------------------------------------------------------

/**
 * @brief Delays the I2C communication for a specific time.
 */
void I2C_Delay(){
    
	__delay_us(25);		//A delay may or may not be needed here.
	}	

/**
 * @brief Sets the SCL line to low.
 */
void SCL_Low()
        {
	SCL_PORT = SCL_PORT & SCL_OFF_MASK;
	SCL_TRIS = SCL_TRIS & SCL_OFF_MASK;
	}	

/**
 * @brief Sets the SCL line to high.
 */
void SCL_High(){	//Really, just release it
	SCL_TRIS = SCL_TRIS | SCL_ON_MASK;
	}	

/**
 * @brief Sets the SDA line to low.
 */
void SDA_Low(){
	SDA_PORT = SDA_PORT & SDA_OFF_MASK;
	SDA_TRIS = SDA_TRIS & SDA_OFF_MASK;
	}	

/**
 * @brief Sets the SDA line to high.
 */
void SDA_High(){	//Really, just release it
	SDA_TRIS = SDA_TRIS | SDA_ON_MASK;
	}	

/**
 * @brief Sends a zero bit over I2C.
 */
void I2C_SendZero(){
	SCL_Low();			//Should already be low
	SDA_Low();			//zero
	SCL_High();			//clock it out
	I2C_Delay();
	SCL_Low();			//clock idle
	}	

/**
 * @brief Sends a one bit over I2C.
 */
void I2C_SendOne(){
	SCL_Low();			//Should already be low
	SDA_High();			//one
	SCL_High();			//clock it out
	I2C_Delay();
	SCL_Low();			//clock idle
	}	

/**
 * @brief Sends the I2C start condition.
 */
void I2C_Start(){
	SDA_High();			//prepare
	SCL_High();			//prepare
	I2C_Delay();
	SDA_Low();			//Start signal
	I2C_Delay();
	SCL_Low();			//Clock low to prepare for transmission
	I2C_Delay();
	}	

/**
 * @brief Sends the I2C stop condition.
 */
void I2C_Stop(){
	SCL_Low();			//prepare
	I2C_Delay();
	SDA_Low();			//prepare
	I2C_Delay();
	SCL_High();			//Stop signal
	I2C_Delay();
	SDA_High();			//Raise SDA to prepare for next Start signal
	I2C_Delay();
	}	

/**
 * @brief Writes a byte over I2C.
 * @param theByte The byte to be written.
 */
void I2C_WriteByte(char theByte){
	char temp = theByte;
	//Send the bits, MSB first (mask them off with 0x80)
	for(int x=0;x<8;x++){
		if((temp & 0x80) == 0x80){I2C_SendOne();}
			else{I2C_SendZero();}
		temp = temp << 1;
		}//for x
	//Send an ack clock bit (and ignore the result)
	I2C_SendOne();		
	//**DO NOT SEND A STOP HERE; NOT NECESSARILY DONE WITH THE TRANSACTION**
	}	

/**
 * @brief Reads the result of an I2C operation.
 * @return The result of the I2C operation.
 */
char I2C_ReadResult(){
	//Device already addressed in Read mode; just clock in the data...
	char readback = 0x00;
    SDA_COPY_TRIS |= SDA_COPY_MASK;
	//Send eight clock pulses, reading the data bus after each one
	SDA_High();	//Just to be sure
	for(int x=0;x<8;x++){
		readback = (readback << 1) & 0xFE;	//Mask off the new LSB
		SCL_High();
		 			//Check if line is high
		if((SDA_COPY_PORT & SDA_COPY_MASK)){readback++;}
		I2C_Delay();
		SCL_Low();
		I2C_Delay();
		}	//for x
	SDA_Low();			//To prepare for stop condition
	I2C_Delay();
	SCL_High();
	 		//Check if line really did go high
	I2C_Delay();

	SDA_High();			//Stop condition
	return(readback);
	}

/**
 * @brief Reads the result of an I2C operation with an acknowledgment.
 * @return The result of the I2C operation.
 */
char I2C_ReadResult_withAck(){
	//Device already addressed in Read mode; just clock in the data...
	char readback = 0x00;
    SDA_COPY_TRIS |= SDA_COPY_MASK;
	//Send eight clock pulses, reading the data bus after each one
	SDA_High();	//Just to be sure
	for(int x=0;x<8;x++){
		readback = (readback << 1) & 0xFE;	//Mask off the new LSB
		SCL_High();
		 			//Check if line is high
		if(SDA_COPY_PORT & SDA_COPY_MASK){readback++;}
		I2C_Delay();
		SCL_Low();
		I2C_Delay();
		}	//for x
	SDA_Low();			
	I2C_Delay();
	SCL_High();
 
	I2C_Delay();
    SDA_Low();
    I2C_Delay();
    SCL_Low();


	return(readback);
	}	

/**
 * @brief Writes to a specific I2C device address.
 * @param deviceAddress The address of the device to write to.
 */
void I2C_WriteToAddress(char deviceAddress){
	char temp;
	temp = deviceAddress << 1;	//Shift it over one bit
	temp = temp & 0xFE;		//Ensure LSB == 0
	I2C_WriteByte(temp);
	}

/**
 * @brief Reads from a specific I2C device address.
 * @param deviceAddress The address of the device to read from.
 */
void I2C_ReadFromAddress(char deviceAddress){
	I2C_WriteByte((deviceAddress << 1) | 0x01); // 7-bit address, then a 1
	}

/**
 * @brief Reads a register from a specific I2C device.
 * @param deviceAddress The address of the device.
 * @param registerAddress The address of the register.
 * @return The value of the register.
 */
char I2C_ReadRegister(char deviceAddress, char registerAddress){
	char result;
	//Write to the device, giving it the memory address to access
	I2C_Start();
	I2C_WriteToAddress(deviceAddress);
	I2C_WriteByte(registerAddress);

	//Read from the device at the memory address given above
	I2C_Start();
	I2C_ReadFromAddress(deviceAddress);
	result = I2C_ReadResult();
	return(result);
	}

/**
 * @brief Writes a value to a register of a specific I2C device.
 * @param deviceAddress The address of the device.
 * @param registerAddress The address of the register.
 * @param data The data to write to the register.
 */
void I2C_WriteRegister(char deviceAddress, char registerAddress, char data){
	char result;
	//Write to the device, giving it the memory address to access
	I2C_Start();
	I2C_WriteToAddress(deviceAddress);
	I2C_WriteByte(registerAddress);
	I2C_WriteByte(data);
	I2C_Stop();
	}
