/*
 * main.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: Anthony
 */


#include "arduino.h"
#include "Wire.h"

void receiveEvent (int size);

void setup(void)
{
    pinMode(13, OUTPUT);
    Wire.begin(0x04);
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

	while (Wire.available() > 1)
	{
		rBuffer[idx] = Wire.read();
		idx++;
	}
}
