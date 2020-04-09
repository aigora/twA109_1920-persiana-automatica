#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
const int base=12;

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); 

void configureSensor(void)
{
  tsl.setGain(TSL2591_GAIN_MED);      
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  
}

void setup(void) 
{
  Serial.begin(9600);
  
  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  
  if (tsl.begin()) 
  {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
  pinMode(base,OUTPUT);
}

void simpleRead(void)
{
  uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);

   if (x > 790)
{
  digitalWrite(base ,HIGH);
delay(10000); 
  digitalWrite(base,LOW);
delay(100000000); 
}
else
{
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("Luminosity: "));
  Serial.println(x, DEC);
}

}

void loop(void) 
{ 
  simpleRead(); 
  delay(5000);  

}
