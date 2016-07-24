/*********************************************************************
Universidad de El Salvador
Facultad de Ingeniería y Arquitectura
Escuela de ingeniería Electrica
Sistemas Embebidos

Estudiantes:
  Br. Fredy Antonio Cáceres Ramos CR11005
  Br. Yohalmo Alexander Diaz Ramirez DR12004

Facilitador: Msc. e Ing. Wilber Calderón
Fecha de creación de Sketch: 06-Mayo-2016
ultima modificación del Sketch: 13-Mayo-2016
*********************************************************************/

/*
 * Objetivo: Este sketch muestra atraves de una página web (index3.htm)
 * almacenada en una memoria microSD la lectura obtenida en el pin analógico A0.
 * La página se actualiza autómaticamente cada 30 segundo. Pero el valor de
 * la lectura se modifica en el momento del cambio de valor.
 */

/*
 * Referencias:
 * http://booleanbite.com/web/optimizacion-de-memoria-de-nuestro-codigo-de-arduino/
 * http://www.mipsandchips.com.es/2013/12/servidor-web-con-arduino-y-tarjeta-sd.html
 * http://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-AJAX-XML-web-server/
 */

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define REQ_BUF_SZ   50 // tamaño del buffer usado para obtener la petición HTTP

//Dirección MAC de la placa Ethernet
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
// Dirección IP, para el acceso al servicio
IPAddress ip(172,16,40,11);
EthernetServer server(80);  // Puerto para el servicio

File webFile; // Definición para el archivo contenido en la SD card
char HTTP_req[REQ_BUF_SZ] = {0};
char req_index = 0;

void setup()
{
  Serial.begin(9600);
    
  // Inicializacion de la tarjeta SD
  Serial.println(F("Inicializando tarjeta SD..."));
  if(!SD.begin(4)) { // Inicializamos la tarjeta. PIN 4 -> CS
    Serial.println(F("ERROR al inicializar la tarjeta SD!"));
    return; 
  }
  Serial.println(F("HECHO - Tarjeta SD inicializada."));

  if(!SD.exists("index3.htm")) {  // Comprobamos que exista el archivo index.htm
    Serial.print(F("ERROR - No se puede encontrar el archivo"));
    return;
  }
  Serial.println(F("HECHO - Archivo index3.htm encontrado."));
  Serial.print(F("Direccion IP del servidor: "));
  Serial.println(Ethernet.localIP());
  
  Ethernet.begin(mac, ip);  // inicialisamos el dispositivo Ethernet
  server.begin();           // Capturas al servidor
}

void loop()
{
    EthernetClient client = server.available();  // Obtenemos el cliente

    if (client) { // Si el cliente esta activo
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                if (c == '\n' && currentLineIsBlank) {
                    // encabezado estandar
                    client.println(F("HTTP/1.1 200 OK"));
                    // Si la petición contiene un "ajax_inputs se envia la respuesta
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        client.println(F("Content-Type: text/xml"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // Enviamos el xml con los valores leidos desde arduino
                        XML_response(client);
                    }
                    else {  // Si es la primera vez, no se recibe una petición
                        // encbezado básico
                        client.println(F("Content-Type: text/html"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // Enviamos la página web
                        webFile = SD.open("index3.htm"); // abrimos el archivo web
                        if (webFile) {
                            while(webFile.available()) { //mientras no lleguemos a la ultima
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close(); //cerramos el archivo
                        }
                    }
                    // mostramos el HTTP request recibido
                    Serial.print(HTTP_req);
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);
        client.stop(); // close the connection
    } // end if (client)
}

// contenido del archivo xml a enviar
void XML_response(EthernetClient cl)
{
    int pinA0;
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    // lectura del pin analogico A0
    pinA0 = analogRead(0);
    cl.print(F("<analog>"));
    cl.print(pinA0);
    cl.print(F("</analog>"));
    cl.print(F("</inputs>"));
}

void StrClear(char *str, char length) {
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind) {
    char found = 0;
    char index = 0;
    char len;
    len = strlen(str);
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }
    return 0;
}
