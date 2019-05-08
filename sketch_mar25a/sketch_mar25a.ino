//Codigo ARDUINO NANO - EMISOR
#include <SPI.h>
#include <nRF24L01.h> // Libreria para la radio
#include <RF24.h>

//Declaremos los pines CE y el CSN del modulo de Radio
#define CE_PIN 9
#define CSN_PIN 10
 
//Variable con la dirección del canal por donde se va a transmitir
byte direccion[5] ={'S','E','D','1','9'};
int id_modulo = 1;
//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector con los datos a enviar
float datos[3];

void setup()
{
  //inicializamos el NRF24L01 
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600); 
 
//Abrimos un canal de escritura
 radio.openWritingPipe(direccion);
 
}
 
void loop()
{ 
 int adc_MQ = analogRead(A0); //Lectura de la salida analogica A0 donde se encuentra el sensor MQ-135
 float voltaje = adc_MQ * (5.0 / 1023.0); //Conversion de la lectura en un valor de voltaje
 
 //cargamos los datos en la variable datos[]
 datos[0]=id_modulo;
 datos[1]=adc_MQ;
 datos[2]=millis();
 //enviamos los datos
 bool ok = radio.write(datos, sizeof(datos));
  //reportamos por el puerto serial los datos enviados 
  if(ok && adc_MQ >=200) //PARA TEST adc_MQ > 10. PARA DETECTAR HUMO adc_MQ >= 110
  {
     Serial.print("Datos enviados: "); 
     Serial.print(datos[0]); 
     Serial.print(" , "); 
     Serial.print(datos[1]); 
     Serial.print(" , "); 
     Serial.println(datos[2]); 
  }
  else
  {
    Serial.println(adc_MQ);
     Serial.println("no se ha podido enviar");
  }
 //Si se detecta humo se activa la alerta sonora
 if(adc_MQ >=200){
  tone(7,294,5000); //Se manda señal a pin D7 sonido con frecuendia de 294 Hz, suena durante 5000 ms
 }
 else{
  noTone(7);
 }
  delay(1000);
}
 
