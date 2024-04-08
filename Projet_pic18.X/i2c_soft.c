#include "i2c_soft.h"
#include "common.h"

//Function implementations
//----------------------------------------------------------------------------
void I2C_Delay(){
    
	__delay_us(10);		//A delay may or may not be needed here.
	}	



void SCL_Low()
        {
	SCL_PORT = SCL_PORT & SCL_OFF_MASK;
	SCL_TRIS = SCL_TRIS & SCL_OFF_MASK;
	}	

void SCL_High(){	//Really, just release it
	SCL_TRIS = SCL_TRIS | SCL_ON_MASK;
	}	

void SDA_Low(){
	SDA_PORT = SDA_PORT & SDA_OFF_MASK;
	SDA_TRIS = SDA_TRIS & SDA_OFF_MASK;
	}	

void SDA_High(){	//Really, just release it
	SDA_TRIS = SDA_TRIS | SDA_ON_MASK;
	}	





void I2C_SendZero(){
	SCL_Low();			//Should already be low
	SDA_Low();			//zero
	SCL_High();			//clock it out
	I2C_Delay();
	SCL_Low();			//clock idle
	}	

void I2C_SendOne(){
	SCL_Low();			//Should already be low
	SDA_High();			//one
	SCL_High();			//clock it out
	I2C_Delay();
	SCL_Low();			//clock idle
	}	

void I2C_Start(){
	SDA_High();			//prepare
	SCL_High();			//prepare
	I2C_Delay();
	SDA_Low();			//Start signal
	I2C_Delay();
	SCL_Low();			//Clock low to prepare for transmission
	I2C_Delay();
	}	


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
    /*
    SCL_Low();
    I2C_Delay();
    SCL_High();
     */
	SDA_High();			//Stop condition
	return(readback);
	}

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
	SDA_Low();			//To prepare for stop condition
	I2C_Delay();
	SCL_High();
	 		//Check if line really did go high
    
	I2C_Delay();
    SDA_Low();
    I2C_Delay();
    SCL_Low();


	return(readback);
	}	


void I2C_WriteToAddress(char deviceAddress){
	char temp;
	temp = deviceAddress << 1;	//Shift it over one bit
	temp = temp & 0xFE;		//Ensure LSB == 0
	I2C_WriteByte(temp);
	}

void I2C_ReadFromAddress(char deviceAddress){
	I2C_WriteByte((deviceAddress << 1) | 0x01); // 7-bit address, then a 1
	}


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

void I2C_WriteRegister(char deviceAddress, char registerAddress, char data){
	char result;
	//Write to the device, giving it the memory address to access
	I2C_Start();
	I2C_WriteToAddress(deviceAddress);
	I2C_WriteByte(registerAddress);
	I2C_WriteByte(data);
	I2C_Stop();
	}

void I2C_Test(void){

    I2C_Start();
    for (int i = 0; i != 256; i++) {
        I2C_WriteByte(i); //address
    }
    I2C_Stop();
}
