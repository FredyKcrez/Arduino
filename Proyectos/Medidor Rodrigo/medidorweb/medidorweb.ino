#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <Ethernet.h> 
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>
#include <Wire.h>


//Variables usadas para el servidor web

#define tamanioBuffer   40                           // buffer para capturar solicitud HTTP
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //MAC del servidor
IPAddress ip(172, 16, 40, 120);                        // IP del servidor
EthernetServer server(80);                           // Crear servidor en puerto 80
char bufferSolicitud[tamanioBuffer] = {0};           // Variable para almacenar solicitud HTTP
char aux = 0;                                        // variable auxiliar

//Variables usadas para la RTC

RTC_DS1307 RTC;
DateTime ahora; 

//Valores que se envian a la página web

float valorVoltaje = 0.01;
float valorCorriente = 0.01;
double valorEnergia = 0.01;
String fechaHora = "";

//Declarando algunas constantes principales

const int cssd=4;           //Chip select de tarjeta SD
const int csEthernet=10;    //Chip select de shield Ethernet
const int cs=2;             //Chip select del medidor
const byte     leer=0b00000000;    
const byte escribir=0b10000000;
const double factorVoltaje = 0.0001214113;          //Factor de registro de voltaje
const double factorCorriente = 0.0000363276;        //Factor de registro de corriente
const double factorEnergia = 0.000118862845443281;  //Factor de registro de energía

//Variables usadas para las restricciones de consumo

const float corrienteBajoConsumoMax = 2.0;
const float corrienteConsumoNormalMax = 10.0;
const float voltajeMax = 130.0;
const float voltajeMin = 110.0;
const float energiaAcumuladaMax = 10.0;
double energiaAcumulada;  // Cantidad guardada de la EEPROM del ultimo vaor acumulado
float limiteCorriente;
double energiaAgregada;  //Energía medida por el ADE7753

//Variables usadas para el envio de email

char mailServer[] = "smtpcorp.com";
int mailPort = 2525;
EthernetClient mailCliente;
String localDominio = "EHLO 172.16.40.120";
boolean prueba = true;


void setup()
{
    //Inicializando todos los elementos
    Serial.begin(9600); 
    initMedidor();
    initRTC();
    initSD();
    initEthernet();
    setConsumoNormal();   //Se establece consumo normal por defecto  
    setEnergia(0.0);      // Utilizando para pruebas
    getEnergia();         //Se obtiene el acumulado de la energia de la eeprom  
}

void loop()
{
    activarEthernet();
    servicioWeb();
    activarMedidor();
    capturarValoresMedidor();
    activarSD();
    escribirEnSD();
    if(valorCorriente >= limiteCorriente)
    {
	activarEthernet();
        if(enviarEmail("Alerta de uso de corriente", 
        "Arduino ha detectado un consumo de corriente excesivo, asegurese de apagar aparatos innecesarios!",
        "rodrigoahv@hotmail.com")) 
        Serial.println(F("Email enviado"));
	else Serial.println(F("Email fallido"));
    }
    if(valorVoltaje >= voltajeMax)
    {
        activarEthernet();
        if(enviarEmail("Alerta de variación de voltaje", 
        "Arduino ha detectado una variación de voltaje que podría ser perjudicial para sus aparatos eléctricos!",
        "rodrigoahv@hotmail.com")) 
        Serial.println(F("Email enviado"));
	else Serial.println(F("Email fallido"));
    }
    if(valorVoltaje <= voltajeMin)
    {
        activarEthernet();
        if(enviarEmail("Alerta de variación de voltaje", 
        "Arduino ha detectado una variación de voltaje que podría ser perjudicial para sus aparatos eléctricos!",
        "rodrigoahv@hotmail.com")) 
        Serial.println(F("Email enviado"));
	else Serial.println(F("Email fallido"));
    }
    /*if(valorEnergia -1 >= energiaAcumuladaMax)
    {
        activarEthernet();
        if(enviarEmail("Alerta de consumo de energía", 
        "Arduino ha detectado que está cerca de llegar al consumo máximo de energía mensual, si lo sobrepasa podría perder su subsidio!",
        "rodrigoahv@hotmail.com")) 
        Serial.println(F("Email enviado"));
	else Serial.println(F("Email fallido"));
    }*/
    if(prueba)
    {
	activarEthernet();
        if(enviarEmail("Prueba de funcionamiento", 
        "Arduino está probando el funcionamiento de los emails!",
        //"78642106@sms.claro.com.sv"))
        "rodrigoahv@hotmail.com")) 
        Serial.println(F("Email enviado"));
	else Serial.println(F("Email fallido"));
        prueba = false;
    }
}
