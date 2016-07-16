#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;

void setup()
   {  Serial.begin(9600);
      while (!Serial)  ;       // wait for serial port to connect. Needed for Leonardo only
      Serial.print("Probando SD card...");
      //pinMode(10, OUTPUT);         // Esto es necesario aunque creas que no lo usas.
      if (!SD.begin(chipSelect))
           {  Serial.println("No hay tarjeta");
              return;    // NO sigas
           }
      Serial.println("Sd encontrada.");
      File dataFile = SD.open("index.htm");
     if (dataFile)   // Si ha podido abrir el fichero
        {    while (dataFile.available())              // Mientras no llegues al final
                  Serial.write(dataFile.read());       // Lee el fichero y envialo al serie
             dataFile.close();                         // Cierra el fichero al acabar
        }
     else     // SI no puede abrir el fichero
           Serial.println("Error, no puedo usar indes.htm");
   }
void loop()
   { }

