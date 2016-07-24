#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>

/*
 * Token twitter: 701594541090942976-nEurElfKV8QZoAdB2s8zB59kZsdOgwf
 */

// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>


byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
// Dirección IP, para el acceso al servicio
//IPAddress ip(192,168,1,12);
byte ip[] = { 192, 168, 1, 12 };

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("701594541090942976-vfPEzULJuquGqLMknXk4QBGdOPOiVU7");

// Message to post
String msg = "Se ha superado la lectura de 3.4V. Se que no te importa pero igual queria hacertelo saber :)";

void setup()
{
  delay(1);
  Ethernet.begin(mac, ip);
  // or you can use DHCP for autoomatic IP address configuration.
  // Ethernet.begin(mac);
  Serial.begin(9600);
}

void loop()
{
  float lectura = fmap(analogRead(A0), 0, 1023, 0, 5);
  if(lectura > 3.4){
    Serial.println("connecting ...");
    tweet(msg);
  }
  delay(3000);
}

void tweet(String strMessage) {
  char msg[strMessage.length() + 1];//Mensaje a enviar cada vez que se cumpla la condicion de temperatura o humo
 
  strMessage.toCharArray(msg, 100);
 
  Serial.println("conectando...");//enviamos mensaje a pc que empezo a conectarse
 
  if (twitter.post(msg)) { //envia mensaje a twitter
    int status = twitter.wait(&Serial);//envia datos de la conexion con twtter al pc
    if (status == 200) {//el estado de envio es correcto
      Serial.println("OK");// envia a pc
    } else {
      Serial.print("fallo : codigo ");// si el estado es diferente algo fallo, y muestra el codigo del fallo
      Serial.println(status);// imprime el codigo del estado
    }
  } else {
    Serial.println("Fallo conexion");//fallo de conexion a internet, verificar mac, ip, gateway, y subnet
  }
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
