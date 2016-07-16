/*
 * Estudiantes:                       Carnet:
 * Cáceres Ramos, Fredy Antonio       CR11005
 * Segovia Salazar, Mario Alejandro   SS12008
 * 
 * Facultad de Ingeniería & Arquitectura
 * Escuela de Ingeniería Electrica
 * EEBDEA115
 * Parcial 1
 * 
 * Instructor: Msc. Wilber Calderón
 * Fecha: Jueves 21 de Abril de 2016 9:09 a.m.
 */

// Definición de variable

const int RCLK = 8; //latchPin
const int SRCLK = 12; //clockPin
const int SER = 11; //dataIn

const int pinIn = A0; //pin analogico A0 para lectura del potenciometro
int lectura; //variable que contendra temporalmente el valor leido
float voltaje; //variable que contendra el valor de la lectura en un rango de 0-5v
int bandera = 0; //bandera que cambiará a 1 unicamente cuando sobrepase el umbral, de lo contrario se mantendra en 0
int contador = 0; //contador de las veces que se ha superado el umbral

void setup() {
  //Definiendo pines de salida para los 74595
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SER, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Parcial 1");
}

void loop() {
  lectura = analogRead(pinIn); //lectura del valor en el pin Analogico
    voltaje = fmap(lectura, 0, 1023, 0.0, 5.0); //mapeo del voltaje leido de rango 0-1023 a rango 0-5V

    /*
     * Condicional que accede si el voltaje leido es mayor a 3.1v y la bandera no ha sido modificada, es decir
     * el valor leido se mantiene inferior o igual a 3.1v.
     */
    if(volatje > 3.1 && bandera == 0) {
      contador++; //aumentamos el contador si sobrepasa el umbral
      bandera = 1; //modificamos el estado de la bandera para saber que se mantiene superado el umbral
    }
    else
      bandera = 0; //en caso de no ser superado el umbral, vuelve a colocar a cero la bandera, indicandonos que estamos abajo de umbral

    digitalWrite(RCLK, LOW);
    bitWrite(bitsToSend, whichPin, HIGH);
    shiftOut(SER, SRCLK, MSBFIRST, bitsToSend);
    digitalWrite(RCLK, HIGH);
}
