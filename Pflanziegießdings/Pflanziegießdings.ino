/*
 Name:		Pflanziegießdings.ino
 Created:	15.05.2018 18:26:40
 Author:	manni
*/

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial


#include "Credentials.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


const uint8_t HumidityAlert = 200;
const uint32_t HumidityMax = 215;
const uint32_t HumidityMin = 1010;
const uint32_t humidityReadInverval = 3e+8;

BlynkTimer timer;

void setup()
{
	// Debug console
	Serial.begin(9600);
	Blynk.begin(auth, ssid, pass);
	pinMode(D1, OUTPUT);
	timer.setInterval(humidityReadInverval, readHumiditySensor);
	readHumiditySensor();
}

void loop()
{
	Blynk.run();
	timer.run();
}


void readHumiditySensor() {
	digitalWrite(D1, HIGH);
	delay(10);
	uint8_t val = map(analogRead(A0), HumidityMin, HumidityMax, 0UL, 100UL);
	BLYNK_LOG2("Pushing data...", val);
	Blynk.virtualWrite(V1, val);
	digitalWrite(D1, LOW);
	ESP.deepSleep(humidityReadInverval);
}