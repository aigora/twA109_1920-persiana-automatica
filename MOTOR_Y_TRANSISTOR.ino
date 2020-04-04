const int base=12;

void setup() {
  // put your setup code here, to run once:
pinMode(base,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(base ,HIGH);
//Espera x segunos
delay(30000); //Tiempo que gira
//Apagado del transistor
digitalWrite(base,LOW);
//Espera x segundos
delay(100000000); //Tiempo que para
}
