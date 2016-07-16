//HECTOR ALCIDES FRANCO PAREDES
//RICARDO JOSE PACHECO MENDEZ
//declarando librerias a utilizar
#include <SPI.h>
//declarando alcunas constantes principales
const int cs=5;       //selector chip de integrado
const byte leer=0b00000000;    
const byte escribir=0b10000000;
void setup(){
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  
  Serial.begin(9600);
  pinMode(2, OUTPUT);           //pin de integrado
  
  //iniciamos algunas constantes 
 
  iniciarconstantes();
  delay(5);  
  csalto();
 }
 void loop (){
 long voltaje_rms=vrms();
  //float voltaje_rms_corregido= voltaje_rms*0.000264585;
  float voltaje_rms_corregido= voltaje_rms*0.0001214113;
  Serial.print("V: \t");
  Serial.print(voltaje_rms_corregido);//borrame solo para calibrar
  Serial.println("\t");
  //ahora obtenemos la corriente rms
  //capturando corriente rms en su respectivo registro
 
  long corriente_irms=irms();
  //float corriente_irms_corregido=corriente_irms*0.000048575;
  float corriente_irms_corregido=corriente_irms*0.0000363276;
  Serial.print("A: \t");
  Serial.println(corriente_irms_corregido);//borrame solo para calibrar
 
  long energiaConsumida = get_energia_consumida();
  float energiaAgregada = energiaConsumida * 0.000118862845443281;
  Serial.print("E: \t");
  Serial.println(energiaAgregada);//borrame solo para calibrar
 
 }
