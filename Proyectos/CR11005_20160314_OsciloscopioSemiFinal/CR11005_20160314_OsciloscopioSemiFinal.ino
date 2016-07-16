/*********************************************************************
Universidad de El Salvador
Facultad de Ingeniería y Arquitectura
Escuela de ingeniería Electrica
Sistemas Embebidos

Estudiante: Br. Fredy A. Cáceres Ramos
Facilitador: Msc. e Ing. Wilber Calderón
Fecha de creación de Sketch: 14-Marzo-2016
ultima modificación del Sketch: 17-Marzo-2016
*********************************************************************/

/*
 * Objetivo: Mostrar atravez de la pantalla nokia 5110 la simulación de un oscilospio con datos leidos desde un generador de señales.
 */

/*
 * Referencias:
 * http://kio4.com/arduino/32pantallanokia.htm
 * http://filear.com/?p=211
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
const int pinIn = A1; //pin analogico A0 para lectura del potenciometro
int delayVar = 0; //delay de escala/delay
int xVar = 0; //escala en el eje x
int yCtrl; //contador usado para recolectar y recorrer el arreglo con los valores
int posY = 0; //posición del cursor en el eje y
int myArreglo[81]; //contendra los puntos a graficar en el eje Y
int i; //

void setup() {
  Serial.begin(9600);

  display.begin(); //init done, inicialización de la pantalla
  display.setContrast(60); //mejor visualizción
  display.display(); //muestra el splashscreen
  delay(2000);
  display.clearDisplay(); //limpieza de pantalla

  display.println("Bienvenido!\n\nMini\nOsciloscopio.\n\nEBB-FIA 2016"); //mensaje de bienvenida
  display.display(); //imprimiendo mensaje de bienvenida
  delay(2000);
  display.clearDisplay();//limpieza de pantalla

  drawEjes();
  delay(1000);
}

void loop() {  
  for(yCtrl=0; yCtrl<=80; yCtrl++) { //for para recorrer y llenar la matriz
    if(yCtrl>79) {
      movPantalla();
      yCtrl = 79;
    }
    posY = analogRead(pinIn); //lectura del valor en el pin
    myArreglo[yCtrl] = map(posY, 0, 1023, 0, 47); //mapearlo en escala vertical de la pantalla 0-48
    display.drawPixel(yCtrl+4, myArreglo[yCtrl], BLACK); //dibujado de pixeles
    display.display();
  }
}

void drawEjes() {
  for(yCtrl=0; yCtrl<84; yCtrl++) //eje horizontal
    display.drawPixel(yCtrl, 23, BLACK);
  for(yCtrl=0; yCtrl<48; yCtrl++) //eje vertical
    display.drawPixel(3, yCtrl, BLACK);
  for(yCtrl=0; yCtrl<3; yCtrl++) {
    display.drawPixel(yCtrl, 0, BLACK);
    display.drawPixel(yCtrl, 47, BLACK);
  }
  for(yCtrl=1; yCtrl<3; yCtrl++) {
    display.drawPixel(yCtrl, 11, BLACK);
    display.drawPixel(yCtrl, 35, BLACK);
  }
}

void movPantalla() {
  display.clearDisplay();
  drawEjes();
  for(i=0; i<=80; i++) {
    myArreglo[i] = myArreglo[i+1];
    display.drawPixel(i+4, myArreglo[i], BLACK); //dibujado de pixeles
  }
}
