/*void initMedidor()
{
    /*
        Iniciando el medidor
    */
   /* SPI.setDataMode(SPI_MODE2);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin(); 
    pinMode(cs, OUTPUT);           //pin de integrado
    iniciarconstantes();           //Iniciando constantes
    delay(5);  
    digitalWrite(cs, HIGH);
}*/

void initRTC()
{
    /*
        Iniciando el RTC
    */
    Wire.begin(); // Inicia el puerto I2C
    RTC.begin(); // Inicia la comunicación con el RTC
    RTC.adjust(DateTime(__DATE__, __TIME__)); // Establece la fecha y hora
}

/*void initSD()
{
    /*
        Iniciando SD
    */
  /*  Serial.println("Inicializando SD card...");
    if (!SD.begin(cssd)) {
        Serial.println("ERROR - Fallo inicializacion de SD card!");
        return;    
    }
    Serial.println("SUCCESS - SD card initialized.");
    if (!SD.exists("index.htm")) {      // Busca el archivo index
        Serial.println("ERROR - No se encontro archivo index.htm!");
        return; 
    }
    Serial.println("Exito en inicializacion");
    digitalWrite(cssd, HIGH);
}*/

void initEthernet()
{
    /*
        Iniciando Ethernet
    */
    /*pinMode(csEthernet, OUTPUT);
    digitalWrite(csEthernet, HIGH);
    Ethernet.begin(mac, ip);  // Inicializa Ethernet shield*/
    //server.begin(); ///// ---------------------- OPJO OJO OJO OJO OJO
}

void servicioWeb()
{
    EthernetClient client = server.available();  //Verifica y revisa la solicitud hecha por el cliente
    if (client) {  
        boolean actualLineaEnBlanco = true;
        while (client.connected()) {
            if (client.available()) {   
                char c = client.read(); 
                if (aux < (tamanioBuffer - 1)) {
                    bufferSolicitud[aux] = c;          // save HTTP request character
                    aux++;
                }
                if (c == '\n' && actualLineaEnBlanco) {
                    client.println("HTTP/1.1 200 OK");
                    //Solicitudes Ajax
                    if (StrContains(bufferSolicitud, "actualizar")) { //La petición se envió para actualizar valores
                        respuestaXML(client);
                    }
                    else if(StrContains(bufferSolicitud, "registro.csv")){ //La petición se envió para solicitar el archivo de registro
                        descargaArchivo(client);
                    }
                    else if(StrContains(bufferSolicitud, "bajoConsumo")){ //La petición se envió para establecer bajo consumo
                        setBajoConsumo();
                    }
                    else if(StrContains(bufferSolicitud, "consumoNormal")){ //La petición se envió para establecer consumo normal
                        setConsumoNormal();
                    }
                    else if(StrContains(bufferSolicitud, "reiniciarEnergia")){ //La petición se envió para reiniciar acumulación de energía
                        setEnergia(0.0);
                        getEnergia();
                        energiaAgregada = 0.0;
                        resetStatus();
                    }
      		    else{
      			enviarPagina(client);
      		    }
                    // Muestra la solicitud HTTP enviada
                    Serial.println(bufferSolicitud);
                    aux = 0;
                    StrClear(bufferSolicitud, tamanioBuffer);
                    break;
                }
                if (c == '\n') {
                    actualLineaEnBlanco = true;
                } 
                else if (c != '\r') {
                    actualLineaEnBlanco = false;
                }
            } 
        }
        delay(1);      
        client.stop(); // Cierra la conexion
    }
}


// Metodo que envia los valores a todos los clientes
void respuestaXML(EthernetClient cl)
{
    cl.println("Content-Type: text/xml");
    cl.println("Connection: keep-alive");
    cl.println();
    //Responder al cliente con un XML
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<valores>");
    
    cl.print("<valorVoltaje>");
    cl.print(valorVoltaje);
    cl.print("</valorVoltaje>");
    
    cl.print("<valorCorriente>");
    cl.print(valorCorriente);
    cl.print("</valorCorriente>");
    
    cl.print("<valorEnergia>");
    cl.print(valorEnergia);
    cl.print("</valorEnergia>");
    
    cl.print("<fechaHora>");
    cl.print(fechaHora);
    cl.print("</fechaHora>");
    
    cl.print("</valores>");
}

// Metodo que envia el archivo de registro
void descargaArchivo(EthernetClient cl)
{
  // send rest of HTTP header
  cl.println("Content-type: application/octet-stream");
  cl.println("Content-disposition: attachment; filename=registro.csv");
  cl.println("Connection: close");
  cl.println();
  //Enviar archivo
  File webFile = SD.open("registro.csv");        // open file
  if (webFile) {
      while(webFile.available()) {
          cl.write(webFile.read()); // send file to client
      }
      webFile.close();
  }
}

void enviarPagina(EthernetClient cl)
{
  // Envia la página web
  // Envia un encabezado estandar de respuesta http
  cl.println("Content-Type: text/html");
  cl.println("Connection: keep-alive");
  cl.println();
  File archivo = SD.open("index.htm");        // Abre el archivo de la página web
  if (archivo) {
      while(archivo.available()) {
          cl.write(archivo.read()); // Envia la página web al cliente
      }
      archivo.close();
  }
}

