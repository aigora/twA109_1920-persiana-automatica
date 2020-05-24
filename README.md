# Persiana automática

Desarrollar un motor que permita subir la persiana de una habitación mediante un sistema de reducido tamaño de poleas y carretes, que se activa con un sensor lumínico. Cuando el sensor haya detectado un determinado nivel de iluminación (correspondiente al del amanecer), este manadará una señal al ordenador para que active un motor que hará que suba la persiana medienate un carrete, todo a través de una placa Arduino.

## Integrantes del equipo

Nombre y apellidos: Inés Portilla de Birazel  
Número de matrícula: 55409
Usuario GitHub: @InesPortilla

## Objetivos del trabajo
-Consultar tutoriales y guías de Arduino para rofundizar mi conocimiento en programación con Arduino. En especial en cuanto a sensores lumínicos y de ultrasonidos (entrada) y mini motores (salida).  
-Configurar un sensor lumínico para que envíe datos al programa justo después de leer un determinado nivel de iluminación.  
-Configurar un sensor ultrasonido para que envíe datos al programa justo después de leer una distancia hasta la superficie más cercana a este.  
-Enviar datos de la hora actual desde el reloj integrado en un ordenador al programa.
-Programar el sensor para que envíe datos al PC justo después de leer un determinado nivel de iluminación.  
-Activar un motor al recibirse un dato específico (de luminosidad, distancia o tiempo).  
-Desarrollar un modelo reducido de una ventana con persiana, que permita llevar a cabo el movimiento vertical resultante de subirla y bajarla.

## Hardware
PC o Mac, placa Arduino UNO Rev. 3, sensor de luz para Arduino, mini motor eléctrico, sensor de ultrasonido, protoboard, cables, cable para batería de 9V, puente H, batería de 9 voltios.

## Materiales para la ventana
Un marco de madera (el marco de una caja de vino), dos tubos de cartón piedra de distintos radios, un carrete, un trozo de tela de 30x50 cm aproximadamente, tornillos (para aferrar las placas al marco), silicona caliente, un trozo plano de madera de 0,5x1,5x9,5 cm, cuatro clavos en L (para sujetar la protoboard y el trozo de matera), cinta de doble faz gruesa, y cinta americana.

## Software 
Programación del microcontrolador a través del IDE de Arduino y Processing.

# Sensores y actuadores

## Persiana automática con Arduino
Sistema electrónico conformado por un pequeño motor DC que provoca el giro de un carrete que hace rotatar la barra de la persiana, para poder subirla y bajarla automáticamente. Para tal efecto, el motor se activa con un sensor lumínico cuando este detecta un nivel de iluminación correspondiente al crepúsculo (salida y puesta del sol). Esta comunicación entre el sensor (entrada) y el motor (salida) se efectúa a través de un microcontrolador de Arduino. Después, el motor para de girar cuando, al estar subiéndose la persiana, un sensor de ultrasonido (ubicado unos centímetros debajo del rollo de la persiana) detecta el borde inferior de esta.

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
 
