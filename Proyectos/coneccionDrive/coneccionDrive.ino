#include <SPI.h>
#include <Ethernet.h>

char formKey[] = "15x6VCMZ7WGGknaRTGDOKsKSruLGuLhaC6eNeD9AnQ9k";
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xDC, 0x81 };
byte ip[] = { 192,168,1,12 };
byte subnet[] = { 255,255,255,0 };
byte servdns[] = { 8,8,8,8 };
byte gateway[] = { 192,168,1,11 };

char miServidor[] = "spreadsheets.google.com";
String datos = "";
EthernetClient miCliente;

void setup() {
  Ethernet.begin(mac, ip, servdns, gateway, subnet);
  delay(1000);
}

void loop() {
  datos = "entry.2028571785=4";
  datos = datos +"&entry.100308794_day=13";
  datos = datos +"&entry.100308794_month=5";
  datos = datos +"&entry.100308794_year=2016";
  datos = datos +"&entry.1114396481_hour=22";
  datos = datos +"&entry.1114396481_minute=31";
  datos = datos +"&entry.1019121252=2";
  datos = datos +"&submit=Submit";
  if(miCliente.connect(miServidor, 80)!=0) {
    miCliente.print("POST /formResponse?formkey=");
    miCliente.print(formKey);
    miCliente.println("&ifq HTTP/1.1");
    miCliente.println("Host: spreadsheets.google.com");
    miCliente.println("Content-Type: application/x-www-form-urlencoded");
    miCliente.println("Connection: close");
    miCliente.print("Content-Length: ");
    miCliente.println(datos.length());
    miCliente.println();
    miCliente.print(datos);
    miCliente.println();
  }
  delay(1000);
  if(miCliente.connected() == true) {
    miCliente.stop();
  }
  delay(1000);
}
