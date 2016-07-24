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
 * http://playground.arduino.cc/Code/Keypad
 */

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <Keypad.h>

#define REQ_BUF_SZ   50 // tamaño del buffer usado para obtener la petición HTTP

//Dirección MAC de la placa Ethernet
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
// Dirección IP, para el acceso al servicio
//IPAddress ip(172,16,40,11);
IPAddress ip(192,168,1,12);// Borrar
EthernetServer server(80);  // Puerto para el servicio

File webFile; // Definición para el archivo contenido en la SD card
char HTTP_req[REQ_BUF_SZ] = {0};
char req_index = 0;

/*-------Definiciones keypad--------*/
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, A1, 7}; //pines conectados a las filas del keypad
byte colPins[COLS] = {6, 5, 3, 2}; //pines conectados a las columnas del keypad

//Valores iniciales para los colores, byte para el ahorro de memoria
byte Rojo = 0;
byte Verde = 0;
byte Azul = 0;

//definimos el keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String cad = "000"; //cadena que leera caracter x caracter los numeros leidos del keypad
char letra; //contendra el color al que se esta almacenando del valor R-V-A
/*-------Fin definiciones keypad--------*/

void setup()
{
  Serial.begin(9600);       // for debugging
    
  // Inicializacion de la tarjeta SD
  Serial.println(F("Inicializando tarjeta SD..."));
  if(!SD.begin(4)) { // Inicializamos la tarjeta. PIN 4 -> CS
    Serial.println(F("ERROR al inicializar la tarjeta SD!"));
    return; 
  }
  Serial.println(F("HECHO - Tarjeta SD inicializada."));

  if(!SD.exists("index6.htm")) {  // Comprobamos que exista el archivo index.htm
    Serial.print(F("ERROR - No se puede encontrar el archivo"));
    return;
  }
  Serial.println(F("HECHO - Archivo index.htm encontrado."));
  Serial.print(F("Direccion IP del servidor: "));
  Serial.println(Ethernet.localIP());
  
  Ethernet.begin(mac, ip); // inicialisamos el dispositivo Ethernet
  server.begin(); // Capturas al servidor
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // Si el cliente esta activo
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
                        // send rest of HTTP header
                        client.println(F("Content-Type: text/xml"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // Enviamos el xml con los valores leidos desde arduino
                        XML_response(client);
                    }
                    else {  // Si es la primera vez, no se recibe una petición
                        // encabezado básico
                        client.println(F("Content-Type: text/html"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // send web page
                        webFile = SD.open("index6.htm"); // abrimos el archivo web
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); //mientras no lleguemos a la ultima linea del archivo
                            }
                            webFile.close();
                        }
                    }
                    Serial.print(HTTP_req);
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// contenido del archivo xml a enviar
void XML_response(EthernetClient cl)
{
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    char key = keypad.getKey();
    if(key) {
      /*switch que permite realizar las acciones de acuerdo a los criterios especificados:
       * 0-9 almacenar el digito en el valor a enviar
       * A para el color Rojo
       * B para el color Verde
       * C para el color Azul
       * D no hace nada
       * * para limpiar los valores
       * # enviar los valores a la página web
       */
      switch(key) {
        case '*':
          cad = "000";
          Serial.println(F("Limpieza"));
          Serial.println(cad);
          letra = 'Z';
          break;
        case '#':
          cl.print(F("<analog>"));
          cl.print(map(Rojo, 0, 255, 0, 100));
          cl.print(F("</analog>"));
          cl.print(F("<analog1>"));
          cl.print(map(Verde, 0, 255, 0, 100));
          cl.print(F("</analog1>"));
          cl.print(F("<analog2>"));
          cl.print(map(Azul, 0, 255, 0, 100));
          cl.print(F("</analog2>"));
          cl.print(F("<val>"));
          cl.print(Rojo);
          cl.print(F("</val>"));
          cl.print(F("<val1>"));
          cl.print(Verde);
          cl.print(F("</val1>"));
          cl.print(F("<val2>"));
          cl.print(Azul);
          cl.print(F("</val2>"));

          //Mandamos valores al led RGB
          //Por falta de pines digitales, hacemos uso de los pines analogicos
          analogWrite(A3, Rojo);
          analogWrite(A4, Verde);
          analogWrite(A5, Azul);
          break;
        case 'A':
          letra = 'R';
          Serial.println(F("Rojo"));
          break;
        case 'B':
          letra = 'V';
          Serial.println(F("Verde"));
          break;
        case 'C':
          letra = 'A';
          Serial.println(F("Azul"));
          break;
        case 'D':
          letra = 'Z';
          Serial.println(F("Reseteado"));
          break;
        default:
          ingresarlo(key);
          break;
      }
    }
    cl.print(F("</inputs>"));
}

void ingresarlo(char k) {
  //movemos los valores leidos anteriormente una posición a la izquierda <--
  cad[0] = cad[1];
  cad[1] = cad[2];
  cad[2] = k; //se coloca el valor leido en el presente loop en la ultima posición de la cadena
  int val = cad.toInt(); //conversion de la cadena a enter
  //verificamos que el valor a enviar este contenido en el rango de 0-255 para los colores.
  if(val>=0 && val<= 255) {
    if(letra == 'R') //si 'letra' contiene el caracter R
      Rojo = (byte)val;
    else if(letra == 'V') //si 'letra' contiene el caracter V
      Verde = (byte)val;
    else if(letra == 'A') //si 'letra' contiene el caracter A
      Azul = (byte)val;
  }
  else //de lo contrario se imprime mensaje de error
    Serial.println(F("Valor no permitido"));
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
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
