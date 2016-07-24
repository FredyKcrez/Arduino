/*********************************************************************
* Universidad de El Salvador
* Facultad de Ingeniería y Arquitectura
* Escuela de ingeniería Electrica
* Sistemas Embebidos
* EBB115 - CicloI - 2016
*
* Estudiante: Br. Fredy A. Cáceres Ramos
* Facilitador: Msc. e Ing. Wilber Calderón
* Fecha de creación de Sketch: 2/Marzo/2016
* Última modificación del Sketch: 23/Julio/2016
* Practica #1 version 0.2
*********************************************************************/

/*
 * Referencias 
 * https://www.arduino.cc/en/Reference/Map descripción de la función map
 * https://www.arduino.cc/en/Tutorial/StringConstructors manejo de cadenas string (Revisar)
 * https://www.arduino.cc/en/Reference/Micros funcion Micros
 * Lecturas rapida
 * http://garretlab.web.fc2.com/en/arduino/inside/arduino/wiring_analog.c/analogRead.html
 * http://forum.arduino.cc/index.php?topic=133907.0
 * http://openenergymonitor.blogspot.com/2012/08/low-level-adc-control-admux.html
 */

/*
 * Entrada: Lectura del voltaje en intervalo 0 - 255
 * Proceso: Conversión de la lectura a un rango de (0 - 5)V usando la función MAP
 * Salida: impresión del valor iterativo, tiempo de captación entre dos lecturas y su valor de voltaje respectivo 
 */

#define pinAnalogico A5 //Pin que leerá la señal analogica del potenciometro

float lectura = 0.00; //almacena el valor leido del potenciometro
float voltaje = 0.00; //almacena el valor del voltaje en intervalo de 0.00 a 5.0
int cont = 1; //contador de iteraciones
float times = 0.00; //almacena el tiempo que tarda en entrar al modulo "principal" desde el inicio de la placa arduino
float tiempo = 0.00; //tiempo que tarda en captura la señal del potenciometro desde el incio del funcionamiento del arduino
String tiempoS; //inicializacion de String; el String contendra toda la cadena a imprimir
int centinela = 100; //Bandera para continuar las lecturas

void setup() {
  Serial.begin(9600);

  bitWrite(ADCSRA,ADPS2,1);
  bitWrite(ADCSRA,ADPS1,0);
  bitWrite(ADCSRA,ADPS0,0);

  // Definiendo el puerto analogico A5 como el puerto de lectura de datos
  // Leer en el enlace para mayor referencia
  ADMUX=(1<<ADLAR)|(0<<REFS1)|(1<<REFS0)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0);
  
  Serial.println(F("i \t Tiempo \t Voltaje")); //Impresión de encabezado de la tabla
}

void loop() {
  if(cont <= centinela) { //muestra unicamente 100 ciclos
    principal(); //llama al modulo de lectura de voltaje y tiempos
    if(tiempo > 1000) //compara si el tiempo entre capturas no alcance los mili segundos
      tiempoS = (String) (tiempo/1000) + " milis \t "; //añado a la cadena a imprimir el tiempo en milisegundos
    else
      tiempoS = (String) tiempo + " micros \t "; //si no tarda mas ó 1 milisegundo añado el tiempo en micro
    Serial.println((String) (cont) + " \t " + tiempoS + (String) voltaje); //impresión de lo valores obtenidos y concatenados a la String
    cont++;
  } else {
    Serial.print("¿Desea continuar?: Yes-1 No-0");
    if(Serial.available()>0) {
      if(Serial.read() == 49)
        centinela = 1000;
    }
  }
}

/*
 * El modulo "principal" tiene por encargo realizar la lectura del potenciometro, los tiempos utilizados
 * y la conversion del valor leido en un rango de (0-5)V
 */
void principal() {
  times = micros(); //captura el tiempo que lleva desde que inició el arduino hasta que entre a la funcion principal en cada loop

  /*
   * para verificar el funcionamiento en milisegundos descomentar el delay de la linea 64:
   * al darle una pausa de 1,2,3,.. milisegundo(s) entre la lectura del tiempo al ingresar al modulo y el tiempo en que captura el valor
   * del potenciometro, produce un retarno en milisegundos, el arduino se queda sin aprovechar ese tiempo de retardo.
   */
  lectura = analogReadFast(); //capta el valor que marca el potenciometro
  tiempo = micros() - times; //captura el tiempo despues en que se lee el voltaje y le resta el tiempo que tardo en arrancar el nuevo ciclo
  voltaje = fmap(lectura, 0, 255, 0, 5); //mapeo del voltaje leido de rango 0-1023 a rango 0-5V
}

/*
 * La siguiente función permite realizar el mapeo de las lecturas de los potenciometros con valores decimales de 2 cifras
 */
float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
