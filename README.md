# Persiana automática

Desarrollar un motor que permita subir la persiana de una habitación mediante un sistema de reducido tamaño de poleas y carretes, que se activa con un sensor lumínico. Cuando el sensor haya detectado un determinado nivel de iluminación (correspondiente al del amanecer), este manadará una señal al ordenador para que active un motor que hará que suba la persiana medienate un carrete, todo a través de una placa Arduino.

## Integrantes del equipo

Nombre y apellidos: Inés Portilla de Birazel
Usuario GitHub: InesPortilla

## Objetivos del trabajo
-Consultar tutoriales y guías de Arduino para rofundizar mi conocimiento en programación con Arduino. En especial en cuanto a sensores lumínicos (entrada) y mini motores (salida).
-Programar el sensor para que envíe datos al PC justo después de leer un determinado nivel de iluminación.
-Transmitir estos datos al PC y hacer que este active un motor.
-Desarrollar una polea que funcione a través de un motor que se activa con los datos que reciba del odenador, que permita llevar a cabo el movimiento vertical de bajar la persiana.

## Hardware
PC, Arduino UNO Rev. 3, sensor de luz para Arduino, mini motor eléctrico.

## Software 
Programación del microcontrolador a través del IDE de Arduino

# Funciones empleadas 
 
## Creadas 
 
### Lectura distancia con ultrasonido
* Nombre: lectura_distancia 
* Argumentos: void. No se le pide al usuario que introduzca un valor. 
* Procesamiento:  
1. Generar el disparo de ondas  
2. Medir el tiempo entre pulsos (en microsegundos) 
3. Convertir el tiempo a distancia total recorrida y dividirla entre dos (el ultrasonido lee la ida y la vuelta de las ondas, pero solo interesa cuánto recorrieron las ondas en un sentido). 

Distancia (cm) = (Tiempo (μs))/(29,2*2)

* Salida: Distancia desde el sensor hasta la superficie más cercana contra la que choquen las ondas verticalmente, aunque tampoco es necesario que devuelva un valor (de ahí que sea una función de tipo void). 
```
void lectura_distancia(void) 
{ 
  digitalWrite(trigPin, HIGH); 
       delay(1000);  
       digitalWrite(trigPin, LOW); 
//Tiempo en microsegundos en recibir el pulso 
       duracion = pulseIn(echoPin, HIGH);         
//se calcula la distancia en cm dependiendo de la velocidad del sonido 
       distancia = (duracion/2)/29.1; //velocidad del sonido: 343 m/s 
} 
```
## De biblioteca 
  Las siguientes funciones provienen de bibliotecas de uso público que se pueden conseguir de manera gratuita en un repositorio Git creado por Adafruit (la compañía que fabrica el sensor lumínico usado y demás productos electrónicos). 

### Constructor 
* Nombre biblioteca: Adafruit_TSL2591 
* Nombre: Adafruit_TSL2591(2591) 
* Argumentos: 2591 (el nombre del modelo del sensor lumínico). 
* Procesamiento: Creación de una instancia de la biblioteca. 
```
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591) 
```
* Salida: Uso del sensor lumínico TSL2591(2591) de Adafruit. 
 
### Configuración del sensor 
* Nombre: configureSensor 
* Nombre biblioteca: <Adafruit_Sensor.h> 
* Argumentos y salida: void (no es necesario insertar ni devolver valores) 
* Procesamiento: Se configura la ganancia (cuán sensible será el sensor a la luz que reciba) y el tiempo de integración (durante cuánto tiempo tomarán las muestras de luz). Para el proyecto se escogió una ganancia media (usada para propósitos más generales) y un tiempo ed integración de 300 milisegundos (la mediana dentro del rango de los valores de tiempo del sensor). 
 
 ```
void configureSensor(void) 
{ 
  tsl.setGain(TSL2591_GAIN_MED);       
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); 
} 
 ```
 
### Lectura intensidad lumínica 
* Nombre biblioteca: <Adafruit_Sensor.h> 
* Nombre: getLuminosity 
* Argumentos: Diodos para detectar según qué tipo de radiación electromagnética (ultravioleta, luz visible, infrarrojo). En este caso se leerán valores dentro del espectro de luz visible. 
* Procesamiento:  
1. El diodo lee la intensidad lumínica de luz visible  percibida en el ambiente 
2. Arroja el valor leído en Lux 
```uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE); ```  
* Salida: Valor de la luminosidad leída en Lux (definida como x) de 16 bits. 
 

# Sensores y actuadores

## Título y resumen
_Persiana automática con Arduino_
Sistema electrónico conformado por un pequeño motor DC que provoca el giro de un carrete cuya cuerda está enganchada a la polea de la persiana, para poder subirla automáticamente. Para tal efecto, el motor se activa con un sensor lumínico cuando este detecta un nivel de iluminación correspondiente al del amanecer. Esta comunicación entre el sensor (entrada) y el motor (salida) se efectúa a través de un microcontrolador de Arduino. Después, el motor para de girar cuando, al estar subiéndose la persiana, un sensor de ultrasonido (ubicado unos centímetros debajo del rollo de la persiana) detecta el borde inferior de esta.

