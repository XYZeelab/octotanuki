/*
 * main.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: Anthony
 */


#include "arduino.h"
#include "Wire.h"

const int SLAVE_ADDRESS(0x40);

enum DataFrameFlags {
	START_FLAG=0x12,
	END_FLAG=0x13,
	ESC_FLAG=0x7D
};

void receiveEvent (int size);

void setup(void)
{
    pinMode(13, OUTPUT);
    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
}

void loop(void)
{
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(2500);
}

void receiveEvent (int size)
{
	uint8_t rBuffer[BUFFER_LENGTH];
	int idx = 0;


	if (Wire.available() > 1)
	{
		uint8_t ch = Wire.read();

		// Determine if the first byte is the start flag.
		if (ch == START_FLAG) {
			while (Wire.available() > 1)
			{
				rBuffer[idx] = Wire.read();
				idx++;
			}
		} else {
			// Drop frame.
		}
	}
}