void activarSD()
{
  delay (1);
  digitalWrite(cssd, LOW);
  digitalWrite(cs, HIGH);
  digitalWrite(csEthernet, HIGH);
  SPI.setDataMode(SPI_MODE0);
  delay (1);
}

void activarEthernet()
{
  delay (1);
  digitalWrite(csEthernet, LOW);
  digitalWrite(cs, HIGH);
  digitalWrite(cssd, HIGH);
  SPI.setDataMode(SPI_MODE0);
  delay (1);
}

void activarMedidor()
{
  delay (1);
  digitalWrite(cs, LOW);
  digitalWrite(cssd, HIGH);
  digitalWrite(csEthernet, HIGH);
  SPI.setDataMode(SPI_MODE2);
  delay (1);
}

void escribirEnSD()
{
  //Se realiza la verificación y escritura del archivo
    File registro = SD.open("registro.csv", FILE_WRITE);
    if (registro){
      //Se escribe información en el documento de texto registro.csv
        registro.print(fechaHora);
        registro.print(",");
        registro.print(valorVoltaje);
        registro.print(",");
        registro.print(valorCorriente);
        registro.print(",");
        registro.print(valorEnergia);
        registro.println("");
      //Se cierra el archivo para almacenar los datos.
      registro.close();
    }
}

void capturarValoresMedidor()
{
  long voltaje_rms=vrms();
  valorVoltaje= voltaje_rms * factorVoltaje;
  long corriente_irms=irms();
  valorCorriente=corriente_irms * factorCorriente;
  long energiaConsumida = get_energia_consumida();
  energiaAgregada = energiaConsumida * factorEnergia;
  ahora = RTC.now();
  fechaHora = String(ahora.day()) + "/" + String(ahora.month()) + "/" + String(ahora.year()) + " ";
  fechaHora = fechaHora + String(ahora.hour()) + ":" + String(ahora.minute()) + ":" + String(ahora.second());

  Serial.print(fechaHora);
  Serial.print("\t");
  Serial.print(valorVoltaje);
  Serial.print("\t");
  Serial.print(valorCorriente);
  Serial.print("\t");
  valorEnergia = energiaAgregada + energiaAcumulada;
  Serial.println(valorEnergia);
  Serial.println(energiaAgregada);
  Serial.println(energiaAcumulada);
  Serial.println("");
  
  //Guardando en EEPROM
  setEnergia(valorEnergia);
}

//Establece el limite para un bajo consumo
void setBajoConsumo()
{
  limiteCorriente = corrienteBajoConsumoMax;
  Serial.println(limiteCorriente);
}

//Establece el limite para un consumo normal
void setConsumoNormal()
{
  limiteCorriente = corrienteConsumoNormalMax;
  Serial.println(limiteCorriente);
}

void setEnergia(double cantidad)
{
    int addressFloat = 200;
    Serial.println("Guardando acumulado de energia");
    EEPROM.writeDouble(addressFloat,cantidad);
}

void getEnergia()
{
    int addressFloat = 200;
    Serial.println("Leyendo acumulado de energia");
    energiaAcumulada = EEPROM.readDouble(addressFloat);
}


byte enviarEmail(String asunto, String mensaje, String destinatario)
{
	byte thisByte = 0;
	byte respCode;

	if(mailCliente.connect(mailServer,mailPort) == 1) {
		Serial.println(F("Conectado"));
	} else {
		Serial.println(F("Conexión fallida"));
		return 0;
	}

	if(!eRcv()) return 0;

	Serial.println(F("Enviando hello"));
	// IP del Arduino
	mailCliente.println(localDominio);
	if(!eRcv()) return 0;

	Serial.println(F("Enviando auth login"));
	mailCliente.println("auth login");
	if(!eRcv()) return 0;

	Serial.println(F("Enviando User"));
	// Change to your base64 encoded user (medidor.arduino@gmail.com)
	mailCliente.println("bWVkaWRvci5hcmR1aW5vQGdtYWlsLmNvbQ==");
	if(!eRcv()) return 0;

	Serial.println(F("Enviando Password"));
	// change to your base64 encoded password (medidorarduino.1)
	mailCliente.println("bWVkaWRvcmFyZHVpbm8uMQ==");
	if(!eRcv()) return 0;

	// Email del remitente
	Serial.println(F("Enviando remitente"));
	mailCliente.println("MAIL From: <medidor.arduino@gmail.com>");
	if(!eRcv()) return 0;

	// Email del destinatario
	Serial.println(F("Enviando destinatario"));
	mailCliente.println("RCPT To: <" + destinatario + ">");
	if(!eRcv()) return 0;

	Serial.println(F("Enviando DATA"));
	mailCliente.println("DATA");
	if(!eRcv()) return 0;

	Serial.println(F("Enviando email"));

	// Email de destinatario
	mailCliente.println("To: Usuario <" + destinatario + ">");
	// Email del remitente
	mailCliente.println("From: MedidorArduino <medidor.arduino@gmail.com>");

	//Asunto del correo
	mailCliente.println("Subject: " + asunto + "\r\n");
	//Cuerpo del mensaje
	mailCliente.println(mensaje);
	//Fin del mensaje
	mailCliente.println(".");
	if(!eRcv()) return 0;

	Serial.println(F("Enviando QUIT"));
	mailCliente.println("QUIT");
	if(!eRcv()) return 0;

	mailCliente.stop();

	Serial.println(F("Email - Desconectado"));

	return 1;
}
