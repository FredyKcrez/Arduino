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
char req_index = 0;             // index into HTTP_req buffer

String cont_Req = "";
int red=128, grn=128, blu=128;
int redPin = 6;
int greenPin = 5;
int bluePin = 7;

void setup() {
  Serial.begin(9600);       // for debugging
    
  // Inicializacion de la tarjeta SD
  Serial.println(F("Inicializando tarjeta SD..."));
  if(!SD.begin(4)) { // Inicializamos la tarjeta. PIN 4 -> CS
    Serial.println(F("ERROR al inicializar la tarjeta SD!"));
    return; 
  }
  Serial.println(F("HECHO - Tarjeta SD inicializada."));

  if(!SD.exists("index2.htm")) {  // Comprobamos que exista el archivo index.htm
    Serial.print(F("ERROR - No se puede encontrar el archivo"));
    return;
  }
  Serial.println(F("HECHO - Archivo index2.htm encontrado."));
  Serial.print(F("Direccion IP del servidor: "));
  Serial.println(Ethernet.localIP());
  
  Ethernet.begin(mac, ip);  // initialize Ethernet device
  server.begin(); // start to listen for clients

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
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
                        SetColor();
                        XML_response(client);
                    }
                    else {  // web page request
                        // send rest of HTTP header
                        client.println(F("Content-Type: text/html"));
                        client.println(F("Connection: keep-alive"));
                        client.println();
                        // send web page
                        webFile = SD.open("index2.htm");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
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
        client.stop(); // close the connection
    } // end if (client)
}

void SetColor() {
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
    
  analogWrite(redPin, map(red,0,255,255,0));
  analogWrite(greenPin, map(grn,0,255,255,0));
  analogWrite(bluePin, map(blu,0,255,255,0));
}

void XML_response(EthernetClient cl) {
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
