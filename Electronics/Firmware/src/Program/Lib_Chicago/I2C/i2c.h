/**
* @file i2c.h
*
* @brief Chicago I2C communication functions _H
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

/**
* @defgroup Chicago_i2c [Functions] Chicago I2C communication
* @details
*	These functions handle I2C communication with the Chicago bridge, including
*	register writes, reads, and initialization routines. For most intents and
*	purposes, i2c_begin(), i2c_write_byte(), i2c_write_byte4(), i2c_read_byte(),
*	and i2c_read_byte4() are the only functions which need to be directly called.
*   Further information can be found in ANX753X_Programming_Guide.pdf, page 13.
*/


#ifndef __I2C_H__
	#define __I2C_H__

	//#############################################################################
	// Pre-compiler Definitions
	//-----------------------------------------------------------------------------


	//#############################################################################
	// Type Definitions
	//-----------------------------------------------------------------------------


	//#############################################################################
	// Function Prototypes
	//-----------------------------------------------------------------------------
	/**
	 * @brief 
	 *		Flush bytes from I2C buffer
	 * @ingroup Chicago_i2c
	 * @return void
	 */	
	void i2c_flush();

	/**
	 * @brief 
	 *		Write 1 byte to Chicago wire (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param SlaveID - Chicago Slave ID
	 * @param Offset - Register Address Offset (12 Bit)
	 * @param Data - Register data
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	int8_t i2c_write_byte(uint8_t SlaveID, uint16_t Offset, uint8_t Data);
	
	/**
	 * @brief 
	 *		Write 1 byte to Chicago wire, and keep bus alive (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param Offset - Register Address Offset (12 Bit)
	 * @param Data - Register data
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */		
	int8_t i2c_write_byte_keep(uint16_t Offset, uint8_t Data);
	
	/**
	 * @brief 
	 *		Write 4 bytes to Chicago wire (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param SlaveID - Chicago Slave ID
	 * @param Offset - Register Address Offset (12 Bit)
	 * @param Data - Register data
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	int8_t i2c_write_byte4(uint8_t SlaveID, uint16_t Offset, uint32_t Data);

	/**
	 * @brief 
	 *		Read 1 byte from Chicago wire (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param SlaveID - Chicago Slave ID
	 * @param Offset - Register Address Offset (12 Bit)
	 * @param pData - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	int8_t i2c_read_byte(uint8_t SlaveID, uint16_t Offset, uint8_t *pData);
	
	/**
	 * @brief 
	 *		Read block of registers from Chicago wire (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param SlaveID - Chicago Slave ID
	 * @param Offset - Register Address Offset (12 Bit)
	 * @param pData - Register data returned (uint8_t array)
	 * @param Length - Size of int8_t array
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	int8_t i2c_read_block(uint8_t SlaveID, uint16_t Offset, uint8_t *pData, uint32_t Length);

	/**
	 * @brief 
	 *		Read byte from register (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address
	 * @param pData - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */		
	static int8_t ReadReg(uint8_t DevAddr, uint16_t RegAddr, uint8_t *pData);
	
	/**
	 * @brief 
	 *		Read 4 bytes from register (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address
	 * @param pData - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */			
	static int8_t ReadReg4(uint8_t DevAddr, uint16_t RegAddr, uint32_t *pData);

	/**
	 * @brief 
	 *		Read block of registers (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address
	 * @param n - Number of bytes to read
	 * @param pBuf - Register data char array
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	static int8_t ReadBlockReg(uint8_t DevAddr, uint16_t RegAddr, uint8_t n, uint8_t *pBuf);

	/**
	 * @brief 
	 *		Read byte from register (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address
	 * @param RegVal - Register value
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */			
	static int8_t WriteReg(uint8_t DevAddr, uint16_t RegAddr, uint8_t RegVal);
	
	/**
	 * @brief 
	 *		Write 4 bytes to register (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address
	 * @param RegVal - Register value
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */		
	static int8_t WriteReg4(uint8_t DevAddr, uint16_t RegAddr, uint32_t RegVal);

	/**
	 * @brief 
	 *		Write byte to accessory wire (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param addr - Device address
	 * @param Offset - Register Address Offset (12 Bit)
	 * @param Data - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	int8_t i2c1_write_byte(uint8_t addr, uint16_t Offset, uint8_t Data);

	/**
	 * @brief 
	 *		Read byte from accessory wire (Chicago abstraction)
	 * @ingroup Chicago_i2c
	 * @param addr - Device address
	 * @param Offset - Register address offset (12 Bit)
	 * @param pData - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */		
	int8_t i2c1_read_byte(uint8_t addr, uint16_t Offset, uint8_t *pData);
	
	/**
	 * @brief 
	 *		Read byte from accessory wire (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address offset (12 Bit)
	 * @param pData - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	static int8_t ReadReg1(uint8_t DevAddr, uint16_t RegAddr, uint8_t *pData);
	
	/**
	 * @brief 
	 *		Write byte to accessory register (directly)
	 * @ingroup Chicago_i2c
	 * @param DevAddr - Device address
	 * @param RegAddr - Register address offset (12 Bit)
	 * @param RegVal - Register data returned
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */	
	static int8_t WriteReg1(uint8_t DevAddr, uint16_t RegAddr, uint8_t RegVal);
	
#endif /* __I2C_H__ */

