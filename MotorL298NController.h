/*
 *  MotorController.h
 *
 *  Copyright (C) 2018 by Mar'yan Rachynskyy
 *  rmaryan@gmail.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MOTORL298NCONTROLLER_H_
#define MOTORL298NCONTROLLER_H_

#include <Arduino.h>

/*
 * Motors controlling class
 */
class MotorL298NController {
private:
	uint8_t ENA, IN1, IN2, ENB, IN3, IN4;
	int16_t currentLeftSpeed = 0;
	int16_t currentRightSpeed = 0;
public:
	// Constructs the controlling object
	// Arduino pin numbers must be provided as the input parameters
	MotorL298NController(uint8_t in_ENA, uint8_t in_IN1, uint8_t in_IN2,
			uint8_t in_ENB, uint8_t in_IN3, uint8_t in_IN4);
	// Run both motors at the speed specified. Speed can be between -255 (backward) and 255 (forward)
	void go(int16_t speed);
	// Run each motor at different speed
	void go(int16_t leftSpeed, int16_t rightSpeed);
};

#endif /* MOTORL298NCONTROLLER_H_ */
