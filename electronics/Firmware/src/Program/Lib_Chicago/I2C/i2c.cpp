/**
* @file i2c.cpp
*
* @brief Chicago I2C communication functions
*
* @copyright
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* @copyright
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* @author Analogix, Inc
* @author Arduino LLC
* @author Adam Munich
*/

//#############################################################################
// Includes
//-----------------------------------------------------------------------------
#include <Wire.h>
#include <stdint.h>

#include "./i2c.h"

#include "../Chicago/chicago_config.h"
#include "../Chicago/chicago.h"	
#include "../Debug/debug.h"


//#############################################################################
// Pre-compiler Definitions
//-----------------------------------------------------------------------------


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
void i2c_flush(){
	while(Wire.available()){
		Wire.read();
	}
}

//-----------------------------------------------------------------------------
int8_t i2c_write_byte(uint8_t SlaveID, uint16_t Offset, uint8_t Data){
	#ifdef DEBUG_LEVEL_4
		TRACE3("i2c_write_byte(uint8_t SlaveID=%02X, uint16_t Offset=%03X, uint8_t Data=%02X)\n", SlaveID, Offset, Data);
	#endif
	
	// Check SlaveId and Offset
	if(((SlaveID & 0x0F) != 0) && ((Offset&0xFF00) != 0) || ((Offset&0xF000) != 0)) {
		return RETURN_FAILURE_VALUE;
	}

	if(RETURN_NORMAL_VALUE == WriteReg(CHICAGO_SLAVEID_ADDR, 0x00, (SlaveID | (uint8_t)(( Offset & 0x0F00) >> 8)))) {
		if(RETURN_NORMAL_VALUE == WriteReg(CHICAGO_OFFSET_ADDR, (uint8_t)(Offset & 0x00FF), Data)) {
			return RETURN_NORMAL_VALUE;
		}
	}
	
	#ifdef DEBUG_LEVEL_2
		TRACE3("\tI2C write byte ERROR!! %02X %03X %02X\n", SlaveID, Offset, Data);
	#endif
	
	return RETURN_FAILURE_VALUE;
}

//-----------------------------------------------------------------------------
int8_t i2c_write_byte_keep(uint16_t Offset, uint8_t Data){
	#ifdef DEBUG_LEVEL_4
		TRACE2("i2c_write_byte_keep(uint16_t Offset=%03X, uint8_t Data=%02X)\n", Offset, Data);
	#endif
	
	if(RETURN_NORMAL_VALUE == WriteReg(CHICAGO_OFFSET_ADDR, Offset, Data)) {
		return RETURN_NORMAL_VALUE;
	}
	
	#ifdef DEBUG_LEVEL_2
		TRACE2("\tI2C write offset data ERROR!! %03X %02X\n", Offset, Data);
	#endif
	
	return RETURN_FAILURE_VALUE;
}

//-----------------------------------------------------------------------------
int8_t i2c_write_byte4(uint8_t SlaveID, uint16_t Offset, uint32_t Data){
	
	#ifdef DEBUG_LEVEL_4
		TRACE3("i2c_write_byte4(uint8_t SlaveID=%02X, uint16_t Offset=%03X, uint32_t Data=%08X)\n", SlaveID, Offset, Data);
	#endif

	// Check SlaveId and Offset
	if(((SlaveID & 0x0F)!=0)&&((Offset&0xFF00)!=0)||((Offset&0xF000)!=0)) {
		#ifdef DEBUG_LEVEL_2
		TRACE3("\tI2C SlaveID Offset ERROR!! %02X %03X %08X\n", SlaveID, Offset, Data);
		#endif
		return RETURN_FAILURE_VALUE;
	}

	if(RETURN_NORMAL_VALUE == WriteReg(CHICAGO_SLAVEID_ADDR, 0x00, (SlaveID | (uint8_t)((Offset & 0x0F00) >> 8)) )) {
		if(RETURN_NORMAL_VALUE == WriteReg4(CHICAGO_OFFSET_ADDR, (uint8_t)(Offset & 0x00FF), Data)) {
			return RETURN_NORMAL_VALUE;
		}
		else{
			#ifdef DEBUG_LEVEL_2
			TRACE3("\tI2C write OFFSET ERROR!! %02X %03X %08X\n", SlaveID, Offset, Data);
			#endif
			return RETURN_FAILURE_VALUE;
		}
	}
	else{
		#ifdef DEBUG_LEVEL_2
		TRACE3("\tI2C write SLAVEID ERROR!! %02X %03X %08X\n", SlaveID, Offset, Data);
		#endif
		return RETURN_FAILURE_VALUE;
	}
	
}