### Detener el motor - Sensor Ultrasonido
 Un sensor de ultrasonidos es un dispositivo empleado para medir distancias mediante el envío de un pulso de alta frecuencia (no audible por el ser humano) que rebota en los objetos cercanos y vuelve al ultrasonido. Esto es posible gracias a que tiene un micrófono integrado adecuado para esta frecuencia. Más en detalle, conociendo la velocidad del sonido y midiendo el tiempo entre pulsos se puede estimar la medida de la distancia entre el dispositivo y el objeto más cercano.  
 
 Sabiendo que la velocidad del sonido es 343m/s, sus unidades se tranforman a centímetros por microsegundos:  
 
 343 m/s → 1/29.2 cm/µs  
 
 Distancia (cm) = Tiempo(µs)/29.2x2  
 
 Se divide el tiempo entre dos porque por defecto el sensor lee la distancia de ida y vuelta pero solo se necesitamos la mitad de la distancia total (que vendría siendo la "ida" o la "vuelta" del pulso).  
 
 Los sensores de ultrasonido son sensores de baja precisión, es decir, la orientación de la superficie a medir puede provocar que la onda se refleje, falseando la medición. Además, no son muy adecuados para entornos con un gran número de objetos ni para ser usado en exteriores. Sin embargo, este sensor es para ser usado en el interior del marco de una persiana, apuntando al otro extremo interior del marco, y dentro de un entorno doméstico. Incluso, su rango de medición real es de 20cm a 2m, apropiado para lo que se quiere usar. Son sensores muy utilizandos dentro del mundo de la robótica, son baratos y sencillos de usar.
 Se ensambla a la protoboard de la siguiente manera:
 
  ![Alt text](https://www.luisllamas.es/wp-content/uploads/2015/06/arduino-ultrasonidos-montaje.png)
 
 Y para medir las distancias se usará eeste código, como base:
 
```
#define IN3 5
#define IN4 4 
#define echoPin 8 // Echo Pin
#define trigPin 9 // Trigger Pin

float duracion, distancia; 

void setup() {
  Serial.begin (9600);
  
  pinMode(trigPin, OUTPUT);     //pines del sensor de ultrasonido
  pinMode(echoPin, INPUT);

  pinMode (IN4, OUTPUT);
  pinMode (IN3, OUTPUT);
  }

void loop() 
{
  
  digitalWrite (IN4, HIGH); //Se enciende el motor
  digitalWrite (IN3, LOW);
  
  digitalWrite(trigPin, HIGH);
  delay(5000); 
  digitalWrite(trigPin, LOW);
  
  duracion = pulseIn(echoPin, HIGH); //Tiempo en microsegundos en recibir el pulso
  
  //se calcula la distancia en cm dependiendo de la velocidad del sonido
  distancia = (duracion/2)/29.1; //velocidad del sonido: 343 m/s

  if (distancia<30)
  { digitalWrite (IN4,LOW); }
  else
  {  Serial.print("Distancia: ");
     Serial.println(distancia);
     delay(10)
     }
 }
 ```

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

### Pedir sincronización del tiempo
* Nombre: requestSync()
* Argumentos: void. No se le pide al usuario que introduzca un valor. 
* Procesamiento: 
1. La biblioteca TimeLib detecta un desbordamiento en la función millis (se utiliza para contar el paso del tiempo).
2. Envía a Processing un caracter (un 7, llamado TIME_HEADER) a través de l puerto serie.
```
time_t requestSync()
{
  Serial.write(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}
```

### Enviar datos del tiempo
* Nombre: sendTimeMessage
* Argumentos: void. No se le pide al usuario que introduzca un valor. 
* Procesamiento: TIME_HEADER y el tiempo actual (t)
1. Se lee el tiempo
2. Se formatea y se envía a Arduino por el puerto serie
```
void sendTimeMessage(String header, long time) {
  String timeStr = String.valueOf(time);
  myPort.write(header);  // send header and time to arduino
  myPort.write(timeStr);
  myPort.write('\n');
  ```

### Sincronizar Arduino con la hora actual
* Nombre: processSyncMessage()
* Argumentos: TIME_HEADER
* Procesamiento: 
1. Mediante Serial.find(), detecta si ha recibido la señal de que Processing ha procesado y enviado el tiempo.
2. Declara la variable pctime, donde almacena el tiempo leído por Processing.
3. Sincroniza Arduino con la hora actual mediante setTime().
```
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}
```
### Mostrar por el monitor serie la hora actual
* Nombre: digitalClockDisplay()
* Argumentos: pctime
* Procesamiento: Recibe la hora actual con el valor pctime y muesatra los datos por pantalla.
```

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
```

### Imprimir los minutos y segundos
* Nombre: printDigits()
* Argumentos: digits (los minutos y segundos del pctime)
* Procesamiento:
1. Recibe los datos de los minutos y segundos procesados por Processing
2. Imprime un doble punto (:) para separar cada unidad de tiempo
3. Si el digito recibido es mayor que 0, se imprime

```
void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
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
* Procesamiento: Se configura la ganancia (cuán sensible será el sensor a la luz que reciba) y el tiempo de integración (durante cuánto tiempo tomarán las muestras de luz). Para el proyecto se escogió una ganancia media (usada para propósitos más generales) y un tiempo de integración de 300 milisegundos (la mediana dentro del rango de los valores de tiempo del sensor). 
 
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

### Obtención de la hora 
* Nombre biblioteca: processing.serial.*;
* Nombre: getTimeNow() 
* Argumentos: No aplica, pero devuelve la hora 
* Procesamiento:  
1. Se declara una variable (d) y se almacena en ella la hora, minutos y segundos actuales
2. Devuelve el valor a la función sendTimeMessage()
```

long getTimeNow(){
  // java time is in ms, we want secs    
  Date d = new Date();
  Calendar cal = new GregorianCalendar();
  long current = d.getTime()/1000;
  long timezone = cal.get(cal.ZONE_OFFSET)/1000;
  long daylight = cal.get(cal.DST_OFFSET)/1000;
  return current + timezone + daylight; 
}
```

### Establecer el tiempo
* Nombre biblioteca: Timelib.h
* Nombre: setTime()
* Argumentos: pctime (la hora del ordenador procesada)
* Procesamiento: Establece en Arduino que la hora actual es la recibida desde Processing
```setTime(pctime);```

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

