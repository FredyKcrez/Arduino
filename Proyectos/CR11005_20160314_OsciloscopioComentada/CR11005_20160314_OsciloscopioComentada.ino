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
const int pinIn = A1; //pin analogico A0 para lectura del potenciometro.
int myArreglo[81]; /* Vector que contendra los puntos graficados según coordenadas, el valor 81 sale de restaler 4 pixeles a la dimensión
                    * horizontal de la pantalla 84-4=80, 80 son los pixeles que me quedan libres para la graficar, ahora aumento a 1 ese
                    * valor para que contenga el caracter de fin de un arreglo ('/0'), ahora, le resto 4 pixeles ya que el eje vertical
                    * esta dibujado sobre el pixel numero 3, eso significa que los pixeles 0,1,2,3 no deben estar habilitados para el
                    * movimiento de la grafica. */
int yCtrl; //contador usado para recolectar y recorrer el arreglo.
int posY = 0; //lee y almacena momentaneamente el valor leido del dispositivo, antes de ser almacenado en el vector.
int i; //otro contador para recolectar y recorrer el arreglo, pero utilizado cuando la pantalla a un esta llena horizontalmente.

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

  drawEjes(); //invocación del metodo para dibujar los ejes horizontal, vertical y las escalas en el eje vertical
  delay(1000);
}

void loop() {  
  for(yCtrl=0; yCtrl<=80; yCtrl++) { /* for para recorrer, llenar la matriz, y mostrar el(los) pixel(es) en la pantalla, recorre
                                      * todo el vector, inclusive la posición donde esta contenida el caracter '/0'. */
    if(yCtrl>79) { // si se encuentra en la posición que contiene el caracter '/0'.
      movPantalla(); // llamada a la función para desplazar en uno los pixeles de la pantalla.
      yCtrl = 79; // volvemos a la posición anterior a '/0' en el arreglo.
    }
    posY = analogRead(pinIn); // lectura del valor en el pin A0
    myArreglo[yCtrl] = map(posY, 0, 1023, 0, 47); // mapearlo en escala vertical de la pantalla 0-47 (dimensión vertical de la pantalla)
    display.drawPixel(yCtrl+4, myArreglo[yCtrl], BLACK); /* dibujado de pixeles, a yCtrl se le aumenta en uno para que comience a dibujar
                                                          * desde la coordenada x=4 en el eje horizontal, y mapea en el eje vertical
                                                          * el valor contenido en la posición yCtrl dentro del arreglo. */
    display.display(); //muestra la grafica en pantalla
  }
}

/*
 * Función utilizada para trazar los ejes horizontal y vertical en la pizarrita, ademas de trazar escalas para una mejor visualización
 * y entendimiento de la grafica.
 */
void drawEjes() {
  for(yCtrl=0; yCtrl<84; yCtrl++) // dibuja el eje horizontal en pantalla
    display.drawPixel(yCtrl, 23, BLACK);
  for(yCtrl=0; yCtrl<48; yCtrl++) //eje vertical
    display.drawPixel(3, yCtrl, BLACK);
  for(yCtrl=0; yCtrl<3; yCtrl++) { //escalas dentro del eje vertical
    display.drawPixel(yCtrl, 0, BLACK); //ya que la pantalla cuenta con 48 pixeles verticalmente, dividi las escalas cada 12 pixeles
    display.drawPixel(yCtrl, 11, BLACK); //para que contenga cinco puntos de referencia, que serian 1, 0.5, 0, -0.5 y -1, por lo que
    display.drawPixel(yCtrl, 35, BLACK); //cada escala esta 12 pixeles separados entre si: 0, 11, 23, 35, 47.
    display.drawPixel(yCtrl, 47, BLACK);
  }
}

/*
 * Función usada para desplazar los pixeles 1 pixel a la derecha cuando la pantalla ya ha alcanzado la coordenada 84 en el eje horizontal.
 */
void movPantalla() {
  display.clearDisplay(); //limpia la pantalla, para comenzar a dibujar nuevamente los pixeles anteriores un pixel antes.
  drawEjes(); //dibuja nuevamente los ejes y escalas.
  for(i=0; i<=80; i++) { //recorremos el arreglo, consultando los valores que contiene.
    myArreglo[i] = myArreglo[i+1]; // desplaza los valores contenidos en la posición i+1 a la posició i, lo que genera el desplazamiento
                                   // al volver a dibujar la grafica, esto demora cierta cantidad de tiempo que pareciera no importar.
    display.drawPixel(i+4, myArreglo[i], BLACK); //re-dibujado de pixeles
  }
}
