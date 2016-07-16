/*********************************************************************
Universidad de El Salvador
Facultad de Ingeniería y Arquitectura
Escuela de ingeniería Electrica
Sistemas Embebidos

Estudiante: Br. Fredy A. Cáceres Ramos
Facilitador: Msc. e Ing. Wilber Calderón
Fecha de creación de Sketch: 14-Marzo-2016
ultima modificación del Sketch: 31-Marzo-2016
*********************************************************************/

/*
 * Objetivo: Mostrar atravez de la pantalla nokia 5110 la simulación de un oscilospio con datos leidos desde un generador de señales.
 */

/*
 * Referencias:
 * Archivo Adafruit_GFX.cpp de la libreria Adafruit
 * http://kio4.com/arduino/32pantallanokia.htm (9 Miniosciloscopio)
 * http://www.seta43.netau.net/ardu_os.html aumento de velocidad de muestreo
 */

/*
 * Observaciones:
 * Valores utilizados en el generador de datos para las pruebas:
 * Frecuencia: 1 kHz
 * Periodo: 1 ms
 * Amplitud: 3 Vpp
 * Offset: 1 Vdc
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

/*
 * Variables:
 */
byte myArreglo[500]; //Almacena los valores leidos, maximo de lectura 698 datos
int yCtrl; //contador usado para recolectar y recorrer el arreglo.
int posY = 0; //lee y almacena momentaneamente el valor leido del dispositivo, antes de ser almacenado en el vector.
int j, aux; //otro contador para recolectar y recorrer el arreglo, pero utilizado cuando la pantalla a un esta llena horizontalmente.
unsigned long tStart, tEnd; //captan el tiempo antes y despues de la lectura de los datos

void setup() {
  Serial.begin(9600);

  bitWrite(ADCSRA,ADPS2,1);
  bitWrite(ADCSRA,ADPS1,0);
  bitWrite(ADCSRA,ADPS0,0);

  //Definiendo el puerto analogico A0 como el puerto de lectura de datos
  ADMUX=(1<<ADLAR)|(0<<REFS1)|(1<<REFS0)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);

  display.begin(); //init done, inicialización de la pantalla
  display.setContrast(60); //mejor visualizción
  display.display(); //muestra el splashscreen
  delay(1000);
  display.clearDisplay(); //limpieza de pantalla

  display.println("Bienvenido!\n\nMini\nOsciloscopio.\n\nEBB-FIA 2016"); //mensaje de bienvenida
  display.display(); //imprimiendo mensaje de bienvenida
  delay(2000);
  display.clearDisplay();//limpieza de pantalla
}

void loop() {
  drawEjes(); //invocación del metodo para dibujar los ejes horizontal, vertical y las escalas en el eje vertical
  tStart = micros(); //leo el tiempo que ha transcurrido desde el inico del sketch y el inicio de las lecturas
  for(yCtrl=0; yCtrl<500; yCtrl++) { //Llenado del vector
    posY = analogReadFast(); //lectura rapida
    myArreglo[yCtrl] = map(posY, 0, 255, 39, 0); //Mapeo para no mostrar la grafica invertida
  }
  tEnd = micros() - tStart; //tiempo despues de la lecturas - tiempo antes de las mismas.
  //tEnd -= tStart; //igual que la linea anterior, pero sobreescribiendo la variable.
  display.setCursor(7, 41);
  display.print("f:");
  display.print((float)50000/(float)tEnd); //los 50000, son las 500 muestras multiplicadas por 100
  display.print(" kHz");

  for(yCtrl=0; yCtrl<=80; yCtrl++) { //muestra la grafica
    display.drawLine(yCtrl+4, myArreglo[yCtrl], yCtrl+5, myArreglo[yCtrl+1], BLACK); //dibujado de lineas de pixeles
    display.display(); //muestra la grafica en pantalla
  }
  display.clearDisplay();
}

/*
 * Función utilizada para trazar los ejes horizontal y vertical en la pizarrita, ademas de trazar escalas para una mejor visualización
 * y entendimiento de la grafica.
 */
void drawEjes() {
  display.drawFastHLine(0, 20, 84, BLACK); // dibuja de forma rápida el eje horizontal
  display.drawFastVLine(3, 0, 40, BLACK); // dibuja de forma rápida el eje vertical
  //Dibujo de escalas en el eje vertical
  display.drawFastHLine(0, 0, 3, BLACK);
  /*display.drawFastHLine(0, 9, 3, BLACK);
  display.drawFastHLine(0, 30, 3, BLACK);*/
  display.drawFastHLine(0, 39, 3, BLACK);
  display.display();
}

/*
 * Función utilizada para realizar la lectura de datos de una manera mas rápida, basado en el segundo enlace de referencia.
 */
int analogReadFast()
{
 ADCSRA|=(1<<ADSC);
 while (bit_is_set(ADCSRA, ADSC)); // Se limpia el ADSC cuando termina la conversión
        return ADCH;
}

/*
 * Función usada para desplazar los pixeles 1 pixel a la derecha cuando la pantalla ya ha alcanzado la coordenada 84 en el eje horizontal.
 */
void movPantalla() {
  int z = aux;
  display.clearDisplay(); //limpia la pantalla, para comenzar a dibujar nuevamente los pixeles anteriores un pixel antes.
  drawEjes(); //dibuja nuevamente los ejes y escalas.
  for(j=z; j<=z+79; j++) { //recorremos el arreglo, consultando los valores que contiene.
    //myArreglo[j] = myArreglo[j+1]; // desplaza los valores contenidos en la posición i+1 a la posició i, lo que genera el desplazamiento
                                   // al volver a dibujar la grafica, esto demora cierta cantidad de tiempo que pareciera no importar.
    display.drawLine(j-z+4, myArreglo[j], j-z+5, myArreglo[j+1], BLACK); //re-dibujado de pixeles
  }
}
