 
 //registros
void iniciarconstantes(void){
#define WAVEFORM 0x01
#define AENERGY 0x02
#define RAENERGY 0x03
#define LAENERGY 0x04
#define VAENERGY 0x05
#define RVAENERGY 0x06
#define LVAENERGY 0x07
#define LVARENERGY 0x08
#define MODE 0x09
#define IRQEN 0x0A
#define STATUS 0x0B
#define RSTSTATUS 0x0C
#define CH1OS 0x0D
#define CH2OS 0x0E
#define GAIN 0x0F
#define PHCAL 0x10
#define APOS 0x11
#define WGAIN 0x12
#define WDIV 0x13
#define CFNUM 0x14
#define CFDEN 0x15
#define IRMS 0x16
#define VRMS 0x17
#define IRMSOS 0x18
#define VRMSOS 0x19
#define VAGAIN 0x1A
#define VADIV 0x1B
#define LINECYC 0x1C
#define ZXTOUT 0x1D
#define SAGCYC 0x1E
#define SAGLVL 0x1F
#define IPKLVL 0x20
#define VPKLVL 0x21
#define IPEAK 0x22
#define RSTIPEAK 0x23
#define VPEAK 0x24
#define RSTVPEAK 0x25
#define TEMP 0x26
#define PERIOD 0x27
#define TMODE 0x3D
#define CHKSUM 0x3E
#define DIEREV 0X3F


//bits

//bits del registro de modo
#define DISHPF     0 
#define DISLPF2    1 
#define DISCF      2
#define DISSAG     3
#define ASUSPEND   4 
#define TEMPSEL    5
#define SWRST      6
#define CYCMODE    7 
#define DISCH1     8
#define DISCH2     9
#define SWAP      10
#define DTRT1     11
#define DTRT0     12
#define WAVSEL1   13
#define WAVSEL0   14
#define POAM      15
//interrupciones
#define AEHF      0x0001 // bit 0 - 
#define SAG       0x0002 // bit 1 - 
#define CYCEND    0x0004 // bit 2 - 
#define WSMP      0x0008 // bit 3 - 
#define ZX        0x0010 // bit 4 - 
#define TEMPREADY 0x0020 // bit 5 - 
#define RESET     0x0040 // bit 6 - .
#define AEOF      0x0080 // bit 7 - 
#define PKV       0x0100 // bit 8 - 
#define PKI       0x0200 // bit 9 - 
#define VAEHF     0x0400 // bit 10 - 
#define VAEOF     0x0800 // bit 11 - 
#define ZXTO      0x1000 // bit 12 - 
#define PPOS      0x2000 // bit 13
#define PNEG      0x4000 // bit 14 -
#define RESERVED  0x8000 // bit 15 - reservado		


}
 
 //habilitar y deshabilitar cs
void csbajo(void){
 digitalWrite(cs,LOW); 
}
void csalto(void){
digitalWrite(cs,HIGH); 
}
unsigned char leer8(char reg){
 reg = reg | leer;
  csbajo();
 delayMicroseconds(5);
  SPI.transfer(reg);
 delayMicroseconds(5); 
 return (unsigned char)SPI.transfer(0x00);//cambiar por unsigned byte
 csalto();
}
unsigned int leer16(char reg){
  reg = reg | leer;
  csbajo();
  unsigned char b1;
  unsigned char b0;
   delayMicroseconds(5);
   SPI.transfer(reg);
   delayMicroseconds(5);
   b1=SPI.transfer(0x00);
   delayMicroseconds(5);
   b0=SPI.transfer(0x00);
   return (unsigned int)b1<<8 | (unsigned int)b0;  
   csalto;
}
unsigned long leer24(char reg){
  reg = reg | leer;
  csbajo();
  unsigned char b2,b1,b0;
  delayMicroseconds(10);
  SPI.transfer(reg);
  delayMicroseconds(25);
  b2=SPI.transfer(0x00);
  delayMicroseconds(5);
  b1=SPI.transfer(0x00);
  delayMicroseconds(5);
  b0=SPI.transfer(0x00);
  return (unsigned long)b2<<16 | (unsigned long)b1<<8 | (unsigned long)b0;
  csalto();
}
void escribir8(char reg, char data){
 reg = reg | escribir;
 csbajo();
 delayMicroseconds(10);
 SPI.transfer((unsigned char)reg);          //register selection
 delayMicroseconds(5);
 SPI.transfer((unsigned char)data);
 delayMicroseconds(5);
 csalto();
  
} 
void escribir16(char reg, int data){
    reg = reg | escribir;
    csbajo();
    unsigned char data0=0;
    unsigned char data1=0;
    data0 = (unsigned char)data;
    data1 = (unsigned char)(data>>8);
    delayMicroseconds(10);
    SPI.transfer((unsigned char)reg);    
    delayMicroseconds(5);    
    //envio de dato, MSB primeros
    SPI.transfer((unsigned char)data1);
    delayMicroseconds(5);
    SPI.transfer((unsigned char)data0);  
    delayMicroseconds(5);
    csalto(); 
}

void setMode(int m){
    escribir16(MODE, m);
}
int getMode(){
    return leer16(MODE);
}

int getInterrupts(void){
    return leer16(IRQEN);
}
void setInterrupts(int i){
    escribir16(IRQEN,i);
}
int getsuma(void){
return leer8(CHKSUM);
}

int getStatus(void){
    return leer16(STATUS);
}

int resetStatus(void){
    return leer16(RSTSTATUS);
}
//obtener consumo total
long get_energia_consumida(void){
return leer24(AENERGY);  
}

//obtener potencia activa
long get_pot_act(void){
return leer24(RAENERGY);

}
long get_pot_apa(void){
return leer24(RVAENERGY);   
}

//obteer valor IRMS
long getIRMS(void){
long lastupdate=0;
resetStatus();
lastupdate=millis();
while (! (getStatus() & ZX ))
{//Serial.println("encerrado en irms");
long contando=millis();
long salirse=contando-lastupdate;
if ( salirse > 200) break;
}
return leer24(IRMS);
}
//obtener valor VRMS
long getVRMS(void){
long lastupdate=0;
resetStatus();
lastupdate=millis();
while (! (getStatus() & ZX ))
{//Serial.println("encerrado en vrms");
long contando=millis();
long salirse=contando-lastupdate;
if ( salirse > 200) break;
}
return leer24(VRMS);
}
//LA SIGUIENTE FUNCION
//INGNORA LAS PRIMERAS GETRMS Y SACA EL PROMEDIODE 100 LECTURAS
long vrms(){
	char i=0;
	long v=0;
	getVRMS();//ignora la primera lectura para evitar basura
	getVRMS();
        getVRMS();
        for(i=0;i<50;++i){
		v+=getVRMS();
	}
	return v/50;
}
//ahora para la corriente
long irms(){
	char n=0;
	long i=0;
	getIRMS();//ignora la primera lectura para evitar basura
	getIRMS();
        getIRMS();
          for(n=0;n<50;++n){
		i+=getIRMS();
	}
	return i/50;
}


