/*
 *  MotorController.cpp
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
 */

#include "MotorL298NController.h"

MotorL298NController::MotorL298NController(uint8_t in_ENA, uint8_t in_IN1, uint8_t in_IN2,
		uint8_t in_ENB, uint8_t in_IN3, uint8_t in_IN4) {
	ENA = in_ENA;
	IN1 = in_IN1;
	IN2 = in_IN2;
	ENB = in_ENB;
	IN3 = in_IN3;
	IN4 = in_IN4;
}

void MotorL298NController::go(int16_t speed) {
	go(speed, speed);
}

void MotorL298NController::go(int16_t leftSpeed, int16_t rightSpeed) {
	bool goLeftForward = true;
	bool goRightForward = true;

	if (leftSpeed != currentLeftSpeed) {
		currentLeftSpeed = leftSpeed;
		if (leftSpeed < 0) {
			goLeftForward = false;
			leftSpeed = -leftSpeed;
		}

		digitalWrite(IN1, goLeftForward ? LOW : HIGH);
		digitalWrite(IN2, goLeftForward ? HIGH : LOW);
		analogWrite(ENA, leftSpeed);
	}

	if (rightSpeed != currentRightSpeed) {
		currentRightSpeed = rightSpeed;
		if (rightSpeed < 0) {
			goRightForward = false;
			rightSpeed = -rightSpeed;
		}

		digitalWrite(IN3, goRightForward ? LOW : HIGH);
		digitalWrite(IN4, goRightForward ? HIGH : LOW);
		analogWrite(ENB, rightSpeed);
	}
}
