/*********************************************************************
* Universidad de El Salvador
* Facultad de Ingeniería y Arquitectura
* Escuela de ingeniería Electrica
* Sistemas Embebidos
* EBB115 - CicloI - 2016
*
* Estudiante: Br. Fredy A. Cáceres Ramos
* Facilitador: Msc. e Ing. Wilber Calderón
* Fecha de creación de Sketch: 26/Febrero/2016
* Última modificación del Sketch: 20/Julio/2016
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

String cad = "i \t Tiempo \t Voltaje"; //inicializacion de String; el String contendra toda la cadena a imprimir
float lectura = 0.00; //almacena el valor leido del potenciometro
float voltaje = 0.00; //almacena el valor del voltaje en intervalo de 0.00 a 5.0
int cont = 1; //contador de iteraciones
float times = 0.00; //almacena el tiempo que tarda en entrar al modulo "principal" desde el inicio de la placa arduino
float tiempo = 0.00; //tiempo que tarda en captura la señal del potenciometro desde el incio del funcionamiento del arduino 

/*
 * Funcionamiento del tiempo:
 * Ya que las funciones de tiempo de arduino (micros() y millis()) captan el tiempo desde que el arduino comenzo a operar el codigo
 * hasta que se hace el llamado del mismo con estas funciones, he utilizado la operación aritmetica de resta entre dos puntos de tiempos
 * especificos, el primero de ellos es el almacenado en la variable times, la cual almacena el tiempo que ha transcurrido desde que
 * se inicio el arduino y el tiempo que transcurria cuando el sistema ingreso a la funcion principal, posteriormente el arduion realiza
 * la lectura del voltaje, y por ultimo el tiempo posterior a la lectura de esta, almacenando su valor en la variable tiempo, de igual
 * manera desde que se inicio el arduino hasta que se haec el llamado con la funcion Micro(), posteriormente a este tiempo le resto
 * el tiempo tomado al ingresar a la funcion principal (almacenada en times)
 */

void setup() {
  Serial.begin(9600);

  bitWrite(ADCSRA,ADPS2,1);
  bitWrite(ADCSRA,ADPS1,0);
  bitWrite(ADCSRA,ADPS0,0);

  // Definiendo el puerto analogico A5 como el puerto de lectura de datos
  // Leer en el enlace para mayor referencia
  ADMUX=(1<<ADLAR)|(0<<REFS1)|(1<<REFS0)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0);

  Serial.println(cad); //Impresión de encabezado de la tabla
}

void loop() {
  if(cont<=1000) { //muestra unicamente 1000 ciclos
    principal(); //llama al modulo de lectura de voltaje y tiempos
    cad = (String) cont + " \t "; //sobreescribe el dato que contenia la String para dar inicio a una nueva lectura
    if(tiempo>1000) { //compara si el tiempo entre capturas no alcance los mili segundos
      float timeMili = tiempo/1000; //si tarda mas de 1000 microsegundos, lo convierto a milisegundos
      cad = cad + (String) timeMili + " milis \t "; //añado a la cadena a imprimir el tiempo en milisegundos
    }
    else {
      cad = cad + (String) tiempo + " micros \t "; //si no tarda mas ó 1 milisegundo añado el tiempo en micro
    }
    cad = cad + (String) voltaje; //añade el voltaje a la string
    Serial.println(cad); //impresión de lo valores obtenidos y concatenados a la String
    cont++;
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
  lectura = analogReadFast(); //analogRead(pinAnalogico); //capta el valor que marca el potenciometro
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
