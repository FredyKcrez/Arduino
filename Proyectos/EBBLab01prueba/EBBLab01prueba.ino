/*
 * Cáceres Ramos, Fredy Antonio - CR11005
 * EBB115 - CicloI - 2016
 * Fecha inicio Sketch: 26/Febrero/2016
 * Fecha fin: 2/Marzo/2016
 * Practica 1
 */

/*
 * Referencias 
 * https://www.arduino.cc/en/Reference/Map descripción de la función map
 * https://www.arduino.cc/en/Tutorial/StringConstructors manejo de cadenas string (Revisar)
 * https://www.arduino.cc/en/Reference/Micros funcion Micros
 * 
 */

/*
 * Entrada: Lectura del voltaje en intervalo 0 - 1023
 * Proceso: Conversión de la lectura a un rango de (0 - 5)V usando la función MAP
 * Salida: impresión del valor iterativo, tiempo de captación entre dos lecturas y su valor de voltaje respectivo 
 */

 const int pinAnalogico = A0; //Pin que leerá la señal analogica del potenciometro
 float lectura = 0.00; //almacena el valor leido del potenciometro
 float voltaje = 0.00; //almacena el valor del voltaje en intervalo de 0.00 a 5.0
 int cont = 1; //contador de iteraciones
 float times = 0.00; //almacena el tiempo que tarda en entrar al modulo "principal" desde el inicio de la placa arduino
 float tiempo = 0.00; //tiempo que tarda en captura la señal del potenciometro desde el incio del funcionamiento del arduino
 String cad = "i \t Tiempo \t Voltaje"; //inicializacion de String; el String contendra toda la cadena a imprimir
 int more = 0;

/*
 * Funcionamiento del tiempo:
 * Ya que con las funciones mili
 */

void setup() {
  Serial.begin(9600);
  Serial.write(0x0d); //Limpiar el monitor serial, para que no muestra datos basura
  Serial.println(cad); //Impresión de encabezado de la tabla
}

void loop() {
  if(cont<=10 || more == 1) { //muestra unicamente 10 ciclos
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
  } else {
    Serial.print("¿Desea continuar?: Yes-1 No-0");
    delay(5000);
    if(Serial.available()>0){
      more = (int) Serial.read();
      Serial.println(more);
    }
  }
  cont++;
  delay(100);
}

/*
 * El modulo "principal" tiene por encargo realizar la lectura del potenciometro, los tiempos utilizados
 * y la conversion del valor leido en un rango de (0-5)V
 */
void principal() {
  times = micros(); //captura el tiempo que lleva desde que inició el arduino hasta que entre a la funcion principal en cada loop
  //delay(1);
  /*
   * para verificar el funcionamiento en milisegundos descomentar el delay de la linea 64:
   * al darle una pausa de 1,2,3,.. milisegundo(s) entre la lectura del tiempo al ingresar al modulo y el tiempo en que captura el valor
   * del potenciometro, produce un retarno en milisegundos, el arduino se queda sin aprovechar ese tiempo de retardo.
   */
  lectura = analogRead(pinAnalogico); //capta el valor que marca el potenciometro
  tiempo = micros() - times; //captura el tiempo despues en que se lee el voltaje y le resta el tiempo que tardo en arrancar el nuevo ciclo
  voltaje = map(lectura, 0, 1023, 0, 5); //mapeo del voltaje leido de rango 0-1023 a rango 0-5V
}
