/*********************************************************************
  Universidad de El Salvador
  Facultad de Ingeniería y Arquitectura
  Escuela de Ingeniería Electrica
  Sistemas Embebidos

  Estudiante: Br. Fredy A. Cáceres Ramos
  Facilitador: Msc. e Ing. Wilber Calderón
  Fecha de creación de Sketch: 11-Marzo-2016
  Ultima modoficación del Sketch: 14-Marzo-2016
*********************************************************************/

/*
 * Objetivo: Desarrollar un sistema electrico-digital que muestre el seguimiento del cursor, atravez de una pantalla Nokia 5110 con
 * reolucion 84x48 y un inversor CD4050BE; atravez del uso de dos potenciometros para dirigir la dirección del cursor.
 */

/*
 * Referencias:
 * https://learn.adafruit.com/downloads/pdf/nokia-5110-3310-monochrome-lcd.pdf manejo y control de pantalla Nokia 5110
 * http://www.ti.com/lit/ds/symlink/cd4050b.pdf datasheet del inversor CD4050BE
 * http://hetpro-store.com/TUTORIALES/lcd-nokia-5110-arduino/
 * http://www.ardumania.es/lcd-grafico-de-84x48/
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

/*
 * Definición de Variables:
 * pinH y pinV son los pines de lectura analogica, para determinar hacia donde se deverá mover el cursor
 * x0 y y0 coordenada de inico del cursor, al centro de la pantalla
 * Xpos, Xneg, Ypos, Yneg contendran las coordenadas a las que se movera el cursor
 * uXmov, uYmov almacena el ultimo movimiento vertical y horizontal que se realizo
 * lectH y lectV variables que almacenan los valores mapeados de los potenciometros
 */

/*
 * Consejo:
 * Para mejor visualización de las coordenadas y direccionamiento del cursor abrir el monitor serie.
 */

const int pinH = A1;
const int pinV = A0;

#define x0 41
#define y0 23

int Xpos = 41;
int Xneg = 41;
int Ypos = 23;
int Yneg = 23;
int uXmov = 41;
int uYmov = 23;
//int cont = 0; //contador para ver cuando se terminar de ejecitar el movimiento del cursor.
int lectH, lectV;

/* Definición de los pines para el uso de la pantalla 5110*/
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void setup() {
  Serial.begin(9600);
  display.begin(); //inicialización de la display e impresion de la florcita, no funciona si se quita

  display.setContrast(60); //contraste de 60 para mejor visualización.
  delay(2000);
  display.clearDisplay(); //limpieza de pantalla

  display.println("Bienvenido!\n\nSeguimiento\nde cursor.\n\nEBB-FIA 2016"); //mensaje de bienvenida
  display.display(); //imprimiendo mensaje de bienvenida
  delay(2000);
  display.clearDisplay();//limpieza de pantalla

  //display.setCursor(x0,y0);
  //display.print("Hello");
  display.drawPixel(x0, y0, BLACK); //dibuja un pixel al centro de la pantalla 5110
  display.display(); //muestra el pixel inicial
}

void loop() {
  //if(cont!=4031) { //Si no se han llenado todos los pixeles de la pantalla.
  //  cont++; //aumentar el contador para marcar el pixel que ha sido utilizado.
    lectH = map(analogRead(pinH),0,1023,0,100); //mapea el valor leido del potenciometro a un rango 0-100 en el eje horizontal.
    lectV = map(analogRead(pinV),0,1023,0,100); //mapea el valor leido del potenciometro a un rango 0-100 en el eje vertical.
    Serial.print("X: ");
    Serial.print(lectH);
    Serial.print("\tY: ");
    Serial.println(lectV);
    drawMyPixel(lectH, lectV); //invocacion del modulo para dibujar el pixel en las coordenas lectH y lectV
    delay(300); //retraso de 300 milisegundos
  /*}
  else
    endSketch();*/ //si ya se cubrieron todos lo pixeles de la pantalla finaliza el sketch
}

/*
 * drawMyPixel() dibuja los pixeles en la pantalla segun las coordenadas que la función recibe, y que han sido leidas de los
 * potenciometros.
 */
void drawMyPixel(int movH, int movV) {
  if(movH > 55) { //movimiento a la izquierda
    if(Xneg>0) //si no se ha llegado al tope izquierdo de la pantalla, esto seria toda la columna 0 de la pantalla
       Xneg--; //disminuyo la coordenada en el eje negativo de las X, el cursor se mueve a la izquierda
    /*else
       Xneg=0;*/
    movH = Xneg; //defino que el movimiento a dibujarse debe ser a la izquierda de la pantalla
    uXmov = Xneg; //registro el movimiento horizontal en este ciclo.
    Xpos = Xneg; //asigno la coordenada del cursor para que en el proximo movimiento continue en esa direccion.
  }
  else if(movH < 45) { //movimiento a la derecha
    if(Xpos<83) //si no se ha llegado al tope derecho de la pantalla, esto seria toda la columna 83 de la pantalla
       Xpos++; //aumento la coordenada en el eje positivo de las X, el cursor se mueve a la derecha.
    /*else
       Xpos>84;*/
    movH = Xpos; //defino que el movimiento a dibujarse debe ser a la derecha de la pantalla.
    uXmov = Xpos; //registro el movimiento horizontal en este ciclo.
    Xneg = Xpos;  //asigno la coordenada del cursor para que en el proximo movimiento continue en esa direccion.
  }
  else //Si no hay movimiento en el eje horizontal
    movH = uXmov; //asigna a la coordenada el ultimo valor obtenido en el ciclo anterior
    
  if(movV > 55) { //movimiento hacia abajo
    if(Yneg>0) //si no se ha llegado al top de la pantalla, esto seria toda la fila 0 de la pantalla
       Yneg--; //disminuyo la coordenada en el eje negativo de las Y, el cursor se mueve hacia abajo
    /*else
       Yneg=0;*/
    movV = Yneg;
    uYmov = Yneg;
    Ypos = Yneg;
  }
  else if(movV < 45) { //movimiento hacia arriba
    if(Ypos<47)  //si no se ha llegado al fondo de la pantalla, esto seria toda la fila 47 de la pantalla
       Ypos++; //aumento la coordenada en el eje positivo de las Y, el cursor se mueve hacia arriba.
    /*else
       Ypos=48;*/
    movV = Ypos;
    uYmov = Ypos;
    Yneg = Ypos;
  }
  else //si no hay movimiento en el eje vertical
    movV = uYmov; //asigna a la coordenada el ultimo valor obtenido en el ciclo anterior
    
  display.drawPixel(movH, movV, BLACK); //dibuja el pixel segun las coordenadas movH y movV leidas y procesadas.
  display.display(); //muestra en el display el nuevo pixel.
}

/*
 * endSketch() muestra un mensaje de finalizacion del Sketch, detiene la ejecucion del programa por un minuto y reinicia el
 * cursor al centro para iniciar de nuevo, aun no funcional
 */
 /*
void endSketch() {
  display.clearDisplay(); //limpia la pantalla.
  display.print("Fin del Sketch\n\nFredy Kcrez\n\nUES - FIA"); //mensaje de finalización.
  delay(60000); //espera por un minuto.
  display.drawPixel(x0, y0, BLACK); //dibuja un pixel al centro nuevamente.
  display.display(); //muestra el pixel inicial.
}*/
