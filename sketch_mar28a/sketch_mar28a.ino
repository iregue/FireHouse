//Codigo ARDUINO UNO - RECEPTOR
#include <SPI.h>
#include <nRF24L01.h> //Libreria para la radio
#include <RF24.h>
#include <LiquidCrystal.h>
 
//Declaremos los pines CE y el CSN del modulo de radio
#define CE_PIN 9
#define CSN_PIN 10

LiquidCrystal lcd(2,3,5,4,6,8);
//Variable con la dirección del canal que se va a leer
byte direccion[5] ={'S','E','D','1','9'}; 

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector para los datos recibidos
float datos[3];

void setup()
{
  lcd.begin(16,2);
 //inicializamos el NRF24L01 
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600); 
  
  //Abrimos el canal de Lectura
  radio.openReadingPipe(1, direccion);
  
    //empezamos a escuchar por el canal
  radio.startListening();
 
}
 
void loop() {
 uint8_t numero_canal;

 if ( radio.available() )
 {    
     //Leemos los datos y los guardamos en la variable datos[]
     radio.read(datos,sizeof(datos));
     
     //reportamos por el puerto serial los datos recibidos
     Serial.print("ID MODULO= " );
     Serial.print(datos[0]);
     Serial.print(" , ");
     Serial.print("SENSOR= " );
     Serial.print(datos[1]);
     Serial.print(" , ");
     Serial.print("MILLIS= " );
     Serial.print(datos[2]);
     Serial.println(" ms.");
     //Si se recibe señal de radio con niveles de humo se activa la alerta sonora
     if((int)datos[1]>=70){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ALERTA Modulo:");
      lcd.print((int)datos[0]);
      lcd.setCursor(0,1);
      lcd.print("Humo detectado");

      tone(7,294,5000); //Se manda señal a pin D7 sonido con frecuendia de 294 Hz, suena durante 5000 ms
     }
     else{
      noTone(7);
     }
 }
 else
 {
     Serial.println("No hay datos de radio disponibles");
 }
 delay(1000);
 
}
