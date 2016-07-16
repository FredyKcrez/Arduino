// Escribir datos en una tarjeta SD con Arduino - Datalogger http://zygzax.com/
 
//Incluimos la libreria SD.
#include <SD.h>
// Guardamos en que entrada de Arduino esta conectado el pin CS del modulo.
const int chipSelect = 4;
 
void setup(){
  // Configuramos el puerto serie para informar de fallos a traves de el.
  Serial.begin(9600);
   
  // El pin CS por defecto de la placa arduino debe ser configurado como salida
  // aunque no se use (10 en la mayoria de las placas, 53 en Arduino Mega).
  //pinMode(10, OUTPUT);
   
  // Si ha habido error al leer la tarjeta informamos por el puerto serie.
  if (!SD.begin(chipSelect)){
    Serial.println("Error al leer la tarjeta.");
    return;
  }
}
 
void loop(){
   
  // Creamos las variable donde vamos a guardar el valor del potenciometro.
  int pot=0;
   
  // Leemos los datos del potenciometro.
  pot=12;
     
  // Abrimos el fichero donde vamos a guardar los datos (Si no existe se crea automaticamente).
  File dataFile = SD.open("datos.csv", FILE_WRITE);
     
  // Si el fichero es correcto escribimos en el.
  if (dataFile) {
    // Escribimos en el fichero "POT: "
    dataFile.print("POT: ");
    // A continuacion escribimos el valor de la variable pot y saltamos a la linea siguiente.
    dataFile.println(pot);
    dataFile.println("-----5s-----");
    // Cerramos el archivo.
    dataFile.close();
     
    // Avisamos de que se ha podido escribir correctamente.
    Serial.println("impresion correcta");
     
  // Si no pudimos escribir en el fichero avisamos por el puerto serie.
  }else{
    Serial.println("Error al escribir en valorpot.txt");
  }
   
  // Esperamos 5s para tomar la siguiente medida.
  delay(5000);
}
