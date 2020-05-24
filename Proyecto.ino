#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#define IN3 5
#define IN4 4 
#define echoPin 8 
#define trigPin 9 

float duracion, distancia; 
int estado;

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); 

void configureSensor(void)
{
  tsl.setGain(TSL2591_GAIN_MED);      
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  
}

void setup(void) 
{
  Serial.begin(9600);

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

void loop(void) 
{ 
 uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);        

       lectura_distancia();
           
 switch (estado) 
 {
  case 0:  if (x >350 && distancia>30) //Encendido del motor
            {
              // Motor gira para subir la persiana
              digitalWrite (IN4, LOW);
              digitalWrite (IN3, HIGH);
              estado=1;
            }
           else
             {
              //Seguir imprimiendo la luminosidad
              Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
              Serial.print(F("Luminosity: "));
              Serial.println(x, DEC);

              Serial.print("Distancia: ");
              Serial.println(distancia);
        
             delay(10); 
             } 
   break;
             
   case 1:  if (distancia<30 && x >350)
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

             Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
             Serial.print(F("Luminosity: "));
             Serial.println(x, DEC);
              
             Serial.print("Distancia: ");
             Serial.println(distancia);
            }
   break;

    case 2:  if (distancia<30 && x <350)
            { 
              digitalWrite (IN4,HIGH); 
              digitalWrite (IN3, LOW);
              delay(2000);
              digitalWrite (IN4,LOW); 
              digitalWrite (IN3, LOW);
              estado=0;
              
            }
           else
            {  
             digitalWrite (IN4, LOW);
             digitalWrite (IN3, LOW);
             estado=2;
             
             Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
             Serial.print(F("Luminosity: "));
             Serial.println(x, DEC);
              
             Serial.print("Distancia: ");
             Serial.println(distancia);
            }
   break;
 }  
}

 void lectura_distancia(void)
 {
  digitalWrite(trigPin, HIGH);
       delay(10); 
       digitalWrite(trigPin, LOW);
       duracion = pulseIn(echoPin, HIGH); //Tiempo en microsegundos en recibir el pulso
       //se calcula la distancia en cm dependiendo de la velocidad del sonido
       distancia = (duracion/2)/29.1; //velocidad del sonido: 343 m/s
 }
