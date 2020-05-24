#include <TimeLib.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#define IN3 5
#define IN4 4 
#define echoPin 8 
#define trigPin 9 

float duracion, distancia; 
int estado;
int h;
int m;

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); 

void configureSensor(void)
{
  tsl.setGain(TSL2591_GAIN_MED);      
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  
}

void setup()  
{
   Serial.begin(9600);
   
  //TIEMPO
  pinMode(13, OUTPUT);
 setSyncProvider( requestSync); //set function to call when sync required
  Serial.println("Waiting for sync message");

  //SENSOR LUMÍNICO
  if (tsl.begin()) 
    {
      Serial.println(F("Found a TSL2591 sensor"));
    } 
  else 
    {
      Serial.println(F("No sensor found ... check your wiring?"));
      while (1);
    }

  //MINI MOTOR REDUCTOR  
  pinMode (IN4, OUTPUT); 
  pinMode (IN3, OUTPUT);
  estado=0;

  //ULTRASONIDO
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{    
 time_t t=now();
    
 switch (estado) 
 {
  case 0:
  {
    if (Serial.available()>0) 
  {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) 
  {
    digitalClockDisplay();  
    SetAlarm();
    
    if(h==hour(t) && m==minute(t) && distancia>30)
    {
       Serial.print("He recibido el tiempo");
       // Motor gira para subir la persiana
       digitalWrite (IN4, LOW);
       digitalWrite (IN3, HIGH);
       estado=1;
    }
    else
    {
    estado=0;
    
    Serial.print("Distancia: ");
    Serial.println(distancia);
    delay(100); 
    } 
  
    }
  }
  break;
  
  case 1: 
  {
    lectura_distancia();
    if(distancia<30)
    {
     digitalWrite (IN4,LOW); 
     digitalWrite (IN3, LOW);
     estado=2;    
    }
    else
    {
       digitalWrite (IN4, LOW);
       digitalWrite (IN3, HIGH);
       estado=1;

       Serial.print("Distancia: ");
       Serial.println(distancia);
    }
   }
  break;

   case 2:
   {
     uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
     if(x<350)
     {
      digitalWrite (IN4, HIGH);
      digitalWrite (IN3, LOW);
      estado=3;
      delay(2000);
      digitalWrite (IN4,LOW);
      digitalWrite (IN3,LOW);
      estado=0;
     }
     else
     {
      estado=2;
      Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
      Serial.print(F("Luminosity: "));
      Serial.println(x, DEC);
      
      Serial.print("Distancia: ");
      Serial.println(distancia);
     }
   }
   break;   
  }
}

void SetAlarm()
{
  int h;
  int m;

  h=20;
  m=34;
}


void processSyncMessage() 
{
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) 
  {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) 
     { 
      // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime); // Sync Arduino clock to the time received on the serial por
  lectura_distancia();
     }
}
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void lectura_distancia(void)
 {
  digitalWrite(trigPin, HIGH);
       delay(100); 
       digitalWrite(trigPin, LOW);
       duracion = pulseIn(echoPin, HIGH); //Tiempo en microsegundos en recibir el pulso
       //se calcula la distancia en cm dependiendo de la velocidad del sonido
       distancia = (duracion/2)/29.1; //velocidad del sonido: 343 m/s
 }

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