//-----------------------------------------------------------------------------
int8_t i2c_read_byte(uint8_t SlaveID, uint16_t Offset, uint8_t *pData){
	#ifdef DEBUG_LEVEL_4
		TRACE3("i2c_read_byte(uint8_t SlaveID=%02X, uint16_t Offset=%04X, uint8_t pData=%02X)\n", SlaveID, Offset, pData);
	#endif
	
	// Check SlaveId and Offset
	if(((SlaveID & 0x0F)!=0) && ((Offset & 0xFF00) != 0) || ((Offset & 0xF000) != 0)) {
		*pData = -1;
		return RETURN_FAILURE_VALUE;
	}
	
	if(RETURN_NORMAL_VALUE == WriteReg(CHICAGO_SLAVEID_ADDR, 0x00, (SlaveID | (uint8_t)((Offset & 0x0F00) >> 8)))) {
		if(RETURN_NORMAL_VALUE == ReadReg(CHICAGO_OFFSET_ADDR, (uint8_t)(Offset & 0x00FF), pData)) {
			return RETURN_NORMAL_VALUE;
		}
	}
	
	#ifdef DEBUG_LEVEL_2
	TRACE1("\t%s\n", "I2C FAIL");
	#endif
	
	*pData = -1;
	return RETURN_FAILURE_VALUE;
}

