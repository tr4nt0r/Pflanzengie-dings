/*
 Name:		Pflanziegießdings.ino
 Created:	15.05.2018 18:26:40
 Author:	manni
*/

//Blynk Auth Token
//char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "YourNetworkName";
//char pass[] = "YourPassword";

//Meien WiFi Zugangsdaten, Datei ist in .gitignore 
#include "Credentials.h"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Sensorkalibrierung
const uint8_t HumidityAlert = 0; //Wert für Blynk-Pushnotification (todo) 
const uint32_t HumidityMax = 215; //Sensorwert in Wasser
const uint32_t HumidityMin = 1010; //Sensorwert in Luft
const uint32_t humidityReadInverval = 3e+8; //Sensorwerterfassung alle 5min, Wert in µs

BlynkTimer timer;

void setup()
{
	// Debug console
	Serial.begin(9600);
	Blynk.begin(auth, ssid, pass);
	pinMode(D1, OUTPUT);
	//Wenn SleepMode deaktiviert dann per Timerinterval
	//Sensorwert lesen (RST und D0 müssen gebrückt sein für Wakeup) 
	timer.setInterval(humidityReadInverval, readHumiditySensor);
	readHumiditySensor(); //Sensorwert Lesen nach wakeup
}

void loop()
{
	Blynk.run();
	timer.run();
}


void readHumiditySensor() {	
	digitalWrite(D1, HIGH);	delay(10); //Sensor aktivieren
	uint8_t val = map(analogRead(A0), HumidityMin, HumidityMax, 0UL, 100UL); //Sensorwert lesen und in Wert 0% - 100% umwandeln
	BLYNK_LOG2("Pushing data...", val); //Wert an Blynk-Server pushen
	Blynk.virtualWrite(V1, val);
	digitalWrite(D1, LOW);	//Sensor wieder deaktivieren
	ESP.deepSleep(humidityReadInverval); //Mikrocontroller für festgelegte Zeit in Tiefschlaf versetzen
}