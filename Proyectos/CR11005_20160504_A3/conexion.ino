void conexion(int salida) {
  char c = client.read();
  if(c == '\n') {
    if(currentLineIsBlank) {
      //encabezado estandar
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connnection: close");
      client.println();

      //esqueleto de la página web
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>Arduino Read Switch State</title>");
      client.println("<meta http-equiv=\"refresh\" content=\"30\">");
      client.println("</head>");
      client.println("<body>");
      client.println("<h1>Asignaci&oacute;n 3</h1>");
      client.println("<p>State of switch is:</p>");
      client.println("</body>");
      client.println("</html>");
      break;
    }
    else
      currentLineIsBlank = true;
  }
  else if(c == '\r')
    currentLineIsBlank = false;
}