//-----------------------------------------------------------------------------
int8_t i2c_read_block(uint8_t SlaveID, uint16_t Offset, uint8_t *pData, uint32_t Length){
	// Check SlaveId and Offset
	if(((SlaveID & 0x0F) !=0) && ((Offset & 0xFF00) != 0) || ((Offset & 0xF000) != 0)) {
		return RETURN_FAILURE_VALUE;
	}

	if(RETURN_NORMAL_VALUE == WriteReg(CHICAGO_SLAVEID_ADDR, 0x00, (SlaveID | (uint8_t)((Offset & 0x0F00) >> 8)) )) {
		if(RETURN_NORMAL_VALUE == ReadBlockReg(CHICAGO_OFFSET_ADDR, (uint8_t)(Offset & 0x00FF), Length, pData)) {
			return RETURN_NORMAL_VALUE;
		}
	}
	
	return RETURN_FAILURE_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc ReadReg
static int8_t ReadReg(uint8_t DevAddr, uint16_t RegAddr, uint8_t *pData){
 
	// 7 bit address
	DevAddr = (DevAddr >> 1);
 
 	i2c_flush(); //?Needed? 
 	
 	Wire.beginTransmission(DevAddr);	// if(I2C_WriteByte(DevAddr | I2C_WRITE) == I2C_ACK)
 	Wire.write(RegAddr);				// if(I2C_WriteByte(RegAddr) == I2C_ACK)
 	Wire.endTransmission(false);		// I2C_Restart();
 	
 	Wire.requestFrom(DevAddr, 1);		//  if(I2C_WriteByte(DevAddr | I2C_READ) == I2C_ACK)
	 
 	uint8_t ack = false;
 	
 	while(Wire.available()){			// *pData = I2C_ReadByte(I2C_NACK);
	 	*pData = Wire.read();
	 	ack = true;
 	}
 	
 	if(ack == true){
	 	return RETURN_NORMAL_VALUE;
	}else{
	 	*pData = -1;
	 	return RETURN_FAILURE_VALUE;
 	}
}

//-----------------------------------------------------------------------------
/// @copydoc ReadReg4
static int8_t ReadReg4(uint8_t DevAddr, uint16_t RegAddr, uint32_t *pData){
	
	// 7 bit address
	DevAddr = (DevAddr >> 1);

	i2c_flush(); //?Needed?

	uint8_t reg_temp;

 	Wire.beginTransmission(DevAddr);
 	Wire.write(RegAddr);
 	Wire.endTransmission(false);
 	
 	Wire.requestFrom(DevAddr, 4);
 	
 	uint8_t ack = false;
 	
 	if(Wire.available()){	 	
		reg_temp = Wire.read();
		*pData = (((uint32_t) reg_temp) & 0x000000FF);
		reg_temp = Wire.read();
		*pData = *pData | ((((uint32_t) reg_temp) << 8) & 0x0000FF00);
		reg_temp = Wire.read();
		*pData = *pData | ((((uint32_t) reg_temp) << 16) & 0x00FF0000);
		reg_temp = Wire.read();
		*pData = *pData | ((((uint32_t) reg_temp) << 24) & 0xFF000000);
		 
		ack = true;
 	}
 	
 	if(ack == true){
	 	return RETURN_NORMAL_VALUE;
	 	}else{
	 	*pData = -1;
	 	return RETURN_FAILURE_VALUE;
 	}
}

//-----------------------------------------------------------------------------
/// @copydoc ReadBlockReg
static int8_t ReadBlockReg(uint8_t DevAddr, uint16_t RegAddr, uint8_t n, uint8_t *pBuf){

	// 7 bit address
	DevAddr = (DevAddr >> 1);

	i2c_flush(); //?Needed?

	uint8_t reg_temp;

 	Wire.beginTransmission(DevAddr);
 	Wire.write(RegAddr);
 	Wire.endTransmission(false);
	 
 	Wire.requestFrom(DevAddr, n);
 	
 	uint8_t ack = false;
 	
 	while(Wire.available()){	 	
		*pBuf = Wire.read();
		pBuf++;
		 
		ack = true;
 	}
 	
 	if(ack == true){
	 	return RETURN_NORMAL_VALUE;
	 }
	 else{
		do {
			*pBuf = -1;
			pBuf++;
		}while(--n);
	 
		return RETURN_FAILURE_VALUE;
 	}	
}

//-----------------------------------------------------------------------------
/// @copydoc WriteReg
static int8_t WriteReg(uint8_t DevAddr, uint16_t RegAddr, uint8_t RegVal){
	
	// 7 bit address
	DevAddr = (DevAddr >> 1);
	
	Wire.beginTransmission((uint8_t)(DevAddr));
	Wire.write((uint8_t)(RegAddr));
	Wire.write((uint8_t)(RegVal));
	
	uint8_t result = Wire.endTransmission();
	
	if(result == 0){	// Ack
		return RETURN_NORMAL_VALUE;
	}else{				// Nack
		return RETURN_FAILURE_VALUE;
	}
}

//-----------------------------------------------------------------------------
/// @copydoc WriteReg4
static int8_t WriteReg4(uint8_t DevAddr, uint16_t RegAddr, uint32_t RegVal){

	// 7 bit address
	DevAddr = (DevAddr >> 1);
			
	volatile uint32_t RegVal1 = RegVal & 0x000000FF;
	volatile uint32_t RegVal2 = (RegVal >> 8UL) & 0x000000FFUL;
	volatile uint32_t RegVal3 = (RegVal >> 16UL) & 0x000000FFUL;
	volatile uint32_t RegVal4 = (RegVal >> 24UL) & 0x000000FFUL;
	
	Wire.beginTransmission(DevAddr);	
	
	Wire.write((uint8_t)RegAddr);
	
	Wire.write((uint8_t)(RegVal1));
	Wire.write((uint8_t)(RegVal2));
	Wire.write((uint8_t)(RegVal3));
	Wire.write((uint8_t)(RegVal4));
	
	uint8_t result = Wire.endTransmission();
	
	if(result == 0){	// Ack
		return RETURN_NORMAL_VALUE;
	}
	else{				// Nack
		return RETURN_FAILURE_VALUE;
	}	
}

//-----------------------------------------------------------------------------
int8_t i2c1_write_byte(uint8_t addr, uint16_t Offset, uint8_t Data){
	
	#ifdef DEBUG_LEVEL_3
		TRACE3("i2c1_write_byte(uint8_t addr=%s, uint8_t Offset=%s, uint8_t Data=%s)\n", addr, Offset, Data);
	#endif

	if(RETURN_NORMAL_VALUE == WriteReg1(addr, Offset, Data)) {
		return RETURN_NORMAL_VALUE;
	}
	
	#ifdef	DEBUG_LEVEL_2
	TRACE3("\tI2C1 write byte ERROR!! %bx %x %bx\n", addr, Offset, Data);
	#endif
	
	return RETURN_FAILURE_VALUE;
}

//-----------------------------------------------------------------------------
int8_t i2c1_read_byte(uint8_t addr, uint16_t Offset, uint8_t *pData){
	#ifdef DEBUG_LEVEL_4
		TRACE3("i2c1_read_byte(uint8_t addr=%02X, uint8_t Offset=%03X, uint8_t *pData=%02X)\n", addr, Offset, pData);
	#endif
	
	int8_t return_value;
	
	return_value = ReadReg1(addr, Offset, pData);
	
	if(RETURN_NORMAL_VALUE==return_value) {
		return RETURN_NORMAL_VALUE;
	}
	
	#ifdef	DEBUG_LEVEL_2
	TRACE3("\tI2C1 read byte ERROR!! %02X %03X, return=%d\n", addr, Offset, return_value);
	#endif
	
	return RETURN_FAILURE_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc ReadReg1
static int8_t ReadReg1(uint8_t DevAddr, uint16_t RegAddr, uint8_t *pData){
    
	i2c_flush();
	
	Wire.beginTransmission(DevAddr);
	Wire.write(RegAddr);
	Wire.endTransmission(false);
	
	Wire.requestFrom(DevAddr, 1);
	
	uint8_t ack = false;
	
	while(Wire.available()){
		*pData = Wire.read();
		ack = true;
	}
	
	if(ack == true){
		return RETURN_NORMAL_VALUE;
	}else{
		*pData = -1;
		return RETURN_FAILURE_VALUE;
	}
}

//-----------------------------------------------------------------------------
/// @copydoc WriteReg1
static int8_t WriteReg1(uint8_t DevAddr, uint16_t RegAddr, uint8_t RegVal){
	
	Wire.beginTransmission(DevAddr);
	Wire.write(RegAddr);
	Wire.write(RegVal);
	
	if(Wire.endTransmission() == 0){	// Ack
		return RETURN_NORMAL_VALUE;
	}else{								// Nack
		return RETURN_FAILURE_VALUE;
	}
}



