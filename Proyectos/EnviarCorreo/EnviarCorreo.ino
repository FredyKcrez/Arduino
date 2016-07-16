#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xAF, 0xCE, 0xDD }; // La mac que quieras usar.
byte ip[] = { 172, 16, 40, 12 }; //La ip dentro de tu router.
byte gateway [] = { 192,168,1,254 }; // Puede que no haga falta esta línea.
byte subnet [] = { 255,255,255,0 };
byte server[] = { 173,194,34,215 };//  Dirección del servidor smtp. En este caso de Godaddy. Se sabe haciendo un ping al servidor ( smtpout.secureserver.net )
EthernetClient client;
 
/////////////////////////////
//VARS
//Tiempo que damos para calibrar el sensor. (10-60 segundos) Para pruebas pon poco luego ya lo dejas en 60 para activar la alarma y salir.
int calibrationTime = 60;
 
//the time when the sensor outputs a low impulse
long unsigned int lowIn;
 
//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000;
 
boolean lockLow = true;
boolean takeLowTime;
  
void setup()
{
Ethernet.begin(mac, ip);
Serial.begin(9600); 
delay(50);
}
 
void loop()
{
delay(1000);
if (client.available()) {
char c = client.read();
Serial.print(c);
}
 
Serial.println("connecting…");
 
if (client.connect(server, 2525)) {
Serial.println("connected");
client.println("helo smtpout.secureserver.net");
client.println("AUTH LOGIN");                         //Mira en "http://base64-encoder-online.waraxe.us/"
client.println("***********************");           //Nombre de usuario *****@gmail.com.my and encode it
client.println("***********************"); //La contraseña…
delay(10);
client.println("MAIL From: zholary@gmail.com"); // replace the ** with your mail address
delay(10);
client.println("RCPT TO: zholary@gmail.com");       // replace the ** with to mail address
delay(10);
client.println("DATA");
delay(10);
client.println("From: fredy_kcres@hotmail.com");
delay(10);
client.println("TO: zholary@gmail.com");
delay(10);
client.println("SUBJECT: Algo detectado en casa. ALERTA PIR");
delay(10);
client.println();
client.println("ESTADO DE ALERTA");
client.println("¡¡¡PELIGRO ALGO DETECTADO!!");
client.println(".");
client.println(".");
delay(10);
 
delay(1000);
client.stop();
Serial.println("mail sent!!");
delay(30000);
}
else
{
Serial.println("connection failed");
}
}
