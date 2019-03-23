#include <HardwareSerial.h>

const float DryValue = 3100;   //maximum value, completely dry
const float WetValue = 1000;  //minimum value, completely wet
float SoilMoistureValue = 0.0;
float MoisturePercent = 0.0;

void setup() 
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}

void loop() 
{
SoilMoistureValue = analogRead(A0);  //replace soilmoisture with reading from analog pin
MoisturePercent = SoilMoistureValue/DryValue; 
Serial.println(MoisturePercent); //print the value
delay(100);
}
