/**
 *  Radio Nano Bot entry point
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
 */

#include "MotorL298NController.h"
#include "LiquidCrystal_I2C.h"
#include "U8g2lib.h"

// INPUT pins for the radio-control throttle and turning channels
const uint8_t RC_CH_THROTTLE = 11;
const uint8_t RC_CH_TURN = 12;

// pins to operate motors driver
const uint8_t ENA = 10;
const uint8_t IN1 = 9;
const uint8_t IN2 = 8;
const uint8_t IN3 = 7;
const uint8_t IN4 = 6;
const uint8_t ENB = 5;

// refresh the screen every n-th cycle
const uint16_t SCREEN_REFRESH_DURATION = 20;
uint16_t screenRefreshCounter = SCREEN_REFRESH_DURATION;

//MOTOR CONTROLLER
MotorL298NController motorController(ENA, IN1, IN2, ENB, IN3, IN4);

// Back screen - a 16 by 2 TFT text LCD
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C backLCD = LiquidCrystal_I2C(0x27, 16, 2);

// A front screen - a 128x64 monochrome LCD
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);

// initialize the front screen
void initFrontLCD() {
	u8g2.begin();
	u8g2.firstPage();
	// this code draws a funny smile
	do {
		u8g2.drawLine(41, 8, 45, 32);
		u8g2.drawLine(83, 8, 87, 32);
		u8g2.drawEllipse(65, 43, 29, 8,
				U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
		u8g2.drawLine(73, 51, 78, 55);
		u8g2.drawLine(61, 51, 66, 55);
		u8g2.drawLine(67, 51, 72, 57);
		u8g2.drawEllipse(72, 55, 6, 8,
				U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
	} while (u8g2.nextPage());
}

// this function is called at every loop cycle
// once in a while it outputs to the back LCD the received
// throttle and turning values
void refreshBackLCD(int16_t speedL, int16_t turnRate) {
	// refresh the back screen every n-th cycle
	if (screenRefreshCounter == 0) {
		backLCD.clear();
		backLCD.print("SPD: ");
		backLCD.print(speedL);
		backLCD.setCursor(0, 1);
		backLCD.print("TURN: ");
		backLCD.print(turnRate);
		screenRefreshCounter = SCREEN_REFRESH_DURATION;
	} else {
		screenRefreshCounter--;
	}
}

// a standard Arduino setting-up function
// is called once, right after the board start up
void setup() {
	// initialize the back LCD
	backLCD.init();
	// turn off the auto-scroll, we will control the text output position on our own
	backLCD.noAutoscroll();
	// turn on the backlight
	backLCD.backlight();
	// greet the world
	backLCD.print("Initializing...");

	// these pins are used to read the RC receiver channels
	pinMode(RC_CH_TURN, INPUT);
	pinMode(RC_CH_THROTTLE, INPUT);

	// initialize the front screen
	initFrontLCD();

	// the robot is ready for action
	backLCD.clear();
	backLCD.print("HELLO MASTER!");
}

// the main loop - called continuously
void loop() {
	// read the RC sticks positions
	long chThrottle = pulseIn(RC_CH_THROTTLE, HIGH, 30000); // need to specify 30000 here for Nano
	long chTurn = pulseIn(RC_CH_TURN, HIGH, 30000);

	// values below 900 mean the receiver has no signal from the transmitter
	if (chThrottle < 900) {
		backLCD.setCursor(0, 1);
		backLCD.print("No signal...");
		motorController.go(0);
	} else {
		int16_t speedL = 0;
		// cut off the low throttle values (below 1000), so the motors will be turned off
		// otherwise you will be hearing an annoying squeak at the near-zero throttle positions
		if (chThrottle > 1000) {
			// translate the radio control input (range 1000 to 2000)
			// to the motors operations range (30 to 255)
			speedL = map(chThrottle, 1000, 2000, 30, 255);
		}
		// set the motors initially to the same speed
		int16_t speedR = speedL;

		// shifting the turning stick reduces the corresponding motor speed by max 160 points (of 255)
		int16_t turnRate = map(chTurn, 1000, 2000, -160, 160);

		// refresh the back screen every n-th cycle
		refreshBackLCD(speedL, turnRate);

		// send the speed commands to the motors
		// taking the turning value to the account
		if (turnRate > 0) {
			motorController.go(speedL, speedR - turnRate);
		} else {
			motorController.go(speedL + turnRate, speedR);
		}
	}
}
