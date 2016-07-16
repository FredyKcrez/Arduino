#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

// Dirección MAC correspondiente al arduino Wthernet
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
// Dirección IP, para el acceso al servicio
//IPAddress ip(172,16,40,11);
IPAddress ip(192,168,1,12);
EthernetServer server(80);

File webFile;  // Archivo de página web almacena en microSD

void setup()
{
    Ethernet.begin(mac, ip);  // Inicialización del servidor.
    server.begin();           // Ponemos en marcha el servidor para que empiece a escuchar
    Serial.begin(9600);       // Inicialización del puerto RS232 para depurar
    
    // Inicializacion de la tarjeta SD
    Serial.println("Inicializando tarjeta SD...");
    if (!SD.begin(4)) { // Inicializamos la tarjeta. PIN 4 -> CS
        Serial.println("ERROR - Fallo al inicializar la tarjeta SD!");
        return; 
    }
    Serial.println("HECHO - Tarjeta SD inicializada.");
    
    if (!SD.exists("index.htm")) {  // Comprobamos que exista el archivo index.htm
        Serial.print("ERROR - No se puede encontrar el archivo");
        Serial.println("index.htm");
        return;
    }
    Serial.print("HECHO - Archivo ");
    Serial.print("index.htm");
    Serial.println(" encontrado.");
    
    Serial.print("Direccion IP del servidor: ");
    Serial.println(Ethernet.localIP());
}

void loop()
{
    EthernetClient client = server.available();  // Esperamos que se conete algún cliente

    if (client) {  // si hay algún cliente conectado...
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // hemos recibido algún dato del cliente
                char c = client.read(); // leemos el dato que el cliente ha mandado (navegador)
                if (c == '\n' && currentLineIsBlank) {
                    // enviamos el encabezado del documento HTML
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // Abrimos la página de la SD y la enviamos
                    webFile = SD.open("index.htm");        
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read()); 
                        }
                        webFile.close();
                    }
                    break;
                }
                // Si el cliente ya ha recibido todos los datos y ha contestado
                //enconces cerramos la conexión
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
