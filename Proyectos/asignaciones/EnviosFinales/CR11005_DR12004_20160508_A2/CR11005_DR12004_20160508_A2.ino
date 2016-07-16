/*********************************************************************
  Universidad de El Salvador
  Facultad de Ingeniería y Arquitectura
  Escuela de ingeniería Electrica
  Sistemas Embebidos

  Estudiantes:
  Br. Fredy Antonio Cáceres Ramos CR11005
  Br. Yohalmo Alexander Diaz Ramirez DR12004

  Facilitador: Msc. e Ing. Wilber Calderón
  Fecha de creación de Sketch: 08-Mayo-2016
  ultima modificación del Sketch: 13-Mayo-2016
*********************************************************************/

/*
   Objetivo: Este sketch permite controlar el color de un led RGB atravez
   de la pagina web la cual contiene tre slider.
   Cada uno de los slider representa uno de los tres colores básicos:
   Rojo, Verde y Azul.
*/

/*
   Referencias:
   http://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-AJAX-XML-web-server/
   http://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-IO/
*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define REQ_BUF_SZ   60

// direccion MAC segun etiqueta del Arduino Ethernet
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
IPAddress ip(172, 16, 40, 11); // direccion IP, segun red
EthernetServer server(80);  // crear servidor en el puerto 80

File webFile;
char HTTP_req[REQ_BUF_SZ] = {0};
char req_index = 0;

int red = 0, grn = 0, blu = 0;

int redPin = 6;
int greenPin = 5;
int bluePin = 7;

void setup()
{
  // disable Ethernet chip
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.begin(9600);

  Serial.println("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;
  }
  Serial.println("SUCCESS - SD card initialized.");

  if (!SD.exists("index2.htm")) {
    Serial.println("ERROR - Can't find index.htm file!");
    return;
  }
  Serial.println("SUCCESS - Found index.htm file.");

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  EthernetClient client = server.available();

  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (req_index < (REQ_BUF_SZ - 1)) {
          HTTP_req[req_index] = c;
          req_index++;
        }

        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");

          if (StrContains(HTTP_req, "ajax_inputs")) {
            client.println("Content-Type: text/xml");
            client.println("Connection: keep-alive");
            client.println();
            SetValues();
            XML_response(client);
          }
          else {
            client.println("Content-Type: text/html");
            client.println("Connection: keep-alive");
            client.println();
            webFile = SD.open("index2.htm");
            if (webFile) {
              while (webFile.available()) {
                client.write(webFile.read());
              }
              webFile.close();
            }
          }

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
    client.stop();
  } // end if (client)
}

void SetValues(void)
{
  if (StrContains(HTTP_req, "RED=")) { //GET /ajax_inputsRED=077GRN=178BLU=063
    String valR = HTTP_req;
    String subR = valR.substring(20, 23);
    red = subR.toInt();

    Serial.println("");
    Serial.println(red);
  }

  if (StrContains(HTTP_req, "GRN=")) {
    String valG = HTTP_req;
    String subG = valG.substring(27, 30);
    grn = subG.toInt();

    Serial.println(grn);
  }

  if (StrContains(HTTP_req, "BLU=")) {
    String valB = HTTP_req;
    String subB = valB.substring(34, 37);
    blu = subB.toInt();

    Serial.println(blu);
  }

  analogWrite(redPin, map(red, 0, 255, 255, 0));
  analogWrite(greenPin, map(grn, 0, 255, 255, 0));
  analogWrite(bluePin, map(blu, 0, 255, 255, 0));
}

void XML_response(EthernetClient cl)
{
  cl.print("<?xml version = \"1.0\" ?>");
  cl.print("<inputs>");

  cl.print("<color>");
  cl.print(red);
  cl.print("</color>");

  cl.print("<color>");
  cl.print(grn);
  cl.print("</color>");

  cl.print("<color>");
  cl.print(blu);
  cl.print("</color>");

  cl.print("</inputs>");
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}

char StrContains(char *str, char *sfind)
{
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
