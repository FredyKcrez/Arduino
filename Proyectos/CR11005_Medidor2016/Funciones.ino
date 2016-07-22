/*
 * Esta pestaña contiene todas las funciones correspondientes a la interacción del arduino UNO
 * con la placa del medidor, el arduino Ethernet, la tarjeta SD y el modulo RTC
 */



/*******************************************************************************************
 * Definición de variables útiles para las funciones
 ******************************************************************************************/

/*
 * Constantes
 */
#define csEthernet 10 //CS del arduino ethernet
#define pinSDCard 4 //CS de la tarjeta SD
#define encabezado "Fecha,Hora,Corriente,Voltaje,Energia\n"

/* Factores de multiplicación para la calibración del medidor */
#define factorVoltaje 0.001306967863
#define factorCorriente 0.00003872285917
#define factorEnergia 0.0005
/* Fin de factores de calibración */

/*
 * Variables para el servicio web
 */
byte mac[] = { 0xBA, 0xCA, 0xBA, 0xCA, 0xFE, 0xA5 }; // Dirección MAC correspondiente al arduino Wthernet
EthernetClient cliente;
File datos;
/**** Fin variables para el servicio web ****/

String data = "";
/**** Fin variables del arduino Ethernet ****/

/*
 * Variables que contendran los valores leidos desde el medidor
 * Se inicializan con 0.00 para cuando se envían los datos por primera vez
 */
float voltaje = 0.00;
float corriente = 0.00;
float energia = 0.00;
String fecha = "";
String hora = "";
/* Fin variables de almacenamiento */

/******************************************************************************************/



/*******************************************************************************************
 * Sección correspondiente a las funciones de configuración e inicialización
 ******************************************************************************************/

void configurarSPI() {
  /******* Configuración SPI del medidor *******/
  /*
   * Modos con los que trabaja el SPI
   * SPI_MODE0
   * SPI_MODE1
   * SPI_MODE2
   * SPI_MODE3
   */
  SPI.setDataMode(SPI_MODE1);
  /*
   * Divisores del reloj
   * SPI_CLOCK_DIV2
   * SPI_CLOCK_DIV4
   * SPI_CLOCK_DIV8
   * SPI_CLOCK_DIV16
   * SPI_CLOCK_DIV32
   * SPI_CLOCK_DIV64
   * SPI_CLOCK_DIV128
   */
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  /*
   * Orden del bit
   * LSBFIRST (Menos significativo primero)
   * MSBFIRST (Más significativo primero)
   */
  SPI.setBitOrder(MSBFIRST);
  /**** Inicializar el SPI ****/
  /*
   * Configuración en una sola transacción
   * SPI.beginTransaction(SPISettings (2000000, MSBFIRST, SPI_MODE1));  // 2 MHz clock
   */
  SPI.begin();
  Serial.println(F("HECHO - SPI configurado."));
  /******* Fin Configuración SPI del medidor *******/
}

void iniEthernet() {
  if(!Ethernet.begin(mac))
    Serial.println(F("Error inicializando arduino ethernet"));
  else {
    Serial.println(F("Hecho - Ethernet inicializado..."));
    pinMode(csEthernet, OUTPUT);
    deshabilitar(csEthernet); //Deshabilitamos el pin 10
  }
}

void iniSD() {
  if (!SD.begin(pinSDCard))
    Serial.println(F("Error inicializando tarjeta SD..."));
  else {
    Serial.println(F("HECHO - Tarjeta SD inicializada..."));
    pinMode(pinSDCard, OUTPUT);
    deshabilitar(pinSDCard); //Deshabilitamos el pin 4
  }
}

void iniMedidor() {
  pinMode(csMedidor, OUTPUT);
  deshabilitar(csMedidor);
}

/******************************************************************************************/

/*******************************************************************************************
 * Habilitar/deshabilitar pines
 ******************************************************************************************/

