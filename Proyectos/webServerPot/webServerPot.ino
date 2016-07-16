#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   50

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
// Dirección IP, para el acceso al servicio
//IPAddress ip(172,16,40,11);
IPAddress ip(192,168,1,12);// depending on network
EthernetServer server(80);  // create a server at port 80

File webFile;               // the web page file on the SD card
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

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

  if(!SD.exists("index.htm")) {  // Comprobamos que exista el archivo index.htm
    Serial.print(F("ERROR - No se puede encontrar el archivo"));
    return;
  }
  Serial.println(F("HECHO - Archivo index.htm encontrado."));
  Serial.print(F("Direccion IP del servidor: "));
  Serial.println(Ethernet.localIP());
  
  Ethernet.begin(mac, ip);  // initialize Ethernet device
  server.begin();           // start to listen for clients
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println(F("HTTP/1.1 200 OK"));
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        // send rest of HTTP header
                        client.println(F("Content-Type: text/xml"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // send XML file containing input states
                        XML_response(client);
                    }
                    else {  // web page request
                        // send rest of HTTP header
                        client.println(F("Content-Type: text/html"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // send web page
                        webFile = SD.open("index.htm");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    // display received HTTP request on serial port
                    Serial.print(HTTP_req);
                    // reset buffer index and all buffer elements to 0
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

// send the XML file with switch statuses and analog value
void XML_response(EthernetClient cl)
{
    int pinA0, pinA1, pinA2;
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    // read analog pin
    pinA0 = analogRead(0);
    pinA1 = analogRead(1);
    pinA2 = analogRead(0);
    cl.print("<analog>");
    cl.print(map(pinA0, 0, 1023, 0, 100));
    cl.print("</analog>");
    cl.print("<analog1>");
    cl.print(map(pinA1, 0, 1023, 0, 100));
    cl.print("</analog1>");
    cl.print("<analog2>");
    cl.print(map(pinA2, 0, 1023, 0, 100));
    cl.print("</analog2>");
    cl.print("</inputs>");
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
