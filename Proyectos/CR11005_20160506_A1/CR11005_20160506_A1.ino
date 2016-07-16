#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 }; // Dirección MAC correspondiente al arduino Wthernet
//IPAddress ip(172,16,40,11); // Dirección IP, para el acceso al servicio
IPAddress ip(192,168,1,12);
EthernetServer server(80); // Uso del puerto 80 para la comunicación
File webFile;

/*const int pot1 = A0;
const int pot2 = A1;
const int pot3 = A2;
int ledPin[] = {5,6,7};*/

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  server.begin();
  SD.begin(4);
  /*pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
*/
  /*inicializar();*/
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("Entro 1");
    boolean currentLineIsBlank = true;
    while(client.connected()) {
      //Serial.println("Entro 2");
      if(client.available()) {
        char c = client.read();
        if(c == '\n' && currentLineIsBlank) {
            
            /*client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connnection: open");
            client.println();*/
            if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
            webFile = SD.open("index.htm");
            if(!webFile) 
              Serial.println("Archivo no encontrado");
            else {
              Serial.println("Archivo encontrado");
              while(webFile.available())
                server.write(webFile.read()); // Send web page to client
              webFile.close();
            }
          }
          else if(c == '\n') {
            currentLineIsBlank = true;
        }
        else if(c == '\r')
          currentLineIsBlank = false;
      }
    }
    delay(1);
    client.stop();
  }
}
