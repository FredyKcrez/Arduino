/*
  Web client

This sketch connects to a website (http://www.google.com)
using an Arduino Wiznet Ethernet shield.

Circuit:
* Ethernet shield attached to pins 10, 11, 12, 13

created 18 Dec 2009
by David A. Mellis

*/

#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
IPAddress ip(192,168,1,12);// depending on network
IPAddress server(82,165,139,80); // Google

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
String codigo;
void setup() {
  // start the serial library:
  Serial.begin(57600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET http://www.pandorasoft.com/arduino/index.php?var=100 HTTP/1.0");
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    codigo = codigo + c; /// genero el string con el código completo
    //Serial.print(myPins[0]);
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    //Serial.print(codigo);
   
    int posicion1 = codigo.indexOf("<p1>"); /// en la web la primera palabra empieza por <p1> a partir de aquí esta la primera palabra
    int posicion2 = codigo.indexOf("<p2>");/// en la web la segunda palabra empieza por <p2> a partir de aquí esta la segunda palabra
    String palabra1 = codigo.substring ( posicion1 + 4, posicion1 + 14 ); // el +4 es porque <p1> no me vale y mostrare a partir de que termine ----- el + catorce es para contar hasta 10 dígitos que no va a contener mas de 10 dígitos
    String palabra2 = codigo.substring ( posicion2 + 4, posicion2 + 14 );
      Serial.println(posicion1); // muestra donde empieza la 1 posición
      Serial.println(posicion2); // muestra donde empieza la 2 posición
      Serial.println(palabra1); // ya tengo la primera palabra metida en la variable palabra1
      Serial.println(palabra2); // ya tengo la segunda palabra metida en la variable palabra2
   
    // do nothing forevermore:
    for(;;)
      ;
  }
}
