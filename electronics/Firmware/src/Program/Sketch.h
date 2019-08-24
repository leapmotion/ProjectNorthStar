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

#ifndef SKETCH_H_
#define SKETCH_H_

//#############################################################################
// Pre-compiler Definitions
//-----------------------------------------------------------------------------

#define LEAP_DISPLAY_BOARD_REV          0x0000

const char * buildDate = "Build date: " __DATE__ ;
const char * buildTime = "Build time: " __TIME__ ;

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

#endif /* SKETCH_H_ */
