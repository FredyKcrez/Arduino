void inicializar() {
  Serial.println(Ethernet.localIP());
  Serial.println("Inicializando tarjeta SD...");
  if(!SD.begin(4)) {
    Serial.println("ERROR - Falló al inicializar la SD");
    return;
  }
  Serial.println("EXITO - Buscando archivo htm");
}