## Requisitos funcionales 
1. El proceso se inicia cuando el sensor detecta una intensidad lumínica equivalente a un amanecer (600 lux aproximadamente). 
2. El sensor estará enviando los datos al microcontrolador y cuando se alcanza la intensidad deseada, este acciona el motor. 
3. El motor sigue girando hasta que se alcanza un límite de tiempo definido en el código (el tiempo que tarda el motor en subir la persiana). 

## Hardware – Fundamentos técnicos 

### Detección de luz solar - Sensor de luz TSL2592 High Adafruit para Arduino
  Un sensor de luz permite leer y hacer cálculos con la luz que detecta. Tiene varias aplicaciones, entre ellas en alumbrados públicos automáticos, sistemas de seguridad, como regulador de brillo en las pantallas de aparatos electrónicos (ordenadores y móviles, por ejemplo), e incluso como medidores de luz en las cámaras. Las ventajas de este tipo de sensores son su disponibilidad de formas y tamaños, su bajo coste, son sencillos de programar e incluso necesitan poco voltaje para funcionar (funcionan con 5V de potencia que puede proporcionar el microcontrolador de Arduino Uno). No obstante, los diodos de los sensores lumínicos son sensibles a las temperaturas y son unidireccionales. 
  En lo que concierne a este sensor en particular, su rango de medición de intensidad lumínica que abarca de 188 uLux a 88,000 Lux y soporta una temperatura de –30 a 80°C. Además, al incluir diodos infrarrojos y de espectro completo, es capaz de hacer mediciones en varios espectros de luz por separado, incluyendo el de luz visible. Funciona con un voltaje entre 3,3 y 5V. En comparación con otras células fotovoltaicas de bajo coste (como las de cadmio) esta realiza mediciones más precisas, permitiendo así ser configurado para distintos rangos de tiempo y ganancia. Tiene varios pines de funcionamiento, unos de potencia (para encenderlo y para regular su voltaje) y otros de lógica I2C (pines clock y data). 
  Se conecta al microcontrolador de la siguiente manera: 
 
  ![Alt text](https://cdn-learn.adafruit.com/assets/assets/000/017/910/original/sensors_arduinowire.jpg?1405023389)
 
```Y para configurarlo hace falta ingresar el siguiente código: 
// Configuración de la ganancia y el tiempo de integración para el TSL2561  
 void configureSensor(void) 
{ 
// El tiempo de integración determina el tiempo durante el cual se detecta la luz. Los tiempos largos de integración son útiles en situaciones donde se trabaja con poca luz 
tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);           // Máximo tiempo de integración 
//Mostrar la ganancia y el tiempo de integración:  
Serial.println("------------------------------------");  
Serial.print ("Gain: "); 
tsl2591Gain_t gain = tsl.getGain(); 
 switch(gain)  { 
case TSL2591_GAIN_LOW: Serial.println("1x (Low)"); break; 
case TSL2591_GAIN_MED: Serial.println("25x (Medium)"); break; 
case TSL2591_GAIN_HIGH: Serial.println("428x (High)"); break; 
case TSL2591_GAIN_MAX: Serial.println("9876x (Max)"); break; 
} 
Serial.print ("Timing: ");  
Serial.print((tsl.getTiming() + 1) * 100, DEC);  
Serial.println(" ms");  
Serial.println("------------------------------------");  
Serial.println(""); 
} 
En este proyecto, la tarea del sensor será medir la luminosidad que llega a la ventana de la habitación durante el amanecer. Para medirla, se emplea el siguiente código: 
Salto de página
 
 void simpleRead(void) 
{ 
uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE); 
Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] "); Serial.print("Luminosity: "); 
Serial.println(x, DEC); 
} 
```
 
### Funcionamiento del carrete – Mini motor DC
  Un motor de corriente continua es un aparato que puede convertir energía eléctrica en mecánica, realizando un movimiento rotatorio. Consta de dos partes: una parte fija compuesta por un electroimán, que es puesto en marcha por dos campos magnéticos de polaridades opuestas situados en el extremo al cual se conecta la corriente. A su vez, estos dos campos inducen la fuerza necesaria sobre la parte móvil para hacerle girar, aplicando un torque. Esta otra parte está compuesta por varios espirales o bobinas, que al girar hacen rotar una protuberancia metálica. Gracias a su versatilidad (por su fácil control, paro y automatización) resulta útil en contextos industriales y domésticos. Por ejemplo, se encuentran motores DC en maquinillas eléctricas, en los limpiaparabrisas e incluso en motores de coches. 
  El motor que se usará en concreto para este proyecto es un mini motor reductor 250:1 de alta potencia. Tiene un voltaje nominal de 6 voltios (aunque funciona bien con 9V proporcionados por una batería a través de un Puente H), su velocidad de salida es de 50 rpm y tienen un consumo sin carga de 30mA. Las cualidades de este motor son su resistencia a las temperaturas y a la abrasión y su gran capacidad de carga. En particular, la protuberancia (mediante la cual se hará girar el carrete) está hecha con acero de alta calidad, alta dureza y resistencia. 
  Para poder manipular este motor mediante Arduino y una batería de 9V es necesario el uso (aparte de cables, por supuesto) de un Puente H. Este es un dispositivo que permite manipular el sentido de giro de un motor de corriente continua (el cual depende de la polaridad de la fuente de alimentación), sin necesidad de invertir la batería. También es considerado como un driver, pues aporta más corriente de la que puede aportar el microcontrolador de Arduino. 
  Así pues, es necesario conectar la batería a los inputs +12 y GND del Puente H, a la vez que este GND se conecta al del Arduino Uno, y luego conectar las terminales del motor a cualquiera de los dos pares de outputs. Ahora, el número de los pines de salida del puente han de conectarse a los pines digitales del Arduino del mismo número (siendo uno de ellos un PWM). Para combinar todos estos aparatos se monta el circuito como se muestra en el diagrama:  
 
 ![Alt Text](file:///Users/Ines/Desktop/Captura%20de%20Pantalla%202020-04-22%20a%20la(s)%2011.33.14%20p.%C2%A0m.png?raw=true)
 
Y se activa con este código:  
```
#define IN3 5 
#define IN4 4  
  
void setup()  
{ 
  pinMode (IN4, OUTPUT); 
  pinMode (IN3, OUTPUT); 
} 
void loop() { 
  // Motor gira en un sentido 
  digitalWrite (IN4, HIGH); 
  digitalWrite (IN3, LOW); 
  delay(3000); 
} 
```
 
### Sensor Ultrasonido
 
 
## Referencias 
  Adafruit Industries. (2018). Adafruit TSL2591 High Dynamic Range Digital Light Sensor[Ebook (1st ed., pp. 3-18). Retrieved from 
https://www.cetronic.es/sqlcommerce/disenos/plantilla1/seccion/producto/DetalleProducto.jsp?idIdioma=&idTienda=93&codProducto=999334022&cPath=1343&gclid=Cj0KCQiA-bjyBRCcARIsAFboWg2jEmebQvp0diffuzqg05AEinYSC8SN61KRFypkkhpYK0qUlXu4z-MaAphUEALw_wcB 

  CREA Club de robótica ETSIDI. (2019). Transistores Puente H. Presentation, Madrid. 
  
  El mejor Reductor para motor eléctrico de 2020 | Las20Mejores.com. (2020). Retrieved 21 March 2020, from 
https://las20mejores.com/mejor-reductor-para-motor-electrico/      
  
  Fritz, C. (2017). Definition of a DC Motor. Retrieved 21 March 2020, from 
https://sciencing.com/definition-of-a-dc-motor-13409319.html 

  Mini Motor Reductor Metalico 10:1 Alta Potencia. Retrieved 21 March 2020, from 
https://www.electan.com/mini-motor-reductor-metalico-101-alta-potencia-p-3092.html 

  Motor de corriente continúa; tipos y partes | Tercesa. (2019). Retrieved 21 March 2020, from 
https://tercesa.com/noticias/motor-de-corriente-continua-tipos-y-partes/ 

  Rapetti, G. (2017). CÓMO FUNCIONA UN PUENTE PARA MOTORES DE CORRIENTE CONTINUA. Retrieved 19 April 2020, from  
https://www.inventable.eu/2017/05/26/funciona-puente-motores-corriente-continua/  
  
  Adafruit Industries. (2018). Adafruit TSL2591 High Dynamic Range Digital Light Sensor[Ebook (1st ed., pp. 3-18). Retrieved from 
https://www.cetronic.es/sqlcommerce/disenos/plantilla1/seccion/producto/DetalleProducto.js p?idIdioma=&idTienda=93&codProducto=999334022&cPath=1343&gclid=Cj0KCQiA- bjyBRCcARIsAFboWg2jEmebQvp0diffuzqg05AEinYSC8SN61KRFypkkhpYK0qUlXu4z- MaAphUEALw_wcB  

  Fried, L. (2018). adafruit/Adafruit_TSL2591_Library. Retrieved 14 March 2020, from  
https://github.com/adafruit/Adafruit_TSL2591_Library/blob/master/examples/tsl2591/tsl2591.ino 

  Llamas, L. (2015). Medir distancia con Arduino y sensor de ultrasonidos HC-SR04. Retrieved 18 April 2020, 
from https://www.luisllamas.es/medir-distancia-con-arduino-y-sensor-de-ultrasonidos-hc-sr04/    