void habilitar(int pin) {
  digitalWrite(pin, LOW);
}

void deshabilitar(int pin) {
  digitalWrite(pin, HIGH);
}

/******************************************************************************************/


/*******************************************************************************************
 * Funciones correlativas al main
 ******************************************************************************************/

/*
 * Lecturas de datos con el medidor
 */
void lecturaDatos() {
  voltaje = vrms();
  voltaje *= factorVoltaje;
  corriente = irms();
  corriente *= factorCorriente;
  energia = getActiveEnergy();
}
/* Fin de las lecturas del medidor */

/*
 * Activar el cs del ethernet y la SD card
 * desactiva el cs del medidor
 * activa el modo 0 con el que trabaja el ethernet y la SD card
 */
void activarEthernetSD() {
  habilitar(pinSDCard); // activamos el pin 4
  habilitar(csEthernet); // activamos el cs del ethernet
  deshabilitar(csMedidor); // desactivamos el cs del medidor
  SPI.setDataMode(SPI_MODE0);
}

/*
 * Activar el cs del medidor
 * desactiva el cs del ethernet y la SD card
 * activa el modo 1 con el que trabaja el ADE7753
 */
void activarMedidor() {
  deshabilitar(pinSDCard); // desactivamos el pin 4
  deshabilitar(csEthernet); // desactivamos el cs del ethernet
  habilitar(csMedidor); // activamos el cs del medidor
  SPI.setDataMode(SPI_MODE1);
}

/*
 * Envío de datos al servidor web y almacenamiento de datos en SD card
 */
void almacenarDatos() {
  servicioWeb();
  data = fecha + "," + hora + "," + (String) corriente + "," + (String) voltaje + "," + (String) energia + "\n";
  guardarSD(data);
}

/*
 * Función para poner en linea la página web
 */
void servicioWeb() {
  data = "volt=" + (String) voltaje + "&corr=" + (String) corriente + "&energ=" + (String) energia;

  if (cliente.connect("192.168.1.2",80)) { // REPLACE WITH YOUR SERVER ADDRESS
    cliente.println(F("POST /servidorWebFredy/add.php HTTP/1.1"));
    cliente.println(F("Host: 192.168.1.2")); // SERVER ADDRESS HERE TOO
    cliente.println(F("Content-Type: application/x-www-form-urlencoded"));
    cliente.print(F("Content-Length: "));
    cliente.println(data.length());
    cliente.println();
    cliente.print(data);
  }

  if (cliente.connected()) {
    cliente.stop();  // DISCONNECT FROM THE SERVER
  }
}

/*
 * Función para el almacenamientode datos en la tarjeta SD
 */
void guardarSD(String data) {
  datos = SD.open("Datos.csv", FILE_WRITE);
  if(datos) {
    datos.print(data);
    datos.close();
  } else
    Serial.println(F("Error almacenando datos"));
}

/******************************************************************************************/




/*******************************************************************************************
 * Funciones para debug
 ******************************************************************************************/

/*
 * Función usada para mostrar los datos en el monitor serial para prueba de lecturas
 */
/*void pruebaSerial() {
  Serial.print("Voltaje:\t");
  Serial.println(voltaje);
  Serial.print("Corriente:\t");
  Serial.println(corriente);
  Serial.print("Energia:\t");
  Serial.println(energia);
  Serial.print("\n");
}*/

/*
 * Pasa un numero de coma flotante a cadana de caracteres
 * Útil si se esta utilizando una versión inferior a la 1.6.8
 * del Arduino IDE (Probada en el Arduino IDE v1.6.5)
 */   
/*String Flotante_a_Cadena(float dato)
{
  String signo = "-";
  (dato  < 0) ?  signo = "-" : signo = " ";
  (dato  < 0) ?  dato = -1*dato : dato = dato;
  int Entero  = (int) dato * 100;
  return  signo + String(Entero/100) + "." + String (Entero%100);  
}*/
