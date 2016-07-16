/*
 * 
 * Universidad de El Salvador
 * Facultad de Ingeniería & Arquitectura
 * Escuela de Ingeniería Eléctrica
 * Sistemas Embebidos
 * Ciclo I - 2016
 * 
 * Integrantes:
 * 
 * Instructor: Ing. y MsC. Wilber Calderón
 * Fecha de Sketch: 2016/05/04
 * 
 */

/*
 * 
 * Descripción del Sketch:
 * Control de página web que actualice la página cada 30 segundo, cuando la lectura de el Analógico 0(A0) no sea modificado,
 * si se modifica la lectura, se actualiza únicamente ese campo de la paǵina web.
 * 
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x76 }; // Dirección MAC correspondiente al arduino Wthernet
IPAddress ip(172,16,40,11); // Dirección IP, para el acceso al servicio
EthernetServer server(80); // Uso del puerto 80 para la comunicación

const int pinL = A0;
int lectura;

void setup() {
  Ethernet.begin(mac, ip); // Inicialización del dispositivo Ethernet
  server.begin();

  pinMode(pinL, INPUT);
}

void loop() {
  lectura = analogRead(pinL);
  EthernetClient client = server.available(); // verificar el estado del servidor
  if(client) {
    boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
              conexion(lectura);
            }
        }
    client.stop();
  }
}
