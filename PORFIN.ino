int count = 1;        // Multiplexeo de envio
char EstadoSEM = '0'; // Estado de semaforo
int AVerde = 0;
int ARojo = 0;
int AAmarillo = 0;
int altura=1000;



#include "config.h"

/************************ Example Starts Here *******************************/
AdafruitIO_Feed *RojoFeed = io.feed("Rojo");
AdafruitIO_Feed *VerdeFeed= io.feed("Verde");
AdafruitIO_Feed *AmarilloFeed = io.feed("Amarillo");
AdafruitIO_Feed *ApagarLedsFeed = io.feed("ApagarLeds");

AdafruitIO_Feed *VerPICfeed     = io.feed("VerPIC");
AdafruitIO_Feed *AmaPICfeed     = io.feed("AmaPIC");
AdafruitIO_Feed *RojPICfeed     = io.feed("RojPIC");

//no borrar mas
AdafruitIO_Feed *AlturaFeed     = io.feed("AlturaPIC");
void setup() {
  // start the serial connection
  Serial.begin(115200);
  Serial2.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  RojoFeed->onMessage(handleMessage);
  VerdeFeed->onMessage(handleMessage);
  AmarilloFeed->onMessage(handleMessage);
  ApagarLedsFeed->onMessage(handleMessage);
  // wait for an MQTT connection
  // NOTE: when blending the HTTP and MQTT API, always use the mqttStatus
  // method to check on MQTT connection status specifically
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  RojoFeed->get();
  VerdeFeed->get();
  AmarilloFeed->get();
  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  while(Serial2.available()>0){
    String cadena = Serial2.readStringUntil(' ');
    altura = cadena.toInt();
   }
  
  
  switch(count){
    case 1: // Verde
      Serial.print("sendingv -> ");
      Serial.println(AVerde);
      VerPICfeed->save(AVerde);
      count++;
      break;
      
    case 2: // Amarillo
      Serial.print("sendinga -> ");
      Serial.println(AAmarillo);
      AmaPICfeed->save(AAmarillo);
      count++;
      break;
      
    case 3: // Rojo
      Serial.print("sendingr -> ");
      Serial.println(ARojo);
      RojPICfeed->save(ARojo);
      count++;
      break; 

    case 4: // Altura
      Serial.print("sending cadena -> ");
      Serial.println(altura);
      AlturaFeed->save(altura);
      count=1;
      break; 
      
    default:
      count = 1;
      break;
  }
  // Because this sketch isn't publishing, we don't need
  // a delay() in the main program loop.
  delay(3000);
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  EstadoSEM = *data->value();
  if(*data->value() == '1'){
    Serial2.print('1');
    Serial.println("Rojo");
    LedORojo();
  }
  if(*data->value() == '2'){
    LedOAmarillo();
    Serial2.print('2');
    Serial.println("Amarillo");
  }
  if(*data->value() == '3'){
    LedOVerde();
    Serial2.print('3');
    Serial.println("Verde");
  }
  if(*data->value() == '4'){
    LedApagados();
    Serial2.print('4');
    Serial.print("Apagados");
  }
}
void LedApagados(){
  AVerde = 0;
  ARojo = 0;
  AAmarillo = 0;
}
void LedORojo(){
  AVerde = 0;
  ARojo = 1;
  AAmarillo = 0;
}
void LedOVerde(){
  AVerde = 1;
  ARojo = 0;
  AAmarillo = 0;
}
void LedOAmarillo(){
  AVerde = 0;
  ARojo = 0;
  AAmarillo = 1;
} 
