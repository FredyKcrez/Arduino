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
 * Objetivo: Este sketch permite controlar le color del fondo de pantalla de la
 * página web (index.htm) almacenada en una memoria microSD atravez de tres
 * potenciometros conectados a los pines A0, A1 y A2.
 * Cada uno de los potenciometros representa uno de los tres colores básicos:
 * Rojo, Verde y Azul.
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

//Pines para el led RGB
int redPin = 6;
int greenPin = 5;
int bluePin = 7;

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

  if(!SD.exists("index1.htm")) {  // Comprobamos que exista el archivo htm
    Serial.print(F("ERROR - No se puede encontrar el archivo"));
    return;
  }
  Serial.println(F("HECHO - Archivo index.htm encontrado."));
  Serial.print(F("Direccion IP del servidor: "));
  Serial.println(Ethernet.localIP());
  
  Ethernet.begin(mac, ip);  // inicialisamos el dispositivo Ethernet
  server.begin();           // Capturas al servidor

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()
{
    EthernetClient client = server.available();  // Obtenemos el cliente

    if (client) {  // Si el cliente esta activo
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          if (req_index < (REQ_BUF_SZ - 1)) {
            HTTP_req[req_index] = c;
            req_index++;
          }
          if (c == '\n' && currentLineIsBlank) {
            // encabezado estandar
            client.println(F("HTTP/1.1 200 OK"));
            // Si la petición contiene un "ajax_inputs se envia la respuesta
            if (StrContains(HTTP_req, "ajax_inputs")) {
              client.println(F("Content-Type: text/xml")); // Formato de la respuesta (xml)
              client.println(F("Connection: keep-alive")); // mantenemos activa la conexión
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
              webFile = SD.open("index1.htm"); // abrimos el archivo web
              if (webFile) {
                while(webFile.available()) { //mientras no lleguemos a la ultima linea del archivo
                  client.write(webFile.read()); // send web page to client
                }
              webFile.close(); // hemos terminado de leer el archivo, cerramos
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
      } // fin if(client.available())
    } // fin while(client.connected())
    delay(1);
    client.stop(); //cerramos la conección
  } // fin if(client)
}

// contenido del archivo xml a enviar
void XML_response(EthernetClient cl)
{
    byte inA0, inA1, inA2;
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    // lectura de los pines analógicos y mapeos para los colores
    inA0 = map(analogRead(0), 0, 1023, 0, 255);
    inA1 = map(analogRead(1), 0, 1023, 0, 255);
    inA2 = map(analogRead(2), 0, 1023, 0, 255);
    cl.print(F("<analog>"));
    //mapeos para el porcentaje (0 - 100)%
    cl.print(map(inA0, 0, 255, 0, 100));
    cl.print(F("</analog>"));
    cl.print(F("<analog1>"));
    cl.print(map(inA1, 0, 255, 0, 100));
    cl.print(F("</analog1>"));
    cl.print(F("<analog2>"));
    cl.print(map(inA2, 0, 255, 0, 100));
    cl.print(F("</analog2>"));
    cl.print(F("<val>"));
    cl.print(inA0);
    cl.print(F("</val>"));
    cl.print(F("<val1>"));
    cl.print(inA1);
    cl.print(F("</val1>"));
    cl.print(F("<val2>"));
    cl.print(inA2);
    cl.print(F("</val2>"));
    cl.print(F("</inputs>"));

    //Mandamos valores al led RGB
    analogWrite(redPin, inA0);
    analogWrite(greenPin, inA1);
    analogWrite(bluePin, inA2);
}

// sets every element of str to 0 (clears array)
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
