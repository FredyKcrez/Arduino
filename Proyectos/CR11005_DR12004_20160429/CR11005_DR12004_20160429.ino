/*
 * Universidad de El Salvador
 * Facultad de Ingeniería & Arquitectura
 * Escuela de Ingeniería Eléctrica
 * Sistemas Embebidos
 * 
 * Integrantes:
 * Yohalmo Alexander Diaz Ramirez   DR12004
 * Cáceres Ramos, Fredy Antonio     CR11005
 * 
 * Instructor: MsC. Wilber Calderón
 * Fecha de sketch: 29 de Abril de 2016
 */

/*
 * Páginas de referencia:
 * http://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/web-server-read-switch/
 */
 
#include <SPI.h>
#include <Ethernet.h>

int redPin = 5;
int greenPin = 7;
int bluePin = 6;

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x76 };
IPAddress ip(172,16,40,11); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80

void setup()
{
    Ethernet.begin(mac, ip);    // initialize Ethernet device
    server.begin();             // start to listen for clients

    pinMode(A0, INPUT);  // input pin pot1
    pinMode(A1, INPUT);  // input pin pot2
    pinMode(A2, INPUT);  // input pin pot3
}

void loop()
{
    setColor(map(analogRead(A0),0,982,0,255), map(analogRead(A1),0,982,0,255), map(analogRead(A2),0,982,0,255));
    
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connnection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Arduino Read Switch State</title>");
                    client.println("<meta http-equiv=\"refresh\" content=\"1\">");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>Switch</h1>");
                    client.println("<p>State of switch is:</p>");
                    GetSwitchState(client);
                    client.println("</body>");
                    client.println("</html>");
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

void GetSwitchState(EthernetClient cl)
{
    int pot1Value = map(analogRead(A0),0,982,0,255); 
    String stringOne = "<p>Pot value: ";
    String stringTwo = "</p>";
    String string1Three = stringOne + pot1Value + stringTwo;
    cl.println(string1Three); 

    int pot2Value = map(analogRead(A1),0,982,0,255); 
    String string2Three = stringOne + pot2Value + stringTwo;
    cl.println(string2Three);

    int pot3Value = map(analogRead(A2),0,982,0,255); 
    String string3Three = stringOne + pot3Value + stringTwo;
    cl.println(string3Three);
}

//Función utilizada para asignarles los valores de colores al Led RGB
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
