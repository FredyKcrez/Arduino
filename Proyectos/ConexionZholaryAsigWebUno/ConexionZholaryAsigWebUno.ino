#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xBA, 0xCA, 0xBA, 0xCA, 0xFE, 0xA5 }; // Dirección MAC correspondiente al arduino Wthernet
EthernetClient cliente;
IPAddress ip(192,168,1,12);
int pinA0, pinA1, pinA2;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
}

void loop() {
  if (cliente.connect("104.20.67.46",80)) { // REPLACE WITH YOUR SERVER ADDRESS
    Serial.println("Connected...");
    /*cliente.println(F("POST /servidorWebFredy/add.php HTTP/1.1"));
    cliente.println(F("Host: 192.168.1.2")); // SERVER ADDRESS HERE TOO
    cliente.println(F("Content-Type: application/x-www-form-urlencoded"));
    cliente.print(F("Content-Length: "));
    cliente.println(data.length());
    cliente.println();
    cliente.print(data);*/
  } else
  Serial.println("Disconnected...");

  if (cliente.connected()) {
    cliente.stop();  // DISCONNECT FROM THE SERVER
  }
}
