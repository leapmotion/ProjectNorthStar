/**
* @file start.h
*
* @brief Main program entry point _H
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
* @author Adam Munich
*/

/**
* @defgroup Program [Main] Main program
* @details
*	These are the files which contain the main program, and its functions
*	which control all the peripherals and libraries in a fashion congruent with 
*	proper operation of the board.
*/

#ifndef START_H_
	#define START_H_

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
	 *		The setup routine runs once when you press reset
	 * @ingroup Program
	 * @return void
	 */	
	void setup();

	/**
	 * @brief 
	 *		The loop routine runs over and over again forever
	 * @ingroup Program
	 * @return void
	 */
	void loop();
	
	void find_i2c_devices();
	void ME_cal(uint8_t P0, uint8_t P1, uint8_t P2, uint8_t P4);

#endif /* START_H_ */