/*********************************************************************
Universidad de El Salvador
Facultad de Ingeniería y Arquitectura
Escuela de ingeniería Electrica
Sistemas Embebidos

Estudiante: Br. Fredy A. Cáceres Ramos
Facilitador: Msc. e Ing. Wilber Calderón
Fecha de creación de Sketch: 2016/07/04
Última modificación del Sketch: 2016/07/10
*********************************************************************/

/*
 * Proyecto: Medidor de Voltaje, Corriente y Energia - EBB115_2016
 * Objetivo: 
 */

/*
 * Referencias:
 * http://www.electroensaimada.com/spi.html
 * https://github.com/FriedCircuits/ADE7753/blob/master/ADE7753/ADE7753.ino
 * http://www.avrfreaks.net/forum/problem-reading-ade7753-registers
 * http://arduino.stackexchange.com/questions/16348/how-do-you-use-spi-on-an-arduino
 * http://www.naylampmechatronics.com/blog/38_Tutorial-Arduino-y-memoria-SD-y-micro-SD-.html
 * https://www.arduino.cc/en/Reference/SD
 * http://zygzax.com/escribir-datos-en-una-tarjeta-sd-con-arduino-modulo-microsd-y-ethernet-shield/
 */

#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

void setup() {
  /**** Inicializar el Serial ****/
  Serial.begin(9600);
  iniEthernet();
  iniSD();
  iniMedidor();
  configurarSPI();
}

void loop() {
  activarMedidor();
  lecturaDatos();
  activarEthernetSD();
  almacenarDatos();
  //Prueba Serial
  //pruebaSerial();
}
